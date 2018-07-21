/*
 *  class.cpp
 *  sdl
 *
 *  Created by Daniel Lindfors on 5/10/12.
 *  Copyright 2012 KTH. All rights reserved.
 *
 */

#include "class.h"
#include "function.h"


	Logfile::Logfile(string fileN) {
		filename = fileN;
	}
	
	void Logfile::out(string str) {
		logf.open(filename.c_str(), ios_base::app | ios_base::out);
		logf << str << endl;
		logf.close();
	}
	
	void Logfile::out(int i) {
		logf.open(filename.c_str(), ios_base::app | ios_base::out);
		logf << i << endl;
		logf.close();
	}
	
	void Logfile::setFile(string fileN) {
		filename = fileN;
	}
	
	

	Button::Button(int x, int y, int w, int h) {		//contruct button
		
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
	
	void Button:: handle_events(SDL_Event* event) {			//handle events and set image
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
	
	void Button:: show(SDL_Surface* src, SDL_Surface* des) {		//show image
		apply_surface(box.x, box.y , src, des, &clip[mouse]);
	}
	

	Timer::Timer() 
	:start_ticks(0), time(0), timerstate(0)
	{}void Timer:: start() {
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
	
	void Timer::stop() {
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
	
	void Timer::pause() {
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
	
	Uint32 Timer::getTicks() {
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
	
	bool Timer::isPaused() {
		if (timerstate == 1) {
			return true;
		}
		else {
			return false;
		}
	}
	
	bool Timer::isRunning() {
		if (timerstate == 2) {
			return true;
		}
		else {
			return false;
		}
	}
	
	bool Timer::isStoped() {
		if (timerstate == 0) {
			return true;
		}
		else {
			return false;
		}
	}
	

	FpsRegulator::FpsRegulator(Uint32 fps) 
	: framecount(0), time_sum(0)
	{
		setFps(fps); //sets msec_per_frame;
	}
	
	void FpsRegulator::setFps(Uint32 fps) { 
		msec_per_frame = 1000/fps; 
	}
	
	void FpsRegulator::reset() {
		time.stop();
		time.start();
		framecount = 0;
		time_sum = 0;
	}
	
	void FpsRegulator::stop() {
		time.stop();
		framecount = 0;
		time_sum = 0;
	}
	
	void FpsRegulator::getFps(Uint32 interval, double* fps) {
		
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
	
	void FpsRegulator::regulateFps() {
		
		if ( time.isRunning() ) {
			Uint32 ms = time.getTicks();
			if (ms < msec_per_frame) {
				SDL_Delay( msec_per_frame - ms );
			}
		}
		
		time.start();
	}
	

	Repport::Repport(int intervals)
	:intervals(intervals), counter(0)
	{}
	
	void Repport::repport(string message) {
		if ( counter >= intervals ) {
			cout << message;
			counter = 0;
		}
	}
	void Repport::repport(double message) {
		if ( counter >= intervals ) {
			cout << message;
			counter = 0;
		}
	}
	void Repport::repport(char message) {
		if ( counter >= intervals ) {
			cout << message;
			counter = 0;
		}
	}
	void Repport::repport(int message) {
		if ( counter >= intervals ) {
			cout << message;
			counter = 0;
		}
	}
	void Repport::count() {
		counter++;
	}
	

	MovingScreenObject::MovingScreenObject(float vel, float x, float y, SDL_Surface* objectimage, SDL_Surface* screen) 
: velocity(vel), velX(0), velY(0), image_surface(objectimage), distX(0), distY(0), speedSense(5), screen(screen), dXmoved(0), dYmoved(0)
	{
		SDL_SetColorKey(image_surface, SDL_SRCCOLORKEY | SDL_RLEACCEL, getPixel(image_surface, 0, 0) );
		orgentation.w = image_surface->w;
		orgentation.h = image_surface->h;
		orgentation.x = x - image_surface->w/2;
		orgentation.y = y - image_surface->h/2;
		keystate = SDL_GetKeyState(NULL);
		cout << 1;
		analyseRects();
		cout << 2;
		warp_rect = warp(orgentation);
		circular_constraint.x = orgentation.x + float(orgentation.w/2);
		circular_constraint.y = orgentation.y + float(orgentation.h/2);
		circular_constraint.r = float(orgentation.w/2);
	}
	
	void MovingScreenObject::move() {
		
		static const double root = sqrt(2);
		
		Sint16 cX = orgentation.x;
		Sint16 cY = orgentation.y;
		
		if (velX != 0 && velY != 0) {
			dXmoved = Sint16(velX/root + distX );
			dYmoved = Sint16(velY/root + distY );
			
			orgentation.x += dXmoved;
			orgentation.y += dYmoved;
			
			if (cX == orgentation.x) {
				distX += velX/root;
			}
			else {
				distX = 0;
			}
			
			if (cY == orgentation.y) {
				distY += velY/root;
			}
			else {
				distY = 0;
			}
			
		}
		else {
			dXmoved = Sint16(velX + distX);
			dYmoved = Sint16(velY + distY);
			
			orgentation.x += dXmoved;
			orgentation.y += dYmoved;
			
			if (cX == orgentation.x) {
				distX += velX;
			}
			else {
				distX = 0;
			}
			
			if (cY == orgentation.y) {
				distY += velY;
			}
			else {
				distY = 0;
			}
		}
		
		
		if (orgentation.x <= 0) {
			orgentation.x = screen->w + orgentation.x;
		}
		else if (orgentation.x > screen->w) {
			orgentation.x = orgentation.x - screen->w;
			
		}
		
		if (orgentation.y <= 0) {
			orgentation.y = screen->h + orgentation.y;
		}
		else if (orgentation.y > screen->h) {
			orgentation.y = orgentation.y - screen->h;
		}
		
		warp_rect = warp(orgentation);
		circular_constraint.x = orgentation.x + orgentation.w/2;
		circular_constraint.y = orgentation.y + orgentation.h/2;
		circular_constraint.r = orgentation.w/2;
	}
	
	void MovingScreenObject::show() {
		
		for (int i = 0; i < warp_rect.size() ; i++) {
			apply_surface(warp_rect[i].x, warp_rect[i].y, image_surface, screen);
		}
		
	}
	
	void MovingScreenObject::movePixle (Uint8 dir) {
		switch (dir) {
			case UPP:
				orgentation.y--;
				break;
			case RIGHT:
				orgentation.x++;
				break;
			case DOWN:
				orgentation.y++;
				break;
			case LEFT:
				orgentation.x--;
				break;
			default:
				cerr<< "undefined int to movePixle";
				break;
		}
		warp_rect = warp(orgentation);
	}
	
	void MovingScreenObject::collision(SDL_Rect boxB, Uint8 flag) {
		switch (flag) {
			case RECTANGULAR_COLLISION:
				for (int i = 0; i < warp_rect.size() ; i++) {
					rectangular_collision(warp_rect[i], boxB);
				}
				break;
			case PIXLE_COLLISION:
				SDL_Rect r;
				for (int i = 0; i < warp_rect.size() ; i++) {
					for (int b = 0; b < image_rect_composition.size() ; b++) {
						r.x = warp_rect[i].x + image_rect_composition[b].x;
						r.y = warp_rect[i].y + image_rect_composition[b].y;
						r.w = image_rect_composition[b].w;
						r.h = image_rect_composition[b].h;
						
						rectangular_collision(r, boxB);
					}
				}
				break;
			case CIRCULAR_COLLISION:
				if (circular_constraint.r == 0) {
					collision(boxB, 1);
				}
				else {
					circular_collision(boxB);
				}
				break;
				
			default:
				break;
		}
		
	}
	
	void MovingScreenObject::rectangular_collision(SDL_Rect boxA, SDL_Rect boxB) {
			
			int col = check_collision(boxA, boxB);
		
			if (col != 0 ) {
				
				if (velY != 0 && velX != 0) {
					
					
					if (col == 1 ) {
						orgentation.y -= boxA.y + boxA.h - boxB.y;
						for (int i = 0; i < warp_rect.size() ; i++) {
							warp_rect[i].y -= boxA.y + boxA.h - boxB.y;
						}
					}
					if (col == 2 ) {
						orgentation.x += boxB.x + boxB.w - boxA.x;
						for (int i = 0; i < warp_rect.size() ; i++) {
							warp_rect[i].x += boxB.x + boxB.w - boxA.x;
						}
					}
					if (col == 3 ) {
						orgentation.y += boxB.y + boxB.h - boxA.y;
						for (int i = 0; i < warp_rect.size() ; i++) {
							warp_rect[i].y += boxB.y + boxB.h - boxA.y;
						}					}
					if (col == 4 ) {
						orgentation.x -= boxA.x + boxA.w - boxB.x;
						for (int i = 0; i < warp_rect.size() ; i++) {
							warp_rect[i].x -= boxA.x + boxA.w - boxB.x;
						}							}
				}
				else if ( (velY != 0 && velX == 0) || (velY == 0 && velX != 0) ) {
					
					if (velY > 0 ) {
						orgentation.y -= boxA.y + boxA.h - boxB.y;
						for (int i = 0; i < warp_rect.size() ; i++) {
							warp_rect[i].y -= boxA.y + boxA.h - boxB.y;
						}							}
					if (velX < 0 )  {
						orgentation.x += boxB.x + boxB.w - boxA.x;
						for (int i = 0; i < warp_rect.size() ; i++) {
							warp_rect[i].x += boxB.x + boxB.w - boxA.x;
						}							}
					if (velY < 0 )  {
						orgentation.y += boxB.y + boxB.h - boxA.y;
						for (int i = 0; i < warp_rect.size() ; i++) {
							warp_rect[i].y += boxB.y + boxB.h - boxA.y;
						}							}
					if (velX > 0 )  {
						orgentation.x -= boxA.x + boxA.w - boxB.x;
						for (int i = 0; i < warp_rect.size() ; i++) {
							warp_rect[i].x -= boxA.x + boxA.w - boxB.x;
						}						
					}
					
				}
				
				else {
					
				}
				
			}
		}
	
	void MovingScreenObject::showRects(SDL_Surface* surf, Uint32 c1, Uint32 c2) {
		for (int i = 0 ; i < image_rect_composition.size() - 1 ; i+=2) {
			plotRect(image_rect_composition[i], c1, surf);
			plotRect(image_rect_composition[i+1], c2, surf);
		}
		
		if (image_rect_composition.size() % 2 != 0) {
			plotRect(image_rect_composition[image_rect_composition.size()-1], c1, surf);
		}
		
	}
	
	void MovingScreenObject::handleKeyDown(SDL_Event* event ) {
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
	
    SDL_Rect MovingScreenObject::getOrgentation(){
        return orgentation;
    }


	void MovingScreenObject::handleKeyUp(SDL_Event* event) {
		
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
	
	void MovingScreenObject::setVel(float vel) {
		
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
	
	void MovingScreenObject::alterVel(float vel) {
		
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
	
	void MovingScreenObject::setSpeedSense(float f) {
		speedSense = f;
	}
	
	void MovingScreenObject::rectsInfo() {
		cout << image_rect_composition.size() << endl << endl;
		cout << orgentation.x << endl << orgentation.y << endl <<orgentation.w << endl <<orgentation.h << endl << endl;
		for (int i = 0; i < image_rect_composition.size(); i++) {
			cout << image_rect_composition[i].x << endl <<image_rect_composition[i].y << endl <<image_rect_composition[i].w << endl <<image_rect_composition[i].h << endl << endl;
		}
		
		
		
	}
	
	bool MovingScreenObject::wholeRow(int w, int x, int y) {
		for (int i = 0; i < w; i++) {
			if (getPixel(image_surface, x+i, y) == image_surface->format->colorkey) {
				return false;
			}
		}
		return true;
	}

	bool MovingScreenObject::isRect(vector<SDL_Rect> rec, int x, int y) {
		for (int i = 0; i < rec.size() ; i++) {
			if (x >= rec[i].x && x < rec[i].x + rec[i].w  && y >= rec[i].y && y < rec[i].y + rec[i].h) {
				return true;
			}
		}
		return false;
	}

	bool MovingScreenObject::outofboundry(SDL_Surface* surface, int x, int y) {
		if (x < 0 && y < 0 && x >= surface->w && y >= surface->h) {
			return true;
		}
		else {
			return false;
		}

	}

	void MovingScreenObject::analyseRects() {
		
		SDL_Rect r;
		
		for (int y = 0; y < image_surface->h ; y++) {
			
			for (int x = 0 ; x < image_surface->w ; x++) {
				
				if (!isRect(image_rect_composition, x, y)) {
					
					if (getPixel(image_surface, x, y) != image_surface->format->colorkey) {
						r.h = 1;
						r.w = 1;
						x++;
						while (getPixel(image_surface, x, y) != image_surface->format->colorkey && x < image_surface->w ) {
							r.w++;
							x++;
						}
						while (y+1 < image_surface->h && wholeRow(r.w, x-r.w, y+1) 
							   && (getPixel(image_surface, x-r.w-1, y+1) == image_surface->format->colorkey || x-r.w-1 < 0)
							   && (getPixel(image_surface, x, y+1) == image_surface->format->colorkey || x >= image_surface->w ) )
						{
							r.h++;
							y++;
						}
					
						r.y = y - r.h + 1;
						r.x = x - r.w;
					
						image_rect_composition.push_back(r);
					
						y -= r.h - 1;
						
					}
				}	
				
			}
			
		}
	
	}

	vector<SDL_Rect> MovingScreenObject::warp(SDL_Rect constr) {
		SDL_Rect r = constr;
		vector<SDL_Rect> rv;
		rv.push_back(constr);
		
		if (constr.x + constr.w > screen->w) {
			r.x = constr.x - screen->w;
			rv.push_back(r);
		}
		
		if (constr.y + constr.w > screen->h) {
			r.x = constr.x; //återställer constr
			r.y = constr.y - screen->h;
			rv.push_back(r);
		}
		
		if (constr.x + constr.w > screen->w && constr.y + constr.w > screen->h) {
			r.x = constr.x - screen->w;
			rv.push_back(r);
		}
		
		return rv;
	
	}

	void MovingScreenObject::setCircularConstraint(int x, int y, int rad) {
		circular_constraint.x = x;
		circular_constraint.y = y;
		circular_constraint.r = rad;
	}

	void MovingScreenObject::circular_collision(SDL_Rect boxB) {
		Point nearestPoint;
		Point circ;
		
		circ.x = circular_constraint.x;
		circ.y = circular_constraint.y;
		
		if (circular_constraint.x < boxB.x) {
			
			nearestPoint.x = boxB.x;
			if (circular_constraint.y > boxB.y && circular_constraint.y <= boxB.y + boxB.h) {
				nearestPoint.y = circular_constraint.y;
			}
			else if (circular_constraint.y > boxB.y) {
				nearestPoint.y = boxB.y;
			}
			else {
				nearestPoint.y = boxB.y + boxB.w - 1;
			}
		}
		
		else if (circular_constraint.x >= boxB.x + boxB.w) {
			
			nearestPoint.x = boxB.x + boxB.w - 1;
			if (circular_constraint.y > boxB.y && circular_constraint.y <= boxB.y + boxB.h) {
				nearestPoint.y = circular_constraint.y;
			}
			else if (circular_constraint.y < boxB.y) {
				nearestPoint.y = boxB.y;
			
			}
			else {
				nearestPoint.y = boxB.y + boxB.h - 1;
			}
		}
		
		else {
			
			nearestPoint.x = circular_constraint.x;
			if (circular_constraint.y < boxB.y) {
				nearestPoint.y = boxB.y;
			}
			else {
				nearestPoint.y = boxB.y + boxB.h - 1;
			}
		}
		//använd x2+y2=r2 för att räkna ut distansen
		
		
				
		if (pointDistance(nearestPoint, circ ) < circular_constraint.r) {
			orgentation.x -= velX;
			orgentation.y -= velY;
			for (int i = 0; i < warp_rect.size() ; i++) {
				warp_rect[i].x -= velX;
				warp_rect[i].y -= velY;
			}
			
			
		}
	}

	void MovingScreenObject::circular_collision(Circle circleB) {

	}

	camera::camera(SDL_Surface* scr, SDL_Surface* pla) 
	:screen(scr), plane(pla) {
		orgentation.x = plane->w/2;
		orgentation.y = plane->h/2;
		orgentation.w = screen->w;
		orgentation.h = screen->h;
		halfW = orgentation.w/2;
		halfH = orgentation.h/2;
	}

void camera::show() {
	apply_surface(0, 0, plane, screen, &orgentation);
}

void camera::centerOver(SDL_Rect r) {
	if (r.x < 0 || r.x > plane->w || r.y < 0 || r.y > plane->h) {
		cout<< "error\n";
	}
	else {
		if ( (r.x < halfW || r.x > plane->w - halfW) && (r.y < halfH || r.y > plane->h - halfH) ) {
			
		}
		else if	(r.x < halfW || r.x > plane->w - halfW) {
			orgentation.y = r.y - orgentation.h/2;
		}
		else if (r.y < halfH || r.y > plane->h - halfH) {
			orgentation.x = r.x - orgentation.w/2;
		}
		else {
			orgentation.x = r.x - orgentation.w/2;
			orgentation.y = r.y - orgentation.h/2;
		}
	}
}



void NXT_Robot::setVelosity(float r, float l) {
    velL = l;
    velR = r;
}
void NXT_Robot::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
}
void NXT_Robot::setAngle(float a) {
    angle = a;
}
void NXT_Robot::setRadius(float r) {
    radius = r;
}

void NXT_Robot::initTime() {
    time = SDL_GetTicks();
}

void NXT_Robot::uppdate(int ms) {
    int dtime = ms - time;
    time = ms;
    float newAngle = (velR-velL)*dtime*1000/radius + angle;
    position.x = (radius*(velL+velR)*sin(newAngle))/((velR-velL)) + position.x;
    position.y = (radius*(velL+velR)*cos(newAngle))/((velR-velL)) + position.y;
    angle = newAngle;
    
}

/*
	då rectangular_collision flyttar warp_rects genom warp zonen så kan problem uppstå då 
	recrangular_collision inte skapa fler warp_rects om det skulle behövas.
 
	om bildfilerna inte ligger i samma mapp som sdl application så crashar 
 
	collision ska ta emot och jobba med en vector<SDL_Rect> iställer för bara en SDL_Rect
 
	effektivisera: referenser ska användas där det är möjligt
				   många funktioner ska bli private
				   funktionerna ska generaliseras så mycket som möjligt.
				   vad händer om ett objekt ska byta screen?
				   stänga av och sätta på warp, eller åtminstonne välja i början.
				   cirkulät kollision
				   crash säkra med catch block.
				   event hantering ska skötas utanför classen. föreslagsvis en klass med pointer till funktion som användaren sätter
				   lägg till ljud vid collision, släp och bakgrundmusik (ljud class)
				   MovingScreenObject ska kunna animeras
					collision class
					
 
 
 typedef
 enum
 function pointer
 functor
 template
 virtual
 friend
 callback function
 inherritance
 exception
 
 */
