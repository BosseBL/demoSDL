#include "header.h"
#include "class.h"
#include "function.h"
#include "animation.h"

//main function
int main(int argc, char* args[] ) {
    
	Logfile logf(LOG_FILE);	//log file
		
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

	background = loadImage("/Users/daniellindfors/Documents/Xcode projects/sdl/build/Debug/bg.png");
    if(background == NULL) {
        cout<<"image not loaded\n";
        exit(1);
    }
	screenbuff = copySurface(background);
	camera cam(screen, screenbuff);
	SDL_Rect wall; wall.x = 400; wall.y = 40; wall.w = 40; wall.h = 400;
	MovingScreenObject dot(5, screenbuff->w/2, screenbuff->h/2, loadImage("/Users/daniellindfors/Documents/Xcode projects/sdl/build/Debug/dot.bmp") , screenbuff);
	Timer time;
	time.start();
		
	//main loop
	while (!quit) {
		while (SDL_PollEvent(&event)) {	 //event loop
		
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					quit = true;
				}
				
				dot.handleKeyDown(&event);
				

			}
			else if (event.type == SDL_KEYUP) {
				dot.handleKeyUp(&event);
			}
			
			if (event.type == SDL_QUIT ) quit = true;
			
		}
		
		dot.move();
		time.start();
		dot.show();
		cam.centerOver(dot.getOrgentation());
		cam.show();
		SDL_Flip(screen);
		SDL_BlitSurface(background, NULL, screenbuff, NULL);
		fps_reg.getFps(1000, fps_str);
		if (fps_str[0] != fps_str[1]) {
			cout << fps_str[0] << " fps" << endl;
			fps_str[1] = fps_str[0];
		}
		
		fps_reg.regulateFps();
	}
	SDL_FreeSurface(background);
	SDL_FreeSurface(screenbuff);

	TTF_CloseFont(font); 
	TTF_Quit();


	exit(0); //exit and shut down everything
}