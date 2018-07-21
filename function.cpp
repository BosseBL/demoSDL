/*
 *  function.cpp
 *  sdl
 *
 *  Created by Daniel Lindfors on 5/10/12.
 *  Copyright 2012 KTH. All rights reserved.
 *
 */

#include "function.h"
#include "class.h"


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
SDL_Surface* loadImage(string file, Uint32 colorkey) {
	SDL_Surface* img = NULL;
	SDL_Surface* imgOpt = NULL;
	
	img = IMG_Load(file.c_str() );
	
	if (img != NULL) {
		SDL_SetColorKey(imgOpt, SDL_SRCCOLORKEY, colorkey);
		imgOpt = SDL_DisplayFormat(img);
		SDL_FreeSurface(img);
		if (imgOpt != NULL) {
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
	int ly = (A.y <= B.y ? A.y:B.y); //lägsta y
	int hy = (A.y <= B.y ? B.y:A.y); //högsta y
	int lx = (A.x <= B.y ? B.y:A.y); //lägsta x
	int hx = (A.y <= B.y ? A.y:B.y); //lägsta y
	
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
double pointDistance(Point p1, Point p2) {
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}
