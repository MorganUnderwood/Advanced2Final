#include "main.h"


  /*Function that draws circles in relation to x,y placement on the screen with radius and colour*/
void CircleFunction(SDL_Renderer *render, int x, int y, int r,
                    SDL_Color color) { 
        /*Set Colour*/
  SDL_SetRenderDrawColor(render, color.r, color.g, color.b, 255);
  
        /*Draw Circle of the colour set from the set colour above */
  for (int i = -r; i <= r; i++) {
    for (int j = -r; j <= r; j++) {
      if (i * i + j * j <= r * r) {
        SDL_RenderDrawPoint(render, x + i, j + y);
      }
    }
  }
}

  /*Function that clears the screen and draws again*/
void clearPrev(SDL_Renderer *renderer, TTF_Font *font, const char *current,
               const char *future, const char *past, int textWidth) {
  /*Clear screen*/
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  
  /*Set draw colour to white */
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  
  
    /*Set an array for lines to draw to divide the screen into 4 equal sections.*/
  int horizontalLines[3];
  for (int i = 0; i <3; i++){
    if (i ==0) {
      horizontalLines[i] = WIDTH - (WIDTH/6);
    }
    else {
      horizontalLines[i] = horizontalLines[i-1] + horizontalLines[0];
      
    }
     SDL_RenderDrawLine(renderer, drawWidth, horizontalLines[i], WIDTH,
                       horizontalLines[i]);
}

  /*Draw vertical line to complete layout that houses text*/
SDL_RenderDrawLine(renderer, drawWidth, 1, drawWidth, HEIGHT);

  /*Draw the sun*/
  CircleFunction(renderer, center[0], center[1], 25,
                 (SDL_Color){255, 255, 0, 255});
    /*Render the constant text*/
  RenderText(renderer, font, current, future, past, textWidth);

  return;
}

/*Calculate the motion of a planet relative to orbital speed*/
void planetDraw(SDL_Renderer *render, Planet p, int x, int y) {
  /*cx controls the rotation of the planet horizontally (left to right*/
  int cx = x + cos(p.angle) * p.distanceFromSun;
  /*cy controls the rotation of the planet vertically (up and down)*/
  int cy = y - sin(p.angle) * p.distanceFromSun;

  /*Re-draw the planet with the updated data*/
  CircleFunction(render, cx, cy, p.size, p.color);
}
/
  /*Collect serial data from potentiometers serially via an Arduino to control the +/- days variable*/
int openSerial() {

  int serial_port;

  if ((serial_port = serialOpen("/dev/ttyACM0", 115200)) < 0) { 
    return 1;
  }
  if (wiringPiSetup() == -1) {
    return 1;
  }

  return serial_port;
}
  /*Set up thread to run the program, while also having the option of the quit keys of 'q' and 'esc' to quit while the operation is running*/
int InputThread(void *data) {
  SDL_Event event;
  volatile bool *running = (volatile bool *)data;
  while (*running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        *running = false;
      } else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_q ||
            event.key.keysym.sym == SDLK_ESCAPE) {
          *running = false;
        }
      }
    }

  }

  return 0;
}

int serialRead(void *data) {
  int serial_port = *(int *)data;

  char buf[100];
  int bufIndex = 0;
  int tokenCount = 0;

  while (running) {
    if (!serialDataAvail(serial_port)) {
      SDL_Delay(1);
      continue;
    }

    bufIndex = 0;
    tokenCount = 0;
    while (serialDataAvail(serial_port) > 0) {
      char c = serialGetchar(serial_port);
      if (c == '\n')
        break;
      buf[bufIndex++] = c;
    }
    buf[bufIndex] = '\0';
    char *tokens[20];

    char *token;

    token = strtok(buf, "!");

    while (token != NULL) {
      tokens[tokenCount] = token;
      tokenCount++;
      token = strtok(NULL, "!");
    }
    if (tokenCount >= 2) {
      if (tokenCount == 3) {
        button = true;
      }

      int pastDays = atoi(tokens[0]);
      int futureDays = atoi(tokens[1]);

      SDL_LockMutex(inputsMTX);
      sharedInputs.minusDays = pastDays;
      sharedInputs.plusDays = futureDays;
      SDL_UnlockMutex(inputsMTX);
    }

    tcflush(serial_port, TCIOFLUSH);
    tokenCount = 0;
  }
  return 0;
}
  /*Update the position of the planets.
   *Information is based on the angular velocity of each planet
   *and added with the previous angle                        */
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

int main() {
  /*Set up buffers*/
  char current[25];
  char future[25];
  char past[25];
  char simulated[25];
  
  /*Initialize +/- days and the center of the screen that will be drawn on */
  int plusDays = 0;
  int minusDays = 0;
  center[0] = drawWidth / 2;
  center[1] = HEIGHT / 2;

  int serial_port = openSerial();
  if (serial_port == 1) {
    return 1;
  }

  /*Initialize SDL video, if it fails return failure*/
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL Init Failed: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }
  /*Set up the window with a name, position, width, height and display mode (full screen)*/
  SDL_Window *window = SDL_CreateWindow("Planetarium", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, WIDTH,
                                        HEIGHT, // Window width and height.
                                        SDL_WINDOW_FULLSCREEN);
                                        
                                          /*Using the SDL_Window above, render a window matching the parameters*/
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
                                        

      
 /*Calculate different time variables needed*/
  time_t currentDate;
  time(&currentDate);
  long long timeSinceEpoch = (long long)currentDate - 946684800;
  long long timeSinceLinuxEpoch = (long long)currentDate;
  int daysSinceLinEpoch = timeSinceLinuxEpoch / SECONDS_IN_DAY;
  int daysSinceEpoch = timeSinceEpoch / SECONDS_IN_DAY;
  
    /*Make sure text initializes and exit program if it fails*/
  if (TTF_Init() < 0) {
    SDL_Quit();
    return EXIT_FAILURE;
  }
  
  /*Set font and font size for the program*/
  TTF_Font *font = TTF_OpenFont(MY_FONT, 24);
  
    /*Create an SDL thread for the serial data input*/
  inputsMTX = SDL_CreateMutex();
  SDL_Thread *serialID =
  SDL_CreateThread(serialRead, "SerialReading", (void *)&serial_port);

  SDL_Thread *threadID =
      SDL_CreateThread(InputThread, "Listener", (void *)&running);

  /*SDL graphics will continue to run until a quit command is entered*/
  while (running) {
    
  /*Variables that correspond to input from Arduino*/
    int pastDayRead, futureDayRead;

    /*Allow pastday and futureday to be updated, then closed*/
    SDL_LockMutex(inputsMTX);
    pastDayRead = sharedInputs.minusDays;
    futureDayRead = sharedInputs.plusDays;
    SDL_UnlockMutex(inputsMTX);

    /*Set minus days to past day and plusdays to futuredayread*/
    minusDays = pastDayRead;
    plusDays = futureDayRead;
    
    /*Calculate the time of all inputs in order to display them on the screen, then clear the previous data, then render new data*/
    timeCalculation(currentDate, current, future, past, plusDays, minusDays);
    clearPrev(renderer, font, current, future, past, textWidth);
    RenderText(renderer, font, current, future, past, textWidth);

    /*Check for if the start button has been pressed*/
    if (button == true) {
      /*Set the start and finish points to set u the simulation*/
      int start = -minusDays;
      int finish = +plusDays;

      while (start < finish) {
        
        
        long long simulatedDay = daysSinceLinEpoch + start;
        long long simulatedSeconds = simulatedDay * SECONDS_IN_DAY;
        
        
        /*Clear previous date, format the currently simulated date and render it onto the screen */
        clearPrev(renderer, font, current, future, past, textWidth);
        formatDate(simulatedSeconds, simulated);
        simulatedDate(renderer, font, simulated, textWidth);
        
        
        /*Update planetary position by 1 earth day */
        planetUpdate(renderer, planets, PLANETS, simulatedDay, center[0],
                     center[1]);
                     
        /*quit if q or esc is pressed */
        if (!running) {
          start = finish;
          break;
        }
        /*Increment start to prepare for the next simulated day */
        start++;
        
        /*SDL cleanup */
        SDL_RenderPresent(renderer);
        SDL_Delay(1);
      }

      tcflush(serial_port, TCIOFLUSH);
      
      button = false;
    } else if (button == false) {
      
      /*If the button is NOT pressed than the current planetary alignment corresponds with today's date and then display all planets in that alignment*/
      clearPrev(renderer, font, current, future, past, textWidth);
      planetUpdate(renderer, planets, PLANETS, daysSinceEpoch, center[0],
                   center[1]);
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }
  
  /*SDL and serial cleanup */

  SDL_DestroyMutex(inputsMTX);
  SDL_WaitThread(serialID, NULL);
  SDL_WaitThread(threadID, NULL);
  tcflush(serial_port, TCIOFLUSH);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  TTF_CloseFont(font);
  TTF_Quit();

  return 0;
}
