/*
 *  animation.h
 *  sdl
 *
 *  Created by Daniel Lindfors on 5/20/12.
 *  Copyright 2012 KTH. All rights reserved.
 *
 */
#ifndef ANIMATION_H
#define ANIMATION_H

#include "header.h"

class Animation {
private:
	int ofset;
	int velocity;
	int frame;
	int status; // 1 = left, 2 = right
	SDL_Surface* image;
	SDL_Surface* screen;
	vector<SDL_Rect> clipsRight;
	vector<SDL_Rect> clipsLeft;
	int clip_width;
	int clip_height;
	int frameshow;
	
	
public:
	Animation(SDL_Surface* surf, SDL_Surface* scr) 
	:ofset(0), velocity(0), frame(0), status(2), frameshow(0) {
		image = surf;
		screen = scr;
		
		clip_width = image->w/4;
		clip_height = image->h/2;
		
		clipsLeft.resize(4);
		clipsRight.resize(4);
		
		clipsRight[0].x = 0;
		clipsRight[0].y = 0;
		clipsRight[0].w = clip_width;
		clipsRight[0].h = clip_height;
		
		clipsRight[1].x = clip_width;
		clipsRight[1].y = 0;
		clipsRight[1].w = clip_width;
		clipsRight[1].h = clip_height;
		
		clipsRight[2].x = clip_width*2;
		clipsRight[2].y = 0;
		clipsRight[2].w = clip_width;
		clipsRight[2].h = clip_height;
		
		clipsRight[3].x = clip_width*3;
		clipsRight[3].y = 0;
		clipsRight[3].w = clip_width;
		clipsRight[3].h = clip_height;
		
		clipsLeft[0].x = 0;
		clipsLeft[0].y = clip_height;
		clipsLeft[0].w = clip_width;
		clipsLeft[0].h = clip_height;
		
		clipsLeft[1].x = clip_width;
		clipsLeft[1].y = clip_height;
		clipsLeft[1].w = clip_width;
		clipsLeft[1].h = clip_height;
		
		clipsLeft[2].x = clip_width*2;
		clipsLeft[2].y = clip_height;
		clipsLeft[2].w = clip_width;
		clipsLeft[2].h = clip_height;
		
		clipsLeft[3].x = clip_width*3;
		clipsLeft[3].y = clip_height;
		clipsLeft[3].w = clip_width;
		clipsLeft[3].h = clip_height;
		
		SDL_SetColorKey(image, SDL_SRCCOLORKEY | SDL_RLEACCEL, getPixel(image, 0, 0));
	}
	void handle_events(SDL_Event &event) {
		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
				case SDLK_RIGHT:
					velocity += 5;
					break;
				case SDLK_LEFT:
					velocity -= 5;
					break;
				default:
					break;
			}
		}
		if (event.type == SDL_KEYUP) {
			switch (event.key.keysym.sym) {
				case SDLK_RIGHT:
					velocity -= 5;
					break;
				case SDLK_LEFT:
					velocity += 5;
					break;
				default:
					break;
			}
		}
	}
	void move() {
		ofset += velocity;
		
		if (ofset < 0 || ofset + clip_width > screen->w) {
			ofset -= velocity;
		}
	
	}
	void show() {
		if (velocity < 0) {
			status = 1;
			frame++;
		}
		else if (velocity > 0) {
			status = 2;
			frame++;
		}
		else {
			frame = 0;
		}
		
		if (frame > 15) {
			frame = 0;
		}
		
		frameshow = int(frame/5);
		
		if (status == 2) {
			apply_surface(ofset, screen->h-clip_height, image, screen, &clipsRight[frameshow]);
		}
		if (status == 1) {
			apply_surface(ofset, screen->h-clip_height, image, screen, &clipsLeft[frameshow]);
		}
		
		
	
	}


};

#endif