/*
 *  function.h
 *  sdl
 *
 *  Created by Daniel Lindfors on 5/10/12.
 *  Copyright 2012 KTH. All rights reserved.
 *
 */

#ifndef FUNCTION_H
#define FUNCTION_H

#include "header.h"
#include "class.h"




SDL_Surface* initialize(int w, int h, string name);
SDL_Surface* loadImage(string file);
SDL_Surface* loadImage(string file, Uint32 colorkey);
void apply_surface(int x, int y, SDL_Surface* src, SDL_Surface* des);
void apply_surface(int x, int y, SDL_Surface* src, SDL_Surface* des, SDL_Rect* clip);
void output_text(int x, int y, string text, TTF_Font* font, SDL_Color color, SDL_Surface* des);
template <class num> string toString(num i);
string getSDLvideoinfo();
void drawPixel(SDL_Surface* screen, int x, int y, Uint32 color);
Uint32 getPixel(SDL_Surface* screen, int x, int y);
void drawLine(SDL_Surface* screen, SDL_Rect A, SDL_Rect B, Uint32 color);
SDL_Surface* copySurface(SDL_Surface* screen);
TTF_Font* Initfont(string file, int fontsize);
Uint8 check_collision(SDL_Rect boxA, SDL_Rect boxB);
void plotRect(SDL_Rect r, Uint32 c, SDL_Surface* surf);
double pointDistance(Point p1, Point p2);
#endif
