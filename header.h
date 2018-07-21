/*
 *  header.h
 *  sdl
 *
 *  Created by Daniel Lindfors on 5/10/12.
 *  Copyright 2012 KTH. All rights reserved.
 *
 */

#ifndef HEADER_H
#define HEADER_H


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL_image/SDL_image.h>
#include <iostream>
#include <fstream>
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

#define UPP 1
#define	RIGHT 2
#define DOWN 3
#define LEFT 4

#define RECTANGULAR_COLLISION 1
#define PIXLE_COLLISION 2
#define CIRCULAR_COLLISION 3

#define FRAMES_PER_SEC 100

#define APP_NAME "Move the dot"

#define BACKGROUND_FILE "background.png"
#define FONT_FILE "lazy.ttf"
#define LOG_FILE "log.txt"

using namespace std;

#endif
