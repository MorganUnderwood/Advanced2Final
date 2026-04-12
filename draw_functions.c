#include "draw_functions.h"

void CircleFunction(SDL_Renderer *render, int x, int y, int r,
                    SDL_Color color) {
  SDL_SetRenderDrawColor(render, color.r, color.g, color.b, 255);
  for (int i = -r; i <= r; i++) {
    for (int j = -r; j <= r; j++) {
      if (i * i + j * j <= r * r) {
        SDL_RenderDrawPoint(render, x + i, j + y);
      }
    }
  }
}

void clearPrev(SDL_Renderer *renderer, TTF_Font *font, const char *current,
               const char *future, const char *past, int textWidth) {
  int horizontalLines[3];
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  SDL_RenderDrawLine(renderer, drawWidth, 1, drawWidth, HEIGHT);

  for (int i = 0; i < 3; i++) {
	if (i==0) {
		horizontalLines[i] = HEIGHT / 4;
	}
	else {
		horizontalLines[i] = horizontalLines[1-i] + horizontalLines[0];

    SDL_RenderDrawLine(renderer, drawWidth, horizontalLines[i], WIDTH,
                       horizontalLines[i]);
  }
  CircleFunction(renderer, center[0], center[1], 25,
                 (SDL_Color){255, 255, 0, 255});
  RenderText(renderer, font, current, future, past, textWidth);

  return;
}

void planetDraw(SDL_Renderer *render, Planet p, int x, int y) {

  int cx = x + cos(p.angle) * p.distanceFromSun;
  int cy = y - sin(p.angle) * p.distanceFromSun;

  CircleFunction(render, cx, cy, p.size, p.color);
}

void planetUpdate(SDL_Renderer *renderer, Planet planets[], int planetCount,
                  long long day, int centerX, int centerY) {
  for (int i = 0; i < planetCount; i++) {
    double angularVel = (2.0 * PI) / planets[i].speed;
    double angleCalc = fmod(
        planets[i].baseAngle + day * angularVel * SPEED_MULTIPLIER, 2 * PI);
    if (angleCalc < 0) {
      angleCalc += 2.0 * PI;
    }
    planets[i].angle = angleCalc;

    planetDraw(renderer, planets[i], centerX, centerY);
  }
  return;
}

void SDLInitialize (const char* serial_port){  
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Init Failed: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

     SDL_Window *window = SDL_CreateWindow("Planetarium", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, WIDTH,
                                        HEIGHT, // Window width and height.
                                        SDL_WINDOW_FULLSCREEN);

    if (!window) {
        printf("Window Creation Failed: %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }


	  TTF_Font *font = TTF_OpenFont(MY_FONT, 24);
    if (TTF_Init() < 0) {
        printf("TTF Init Failed: %s\n", TTF_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

 
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (!renderer) {
        printf("Renderer Creation Failed: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    
    inputsMTX = SDL_CreateMutex();
    if (!inputsMTX) {
        printf("Mutex Creation Failed: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        TTF_Quit();
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

  
    SDL_Thread *serialID = SDL_CreateThread(serialRead, "SerialReading", (void*)serial_port);
    if (!serialID) {
        printf("Thread Creation Failed: %s\n", SDL_GetError());
        SDL_DestroyMutex(inputsMTX);
        SDL_DestroyRenderer(renderer);
        TTF_Quit();
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }
	return EXIT_SUCCESS;
}

void SDLCleanup() {
	  SDL_DestroyMutex(inputsMTX);
  SDL_WaitThread(serialID, NULL);
  SDL_WaitThread(threadID, NULL);
  tcflush(serial_port, TCIOFLUSH);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  TTF_CloseFont(font);
  TTF_Quit();
}