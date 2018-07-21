
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL_image/SDL_image.h>
#include <iostream.h>
#include <fstream.h>
#include <sstream>
#include <SDL_ttf/SDL_ttf.h>
#include <SDL_mixer/SDL_mixer.h>
#include <vector>


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 32

#define CLIP_MOUSEOVER 0
#define CLIP_MOUSEOUT 1
#define CLIP_MOUSEDOWN 2
#define CLIP_MOUSEUP 3	

#define FRAMES_PER_SEC 100

#define APP_NAME "Move the dot"

#define BACKGROUND_FILE "background.png"
#define FONT_FILE "lazy.ttf"
#define LOG_FILE "log.txt"

using namespace std;


void apply_surface(int x, int y, SDL_Surface* src, SDL_Surface* des);
void apply_surface(int x, int y, SDL_Surface* src, SDL_Surface* des, SDL_Rect* clip);
Uint8 check_collision(SDL_Rect boxA, SDL_Rect boxB);
Uint32 getPixel(SDL_Surface* screen, int x, int y);
void plotRect(SDL_Rect r, Uint32, SDL_Surface* surf);


SDL_Surface* loadImage(string file);

class LogFile {
private:
	ofstream logf;
	string filename;
public:
	LogFile(string fileN) {
		filename = fileN;
	}
	
	void out(string str) {
		logf.open(filename.c_str(), ios_base::app | ios_base::out);
		logf << str << endl;
		logf.close();
	}
	
	void out(int i) {
		logf.open(filename.c_str(), ios_base::app | ios_base::out);
		logf << i << endl;
		logf.close();
	}
	
	void setFile(string fileN) {
		filename = fileN;
	}
	
};
class Button  {
	
private:
	SDL_Rect box;		//button attributes
	SDL_Rect clip[4];		//clip from sprite sheet
	int mouse;
	
public:
	Button(int x, int y, int w, int h) {		//contruct button
		
		box.x = x;
		box.y = y;
		box.w = w;
		box.h = h;
		
		mouse = CLIP_MOUSEOUT;
		
		clip[CLIP_MOUSEOVER].x = 0;
		clip[CLIP_MOUSEOVER].y = 0;
		clip[CLIP_MOUSEOVER].w = 320;
		clip[CLIP_MOUSEOVER].h = 240;
		
		clip[CLIP_MOUSEOUT].x = 320;
		clip[CLIP_MOUSEOUT].y = 0;
		clip[CLIP_MOUSEOUT].w = 320;
		clip[CLIP_MOUSEOUT].h = 240;
		
		clip[CLIP_MOUSEDOWN].x = 0;
		clip[CLIP_MOUSEDOWN].y = 240;
		clip[CLIP_MOUSEDOWN].w = 320;
		clip[CLIP_MOUSEDOWN].h = 240;
		
		clip[CLIP_MOUSEUP].x = 320;
		clip[CLIP_MOUSEUP].y = 240;
		clip[CLIP_MOUSEUP].w = 320;
		clip[CLIP_MOUSEUP].h = 240;
	}
	
	
	void handle_events(SDL_Event* event) {			//handle events and set image
		int x, y;
		
		if (event->type == SDL_MOUSEMOTION) {
			x = event->motion.x;
			y = event->motion.y;
			
			if ( (x > box.x) && (y > box.y) && (x < box.x+box.w) && (y < box.y+box.h) ) {
				mouse = CLIP_MOUSEOVER;
			}
			else {
				mouse = CLIP_MOUSEOUT;
			}
		}
		
		if (event->type == SDL_MOUSEBUTTONDOWN) {
			if (event->button.button == SDL_BUTTON_LEFT) {
				x = event->button.x;
				y = event->button.y;
				
				if ((x > box.x) && (y > box.y) && (x < box.x+box.w) && (y < box.y+box.h)) {
					mouse = CLIP_MOUSEDOWN;
				}
			}
		}
		
		if (event->type == SDL_MOUSEBUTTONUP) {
			if (event->button.button == SDL_BUTTON_LEFT) {
				x = event->button.x;
				y = event->button.y;
				
				if ((x > box.x) && (y > box.y) && (x < box.x+box.w) && (y < box.y+box.h)) {
					mouse = CLIP_MOUSEUP;
				}
			}
		}
}
	
	void show(SDL_Surface* src, SDL_Surface* des) {		//show image
		apply_surface(box.x, box.y , src, des, &clip[mouse]);
	}
	
};
class Timer {
private:
	Uint32 start_ticks;
	Uint32 time;
	Uint8 timerstate; // 0 stoped, 1 paused, 2 running
	
public:
	Timer() 
	:start_ticks(0), time(0), timerstate(0)
	{}
	
	void start() {
		switch (timerstate) {
			case 0:
				time = 0;
				start_ticks = SDL_GetTicks();
				timerstate = 2;
				break;
			case 1:
				start_ticks = SDL_GetTicks();
				timerstate = 2;
				break;
			case 2:
				start_ticks = SDL_GetTicks();
				time = 0;
				break;
			default:
				cerr<<"no such state: " << timerstate;
				break;
		}
	}
	
	void stop() {
		switch (timerstate) {
			case 0:
				cerr<<"timer's already stoped";
				break;
			case 1:
				timerstate = 0;
				break;
			case 2:
				time += SDL_GetTicks() - start_ticks;
				timerstate = 0;
				break;
			default:
				break;
		}
	}
	
	void pause() {
		switch (timerstate) {
			case 0:
				cerr<<"timer's not running";
				break;
			case 1:
				cerr<<"timer's already paused";
				break;
			case 2:
				time += SDL_GetTicks() - start_ticks;
				timerstate = 1;
				break;
			default:
				cerr<<"no such state: " << timerstate;
				break;
		}
	}
	
	Uint32 getTicks() {
		switch (timerstate) {
			case 0: case 1:
				return time;
				break;
			case 2:
				return time + (SDL_GetTicks() - start_ticks);
				break;
			default:
				cerr<<"no such state: " << timerstate;
				return 0;
				break;
		}
	}
	
	bool isPaused() {
		if (timerstate == 1) {
			return true;
		}
		else {
			return false;
		}
	}
	
	bool isRunning() {
		if (timerstate == 2) {
			return true;
		}
		else {
			return false;
		}
	}
	
	bool isStoped() {
		if (timerstate == 0) {
			return true;
		}
		else {
			return false;
		}
	}

};
class FpsRegulator {
private:
	Timer time;
	Timer real_time;
	Uint32 msec_per_frame;
	int framecount;
	Uint32 time_sum;
		
public:
	FpsRegulator(Uint32 fps) 
	: framecount(0), time_sum(0)
	{
		setFps(fps); //sets msec_per_frame;
	}
	
	void setFps(Uint32 fps) { 
		msec_per_frame = 1000/fps; 
	}
	
	void reset() {
		time.stop();
		time.start();
		framecount = 0;
		time_sum = 0;
	}
	
	void stop() {
		time.stop();
		framecount = 0;
		time_sum = 0;
	}
	
	
	void getFps(Uint32 interval, double* fps) {
		
		if (real_time.isRunning()) {
			
			framecount++;
			
			if (interval < real_time.getTicks() ) {
				*fps = double(framecount*1000)/real_time.getTicks();
				framecount = 0;
				real_time.start();
			}
		}
		else {
			real_time.start();
		}

	}
	
	void regulateFps() {
		
		if ( time.isRunning() ) {
			Uint32 ms = time.getTicks();
			if (ms < msec_per_frame) {
				SDL_Delay( msec_per_frame - ms );
			}
		}
		
		time.start();
	}
	
};
class Repport {
	
private:
	int intervals;
	int counter;
	
public:
	Repport(int intervals)
	:intervals(intervals), counter(0)
	{}
	
	void repport(string message) {
		if ( counter >= intervals ) {
			cout << message;
			counter = 0;
		}
	}
	void repport(double message) {
		if ( counter >= intervals ) {
			cout << message;
			counter = 0;
		}
	}
	void repport(char message) {
		if ( counter >= intervals ) {
			cout << message;
			counter = 0;
		}
	}
	void repport(int message) {
		if ( counter >= intervals ) {
			cout << message;
			counter = 0;
		}
	}
	
	void count() {
		counter++;
	}
	
};
class Dot {
private:
	float velX, velY;
	SDL_Rect constraint;
	float velocity;
	string dotfile;
	SDL_Surface* dot;
	float speedSense;
	float distX, distY;
	bool stoped;
	Uint8 *keystate;
	vector<SDL_Rect> rects;
	int rectsOfsetX;
	int rectsOfsetY;
	
	bool isRect(vector<SDL_Rect> rec, int x, int y) {
		
		if (outofboundry(dot, x, y) ) {
			cerr<<"out of boundry isRect()";
			return false;
		}
		
		for (int i = 0 ; i < rec.size() ; i++) {
			if (x >= rec[i].x && x < rec[i].x+rec[i].w && y >= rec[i].y && y < rec[i].y + rec[i].h) {
				return true;
			}
		}
		return false;
	}
	bool outofboundry(SDL_Surface*, int x, int y) {
		if (x >= constraint.w || x < 0 || y >= constraint.h || y < 0 ) {
			return true;
		}
		else {
			return false;
		}

	}
	void analyseRects() {
		
		for (int y = 0; y < constraint.h ; y++) {
			
			for (int x = 0 ; x < constraint.w ; x++) {
				
				if (getPixel(dot, x, y) != dot->format->colorkey) {
					SDL_Rect r;
					r.x = x;
					r.y = y;
					r.h = 1;
					r.w = 1;
					while ( ( (getPixel(dot, x-1 , y+1) == dot->format->colorkey && getPixel(dot, x, y+1) != dot->format->colorkey ) || x == 0 && getPixel(dot, x, y+1) != dot->format->colorkey) && y+1 < constraint.h) {
						r.h++;
						y++;
					}
					
					x++;
					
					while (getPixel(dot, x, y) != dot->format->colorkey && x < constraint.w) {
						r.w++;
						x++;
					}
					
					x = 0;
					rects.push_back(r);
					break;
				}
				
			}
		}
		
		
	
	}
	void analyseRects2() {
		
		SDL_Rect rec;
		
		for (int y = 0 ; y < constraint.h ; y++) {
			
			for (int x = 0 ; x < constraint.w ; x++) {
				
				while (getPixel(dot, x, y) == dot->format->colorkey && !outofboundry(dot, x, y) ) {
					x++;
				}
				
				if (isRect(rects, x, y)) {
					continue;
				}
				
				if (outofboundry(dot, x, y) ) {
					break;
				}
				
				rec.x = x;
				rec.y = y;
				rec.w = 0;
				rec.h = 1;
				
				while (getPixel(dot, x, y) != dot->format->colorkey && !outofboundry(dot, x, y) ) {
					x++;
					rec.w++;
				}
				
				while ( y+1 < constraint.h && (getPixel(dot, rec.x-1, y+1) == dot->format->colorkey || rec.x == 0) 
					   && ( getPixel(dot, rec.x, y+1) != dot->format->colorkey)  
					   && (getPixel(dot, x, y+1) == dot->format->colorkey || x == constraint.w) 
					   && getPixel(dot, x-1, y+1) != dot->format->colorkey  ) {
						   
						   rec.h++;
						   y++;
				}
				
				y = rec.y;
				rects.push_back(rec);
				
			}
		}
	}
	
	
public:
	Dot(float vel, float x, float y, string file) 
	: velocity(vel), velX(0), velY(0), dotfile( file.c_str() ), distX(0), distY(0), speedSense(50)
	{
		dot = loadImage(dotfile);
		SDL_SetColorKey(dot, SDL_SRCCOLORKEY | SDL_RLEACCEL, getPixel(dot, 0, 0) );
		constraint.w = dot->w;
		constraint.h = dot->h;
		constraint.x = x - dot->w/2;
		constraint.y = y - dot->h/2;
		velocity == 0 ? stoped = true : stoped = false;
		keystate = SDL_GetKeyState(NULL);
		analyseRects2();
		if (rects.size() > 0) {
			rectsOfsetX = rects[0].x - constraint.x;
			rectsOfsetY = rects[0].y - constraint.y;
		}
	}
	
	void handleKeyDown(SDL_Event* event ) {
			switch (event->key.keysym.sym) {
				case SDLK_UP:
					velY -= velocity;
					break;
				case SDLK_DOWN:
					velY += velocity; 
					break;
				case SDLK_RIGHT:
					velX += velocity; 
					break;
				case SDLK_LEFT:
					velX -= velocity;
					break;
				case SDLK_w:
					if (velocity == 0) {
						alterVel(speedSense);
						if (keystate[SDLK_UP] ) {
							velY -= velocity;
						}
						if (keystate[SDLK_DOWN]) {
							velY += velocity;
						}
						if (keystate[SDLK_RIGHT]) {
							velX += velocity;
						}
						if (keystate[SDLK_LEFT]) {
							velX -= velocity;
						}
					}
					else {
						alterVel(speedSense);
					}
					break;
				case SDLK_s:
					alterVel(-speedSense);
					break;
				default:
					break; 
			}
		}
		
	void handleKeyUp(SDL_Event* event) {
		
		switch (event->key.keysym.sym) {
			case SDLK_UP:
				velY += velocity;
				break;
			case SDLK_DOWN:
				velY -= velocity; // ?????
				break;
			case SDLK_RIGHT:
				velX -= velocity; // ?????
				break;
			case SDLK_LEFT:
				velX += velocity;
				break;
			default:
				break;
		}
	}

	void move(Uint32 deltaTicks) {
		
		float Tx = velX*deltaTicks/1000.f;
		float Ty = velY*deltaTicks/1000.f;
		
		// !!!!!!!!!!!!!!!
		Sint16 cX = constraint.x;
		Sint16 cY = constraint.y;
		
		
		if (velX != 0 && velY != 0) {
			constraint.x += Sint16(Tx/sqrt(2) + distX );
			constraint.y += Sint16(Ty/sqrt(2) + distY );
			
			if (cX == constraint.x) {
				distX += Tx/sqrt(2);
			}
			else {
				distX = 0;
			}
								
			if (cY == constraint.y) {
				distY += Ty/sqrt(2) ;
			}
			else {
				distY = 0;
			}
			
		}
		else {
			constraint.x += Sint16(Tx + distX);
			constraint.y += Sint16(Ty + distY);
			
			if (cX == constraint.x) {
				distX += Tx;
			}
			else {
				distX = 0;
			}
			
			if (cY == constraint.y) {
				distY += Ty;
			}
			else {
				distY = 0;
			}
		}
		
		if (constraint.x <= 0) {
			constraint.x = SCREEN_WIDTH + constraint.x;
		}
		else if (constraint.x > SCREEN_WIDTH) {
			constraint.x = constraint.x - SCREEN_WIDTH;
			
		}
		
		if (constraint.y <= 0) {
			constraint.y = SCREEN_HEIGHT + constraint.y;
		}
		else if (constraint.y > SCREEN_HEIGHT) {
			constraint.y = constraint.y - SCREEN_HEIGHT;
		}
		
	}
	
	void show(SDL_Surface* surf) {
		
		if (constraint.x + constraint.w > SCREEN_WIDTH) {
			apply_surface( constraint.x - SCREEN_WIDTH, constraint.y, dot, surf);
		}
		
		if (constraint.y + constraint.h > SCREEN_HEIGHT) {
			apply_surface( constraint.x, constraint.y - SCREEN_HEIGHT, dot, surf);
		}
		
		if (constraint.y + constraint.h > SCREEN_HEIGHT && constraint.x + constraint.w > SCREEN_WIDTH) {
			apply_surface( constraint.x - SCREEN_WIDTH, constraint.y - SCREEN_HEIGHT, dot, surf);
		}
		
		
		
		apply_surface( constraint.x, constraint.y, dot, surf);
		
	}
	
	void setVel(float vel) {
		
		velocity = vel;
		
		if (velX < 0) {
			velX = -vel;
		}
		if (velX > 0) {
			velX = vel;
		}
		if (velY < 0) {
			velY = -vel;
		}
		if (velY > 0) {
			velY = vel;
		}
	}
	
	void alterVel(float vel) {
		
		if (velocity + vel < 0) {
			velocity = 0;
			velX = 0;
			velY = 0;
		}
		else {
			velocity += vel;
		
			if (velX < 0) {
				velX -= vel;
			}
			if (velX > 0) {
				velX += vel;
			}
			if (velY < 0) {
				velY -= vel;
			}
			if (velY > 0) {
				velY += vel;
			}
		}
	}
	
	void setSpeedSense(float f) {
		speedSense = f;
	}
	
	void movePixle (Uint8 dir) {
		switch (dir) {
			case 1:
				constraint.y--;
				break;
			case 2:
				constraint.x++;
				break;
			case 3:
				constraint.y++;
				break;
			case 4:
				constraint.x--;
				break;
			default:
				cerr<< "undefined int to movePixle";
				break;
		}
	}
	
	void collision(SDL_Rect boxB) {
				
		Uint8 col = check_collision(constraint, boxB);

		while (col != 0 ) {
			
			if (velY != 0 && velX != 0) {
				
			
				if (col == 1 ) {
					movePixle(1);
				}
				if (col == 2 ) {
					movePixle(2);
				}
				if (col == 3 ) {
					movePixle(3);
				}
				if (col == 4 ) {
					movePixle(4);
				}
			}
			else if ( (velY != 0 && velX == 0) || (velY == 0 && velX != 0) ) {
				
				if (velY > 0 ) {
					movePixle(1);
				}
				if (velX < 0 )  {
					movePixle(2);
				}
				if (velY < 0 )  {
					movePixle(3);
				}
				if (velX > 0 )  {
					movePixle(4);
				}
			
			}
			
			else {
				
				int dleft = constraint.x + constraint.w/2; - boxB.x;
				int dright = boxB.x + boxB.w - constraint.x + constraint.w/2;
				int dtop = constraint.y + constraint.h/2 - boxB.y;
				int dbottom = boxB.y + boxB.h - constraint.y + constraint.h/2;
				
				if (dleft < dright && dleft < dtop && dleft < dbottom) {
					constraint.x -= dleft + constraint.w/2;
				}
				if (dright < dleft && dright < dbottom && dright < dtop) {
					constraint.x += dright + constraint.w/2;
				}
				if (dbottom < dtop && dbottom < dright && dbottom < dleft) {
					constraint.y += dbottom + constraint.h/2;
				}
				if (dtop < dbottom && dtop < dright && dtop < dleft) {
					constraint.y -= dtop + constraint.h/2;
				}
			}
			
			col = check_collision(constraint, boxB);
		}
	}
	
	void pixle_collision(SDL_Rect boxB) {
		
		SDL_Rect rec;
		
		for (int i = 0; i < rects.size() ; i++) {
			
			rec = rects[i];
			rec.x += constraint.x;
			rec.y += constraint.y;
			
			Uint8 col = check_collision(rec, boxB);
			
			while (col != 0 ) {
				
				cout << endl << i << "  " << int(col) << endl;
				
				if (velY != 0 && velX != 0) {
					
					
					if (col == 1 ) {
						movePixle(1);
					}
					if (col == 2 ) {
						movePixle(2);
					}
					if (col == 3 ) {
						movePixle(3);
					}
					if (col == 4 ) {
						movePixle(4);
					}
				}
				else if ( (velY != 0 && velX == 0) || (velY == 0 && velX != 0) ) {
					
					if (velY > 0 ) {
						movePixle(1);
					}
					if (velX < 0 )  {
						movePixle(2);
					}
					if (velY < 0 )  {
						movePixle(3);
					}
					if (velX > 0 )  {
						movePixle(4);
					}
					
				}
				
				else {
					
					int dleft = constraint.x + constraint.w/2; - boxB.x;
					int dright = boxB.x + boxB.w - constraint.x + constraint.w/2;
					int dtop = constraint.y + constraint.h/2 - boxB.y;
					int dbottom = boxB.y + boxB.h - constraint.y + constraint.h/2;
					
					if (dleft < dright && dleft < dtop && dleft < dbottom) {
						constraint.x -= dleft + constraint.w/2;
					}
					if (dright < dleft && dright < dbottom && dright < dtop) {
						constraint.x += dright + constraint.w/2;
					}
					if (dbottom < dtop && dbottom < dright && dbottom < dleft) {
						constraint.y += dbottom + constraint.h/2;
					}
					if (dtop < dbottom && dtop < dright && dtop < dleft) {
						constraint.y -= dtop + constraint.h/2;
					}
				}
				
				
				rec = rects[i];
				rec.x += constraint.x;
				rec.y += constraint.y;
				col = check_collision(rec, boxB);
			}
			
		}
	
	
	}
	
	void rectsInfo() {
		cout << rects.size() << endl << endl;
		cout << constraint.x << endl << constraint.y << endl <<constraint.w << endl <<constraint.h << endl << endl;
		for (int i = 0; i < rects.size(); i++) {
			cout << rects[i].x << endl <<rects[i].y << endl <<rects[i].w << endl <<rects[i].h << endl << endl;
		}
		
		
	
	}
	
	void showRects(SDL_Surface* surf, Uint32 c) {
		for (int i = 0 ; i < rects.size() ; i++) {
			plotRect(rects[i], c, surf);
		}
	
	}
	
	
};

/*
// class Textprinter {};
// class Line {};
// class Rectangle {};
// class Circle {};
// class Triangle {};
// class Functionprinter {};
*/

void error(LogFile* logf, string str) {
	logf->out("error: " + str);
	exit(1);
}
SDL_Surface* initialize(int w, int h, string name) {
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) return NULL;
	SDL_WM_SetCaption(name.c_str(), NULL);
	return SDL_SetVideoMode(w, h, SCREEN_BPP, SDL_SWSURFACE );
}
SDL_Surface* loadImage(string file) {
	SDL_Surface* img = NULL;
	SDL_Surface* imgOpt = NULL;
	
	img = IMG_Load(file.c_str() );
	
	if (img != NULL) {
		imgOpt = SDL_DisplayFormat(img);
		SDL_FreeSurface(img);
		if (imgOpt == NULL) return NULL;
		else return imgOpt;
	}
	SDL_FreeSurface(img);
	SDL_FreeSurface(imgOpt);
	return NULL;
}
SDL_Surface* loadImage(string file, int r, int g, int b) {
	SDL_Surface* img = NULL;
	SDL_Surface* imgOpt = NULL;
	
	img = IMG_Load(file.c_str() );
	
	if (img != NULL) {
		imgOpt = SDL_DisplayFormat(img);
		SDL_FreeSurface(img);
		if (imgOpt != NULL) {
			Uint32 colorkey = SDL_MapRGB(imgOpt->format, r, g, b);
			SDL_SetColorKey(imgOpt, SDL_SRCCOLORKEY, colorkey);
			return imgOpt;
		}
	}
	return NULL;
}
void apply_surface(int x, int y, SDL_Surface* src, SDL_Surface* des) {
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	
	SDL_BlitSurface(src, NULL, des, &offset);
}
void apply_surface(int x, int y, SDL_Surface* src, SDL_Surface* des, SDL_Rect* clip) {
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	
	SDL_BlitSurface(src, clip, des, &offset);
}
void output_text(int x, int y, string text, TTF_Font* font, SDL_Color color, SDL_Surface* des) {
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	
	SDL_Surface* message = NULL;
	message = TTF_RenderText_Solid(font, text.c_str() , color);
	
	
	
	if (message != NULL) {
		SDL_BlitSurface(message, NULL, des, &offset);
	}
	
	SDL_FreeSurface(message);
}
template <class num> string toString(num i) {
	stringstream ss;
	ss << i;
	return ss.str();
}
string getSDLvideoinfo() {
	string str = "Video information: \n\n";
	str += ( "\thw_available: " + toString( SDL_GetVideoInfo()->hw_available ) + "\n" );
	str += ( "\twm_available: " + toString( SDL_GetVideoInfo()->wm_available ) + "\n" );
	str += ( "\tblit_hw: " + toString( SDL_GetVideoInfo()->blit_hw ) + "\n" );
	str += ( "\tblit_hw_CC: " + toString( SDL_GetVideoInfo()->blit_hw_CC ) + "\n" );
	str += ( "\tblit_hw_A: " + toString( SDL_GetVideoInfo()->blit_hw_A ) + "\n" );
	str += ( "\tblit_sw: " + toString( SDL_GetVideoInfo()->blit_sw ) + "\n" );
	str += ( "\tblit_sw_CC: " + toString( SDL_GetVideoInfo()->blit_sw_CC ) + "\n" );
	str += ( "\tblit_sw_A: " + toString( SDL_GetVideoInfo()->blit_sw_A ) + "\n" );
	str += ( "\tblit_fill: " + toString( SDL_GetVideoInfo()->blit_fill) + "\n" );
	str += ( "\tvideo_mem: " + toString( SDL_GetVideoInfo()->video_mem) + "\n" );
	str += ( "\tBits per pixel: " + toString( (int) SDL_GetVideoInfo()->vfmt->BitsPerPixel) + "\n" );
	str += "----------------------------------\n\n\n";
	
	return str;
}
void drawPixel(SDL_Surface* screen, int x, int y, Uint32 color) {
	Uint32 pixel = color;
	int BPP = screen->format->BytesPerPixel; //Bytes per pixel
	Uint8 *pp = (Uint8*)screen->pixels + y*screen->pitch + x*BPP; //pointer to the pixel
	
	switch (BPP) {
		case 1:
			*pp = pixel;
			break;
		case 2:
			*(Uint16*)pp = pixel;
			break;
		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				pp[0] = (pixel >> 16) & 0xff;
				pp[1] = (pixel >> 8) & 0xff;
				pp[3] = pixel & 0xff;
			}
			else {
				pp[0] = pixel & 0xff;
				pp[1] = (pixel >> 8) & 0xff;
				pp[2] = (pixel >> 16) & 0xff;
			}
			break;
		case 4:
			*(Uint32*)pp = pixel;
			break;
		default:
			return;
			break;
	}


}
Uint32 getPixel(SDL_Surface* screen, int x, int y) {
	int BPP = screen->format->BytesPerPixel; 
	Uint8* pp = (Uint8*)screen->pixels + y*screen->pitch + x*BPP;
	
	switch(BPP) {
		case 1:
			return *pp;
			break;
		case 2:
			return *(Uint16*)pp;
			break;
		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				return pp[0] << 16 | pp[1] << 8 | pp[2];
			}
			else {
				return pp[0] | pp[1] << 8 | pp[2] << 16;
			}
			break;
		case 4:
			return *(Uint32*)pp;
			break;
		default:
			return 0;
			break;
				
	}
}
void drawLine(SDL_Surface* screen, SDL_Rect A, SDL_Rect B, Uint32 color) {
	double k = double(A.y - B.y) / double(A.x - B.x);
	double m = B.y - B.x * k;
	int ly = (A.y <= B.y ? A.y:B.y); //lŠgsta y
	int hy = (A.y <= B.y ? B.y:A.y); //hšgsta y
	int lx = (A.x <= B.y ? B.y:A.y); //lŠgsta x
	int hx = (A.y <= B.y ? A.y:B.y); //lŠgsta y

	if (lx != hx) {
		for (int x = lx ; x <= hx ; x++) {
			double dy = k*x+m;
			int iy = int(dy);
			iy = ( (dy-iy) >= 0.5 ? iy+1:iy);

			iy = (iy > hy ? hy:iy);
			for (int i = ly; i < iy ; i++) {
				drawPixel(screen, x, i, color);
			}
			ly = iy;
		}
	}
	else {
		for (int y = ly; y <= hy; y++) {
			drawPixel(screen, A.x, y, color);
		}
	}

}
SDL_Surface* copySurface(SDL_Surface* screen) {
	return SDL_CreateRGBSurface(SDL_SWSURFACE, screen->w, screen->h, 
	screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Bmask, 
	screen->format->Gmask, screen->format->Amask);
}
TTF_Font* Initfont(string file, int fontsize) {
	if ( TTF_Init() == -1 ) return NULL;
	return TTF_OpenFont(file.c_str(), fontsize);
}
Uint8 check_collision(SDL_Rect boxA, SDL_Rect boxB) {
	
	int leftA = boxA.x;
	int rightA = boxA.x + boxA.w;
	int upperA = boxA.y;
	int bottomA = boxA.y + boxA.h;
	
	int leftB = boxB.x;
	int rightB = boxB.x + boxB.w;
	int upperB = boxB.y;
	int bottomB = boxB.y + boxB.h;
	
	if (leftA >= rightB || rightA <= leftB || upperA >= bottomB || bottomA <= upperB ) {
		return 0;
	}
	else {
		if (bottomA-upperB <= bottomB-upperA && bottomA-upperB <= rightB-leftA && bottomA-upperB <= rightA-leftB ) {
			return 1;
		}
		if (rightB-leftA <= rightA-leftB && rightB-leftA <= bottomA-upperB && rightB-leftA <= bottomB-upperA ) {
			return 2;
		}
		if (bottomB-upperA <= bottomA-upperB && bottomB-upperA <= rightB-leftA && bottomB-upperA <= rightA-leftB) {
			return 3;
		}
		if (rightA-leftB <= rightB-leftA && rightA-leftB <= bottomA-upperB && rightA-leftB <= bottomB-upperA) {
			return 4;
		}
		
		return 1;
	}
}
void plotRect(SDL_Rect r, Uint32 c, SDL_Surface* surf) {
	if (r.x + r.w >= surf->w || r.y + r.h >= surf->h || r.x < 0 || r.y < 0) {
		return;
	}
	else {
		for (int y = r.y ; y < r.y + r.h; y++) {
			for (int x = r.x ; x < r.x + r.w ; x++) {
				drawPixel(surf, x, y, c);
			}
		}
	}
} 


//main function
int main(int argc, char* args[] ) {
	
LogFile logf(LOG_FILE);	//log file
	
	

FpsRegulator fps_reg(FRAMES_PER_SEC);
	double fps_str[2] = {0, 0};
	
SDL_Surface* screen = NULL; //main screen
SDL_Surface* screenbuff = NULL;
SDL_Surface* background = NULL;
	
SDL_Event event; //event handler
bool quit = false;	//quit flag for the main loop	

screen = initialize(SCREEN_WIDTH, SCREEN_HEIGHT, APP_NAME); //initialize sdl and make screen
atexit(SDL_Quit); //so SDL_Quit() is callet at exit
screenbuff = copySurface(screen);
	

TTF_Font* font = NULL;

Uint32 color = SDL_MapRGB(screen->format, 0xFF, 0xAA, 0xAA);
background = copySurface(screen);
SDL_FillRect(background, NULL, color);
	
	SDL_Rect wall; wall.x = 400; wall.y = 40; wall.w = 40; wall.h = 400;
	Dot square(200, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, "blob.bmp" );
	square.showRects(background, SDL_MapRGB(screen->format, 0, 0, 0));
	square.setSpeedSense(100);
	Timer time;
	time.start();
	
//main loop
while (!quit) {
	while (SDL_PollEvent(&event)) {	 //event loop
	
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				quit = true;
			}
			square.handleKeyDown(&event);
		}
		
		if (event.type == SDL_KEYUP) {
			square.handleKeyUp(&event);
		}
		
		if (event.type == SDL_QUIT ) quit = true;
		
	}
	
	square.move(time.getTicks());
	time.start();
	square.collision(wall);
	SDL_BlitSurface(background, NULL, screenbuff, NULL);
	SDL_FillRect(background, &wall, SDL_MapRGB(screen->format, 0x77, 0x77, 0x77) );
	square.show(screenbuff);
	SDL_BlitSurface(screenbuff, NULL, screen, NULL);
	SDL_Flip(screen);
	
	fps_reg.getFps(1000, fps_str);
	if (fps_str[0] != fps_str[1]) {
		cout << fps_str[0] << " fps" << endl;
		fps_str[1] = fps_str[0];
	}
	
	fps_reg.regulateFps();
}
	square.rectsInfo();
SDL_FreeSurface(background);
SDL_FreeSurface(screenbuff);

TTF_CloseFont(font); 
TTF_Quit();


exit(0); //exit and shut down everything
}