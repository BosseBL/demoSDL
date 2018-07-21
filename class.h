/*
 *  class.h
 *  sdl
 *
 *  Created by Daniel Lindfors on 5/10/12.
 *  Copyright 2012 KTH. All rights reserved.
 *
 */

#ifndef CLASS_H
#define CLASS_H

#include "header.h"

struct Point {
	int x, y;
};
struct FloatPoint {
    float x, y;
};
struct Circle {
	float x, y;
	float r;
};
class Logfile {
private:
	ofstream logf;
	string filename;
public:
	Logfile(string fileN);
	
	void out(string str);
		
	void out(int i); 		
	void setFile(string fileN);		
};
class Button  {
			
	private:
		SDL_Rect box;		//button attributes
		SDL_Rect clip[4];		//clip from sprite sheet
		int mouse;
			
	public:
		Button(int x, int y, int w, int h);
		void handle_events(SDL_Event* event);			
		void show(SDL_Surface* src, SDL_Surface* des);			
		};
class Timer {
		private:
			Uint32 start_ticks;
			Uint32 time; 
			Uint8 timerstate; // 0 stoped, 1 paused, 2 running
			
		public:
			Timer() ;
			void start();	//also resets or resumes when timer is running or paused 		
			void stop();
			void pause();			
			Uint32 getTicks();			
			bool isPaused();			
			bool isRunning();
			bool isStoped();
		};
class FpsRegulator {
private:
			Timer time;
			Timer real_time;
			Uint32 msec_per_frame;
			int framecount;
			Uint32 time_sum;
public:
	FpsRegulator(Uint32 fps);
	void setFps(Uint32 fps);			
	void reset(); 			
	void stop(); 
	void getFps(Uint32 interval, double* fps);			
	void regulateFps();			
		};
class Repport {
			
		private:
			int intervals;
			int counter;
			
		public:
			Repport(int intervals);
			
			void repport(string message);
			void repport(double message);
			void repport(char message);
			void repport(int message);
			void count();
		};
class MovingScreenObject {
private:
	float velX, velY;
	float dXmoved, dYmoved;
	SDL_Rect orgentation;
	float velocity;
	SDL_Surface* screen;
	SDL_Surface* image_surface;
	float speedSense;
	float distX, distY;
	Uint8 *keystate;
	vector<SDL_Rect> image_rect_composition;
	vector<SDL_Rect> warp_rect;
	Circle circular_constraint;
	
	
	bool isRect(vector<SDL_Rect> rec, int x, int y);
	bool outofboundry(SDL_Surface* surface, int x, int y);
	void analyseRects();
	vector<SDL_Rect> warp(SDL_Rect constr);
	bool wholeRow(int w, int x, int y);
	void rectangular_collision(SDL_Rect boxA, SDL_Rect boxB);
	void circular_collision(SDL_Rect boxB);
	void circular_collision(Circle circleB);

	
	
public:
	MovingScreenObject(float vel, float x, float y, SDL_Surface*, SDL_Surface* );
	void move();			
	void show();
	void movePixle (Uint8 dir);
	void collision(SDL_Rect boxB, Uint8 flag);
	void showRects(SDL_Surface* surf, Uint32 c1, Uint32 c2);
	void handleKeyDown(SDL_Event* event );
	void handleKeyUp(SDL_Event* event);
	void setVel(float vel);
	void alterVel(float vel);
	void setSpeedSense(float f);
	void rectsInfo();
	void setCircularConstraint(int x, int y, int radius);
	SDL_Rect getOrgentation();

};
class InputHandler {
private:
	Uint8 *keystate;
	SDL_Event event;
	
public:
	InputHandler() 
	:keystate(SDL_GetKeyState(NULL))
	{}
};
class camera {
private:
	SDL_Rect orgentation;
	SDL_Surface* screen;
	SDL_Surface* plane;
	int halfW, halfH;
	
	
public:
	camera(SDL_Surface* scr, SDL_Surface* pla);
	
	void show();
	
	void centerOver(SDL_Rect r);
	
	
};

struct NXT_Robot {
    float radius, angle;
    struct FloatPoint position;
    float velR, velL;
    int time;
    
    void setVelosity(float r, float l);
    void setPosition(float x, float y);
    void setAngle(float a);
    void setRadius(float r);
    
    void initTime();
    
    void uppdate(int ms);
};



/*
 // class Textprinter {};
 // class Line {};
 // class Rectangle {};
 // class Circle {};
 // class Triangle {};
 // class Functionprinter {};
 */

#endif
