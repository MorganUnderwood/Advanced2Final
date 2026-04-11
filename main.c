/*TODO (listed from most important to least important).
make drawHeight in an array and use it in a for loop to remove magic numbers and remove some lines of code in main.
Scaling options for different screen resolutions because it just forces 1920x1080 at the moment.
Clean up the main because it's a mess (and still maybe add some more .c and .h files).
*/






#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> //requires libfreetype6-dev?
#include <math.h>
#include <stdbool.h>
#include <time.h>

#include "render_text.h"
#include "time_calc.h"



#define PLANETS 8
#define PI 3.1415926535897932384626433832795
#define MY_FONT "/usr/share/fonts/truetype/noto/NotoSansMono-Regular.ttf" //CHECK TO MAKE SURE THE DEFAULT PI HAS IT I DIDN'T INSTALL ANY FONTS AND IT WAS THERE.
#define WIDTH 1920
#define HEIGHT 1080
int drawHeight = 0; //(just for me) 1080 / 4 = 270 so from 265 to 275 draw lines i suppose from width to the right.
const int drawWidth = (WIDTH - (WIDTH / 6)); //this should be 1/6 of the screen over for the white lines/layout for text on the right side of the screen. (for me. It would be 1680 (1675-1685)at the moment for the functions).
bool layout = false;
const int textWidth = drawWidth + 10; //the width on the monitor where the text will be for the date parameters.
double angle[8];
int plusCalc, minusCalc;
bool button = false;
bool setup = false; //this will draw the constant things like text and the sun. (eventually) and then go true.





typedef struct {
    const char *name;
    double distanceFromSun;
    double angle;
    double speed;
    int size;
    SDL_Color color;
} Planet;

void CircleFunction (SDL_Renderer* render, int x, int y, int r, SDL_Color color){
    SDL_SetRenderDrawColor (render, color.r, color.g, color.b, 255); //draws a circle based upon the colour inputted.
    for (int i = -r; i <= r; i++){
        for (int j = -r; j <= r; j++){
            if (i*i + j*j <= r*r){
                SDL_RenderDrawPoint(render, x+i, j+y);
            }
        }
    }
}
void updatePosition(Planet *p){
    p->angle += p->speed; //updates the angle relative to the speed.
    if (p->angle > 2* PI) //ensures that the angle doesn't exceed 2 * PI since that's a full revolution and subtracts 2*PI if it is.
        p->angle -= 2* PI;
    }
    
void planetDraw (SDL_Renderer *render, Planet p, int x, int y){//takes the value from updatePosition and updates the position using cos and sin

    int cx = x + cos(p.angle) * p.distanceFromSun; 
    int cy = y + sin (p.angle) * p.distanceFromSun;
    
    CircleFunction (render, cx, cy, p.size, p.color);
}


    
int main() {

if (SDL_Init(SDL_INIT_VIDEO) <0) {
    printf ("SDL Init Failed: %s\n", SDL_GetError());
    return EXIT_FAILURE;
}
    
    SDL_Window *window = SDL_CreateWindow(
        "Planetarium",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT, //Window width and height.
        SDL_WINDOW_FULLSCREEN
    );
    
    if (TTF_Init() < 0) {
        SDL_Quit();
        return EXIT_FAILURE;
        
    }
       SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    
    //FUNCTION THAT RETURNS TIME SINCE JANUARY 1 1970 USED TO MULTIPLY BY PLANET SPEED AND ANGLE TO SET COORDINATES.
    
    time_t currentDate;
    time(&currentDate);
    long long timeSinceEpoch = (long long ) currentDate;
    int daysSinceEpoch = timeSinceEpoch / SECONDS_IN_DAY;
   

    
 
     Planet planets[] ={ //Initialize the planets with distance from sun, initial angle (based upon real time), orbit speed, size and colour. //REMEMBER TO CHANGE BACK TO REAL TIME THIS IS FAST FOR TESTING.
        {"Mercury", 60, 0, 0.05, 4, {183, 184, 185, 255}},
        {"Venus", 100, 0, 0.01667, 6 ,{248, 226, 176, 255}},
        {"Earth", 150, 0, 0.01, 6, {0, 100, 255, 255}},
        {"Mars", 190, 0,  0.005, 5, {173,98, 60, 255}},
        {"Jupiter", 240, 0, 0.00084, 14, {227, 220, 209, 255}},
        {"Saturn", 310, 0, 0.0003898, 12, {206, 184, 184, 255}},
        {"Uranus", 380, 0, 0.0001905, 9, {175, 229, 238, 255}},
        {"Neptune", 450,0, 0.00006068, 9, {91, 93, 223, 255}}
		//Planet, Distance from Sun, Initial angle(calculated below), Orbital speed, size, colour.
    };
    for (int i=0;i<PLANETS; i++){ //takes days since epoch time and multiplies it by the orbital speed of each planet which sets the angle of each planet.
        double angleCalculated = fmod(daysSinceEpoch*planets[i].speed, 360);

        if (angleCalculated < 0) angleCalculated+= 360.0;

        if (angleCalculated < 0){
			angleCalculated+= 360.0;
		}
        planets[i].angle = angleCalculated;
        
}


    


    //TIME CALCULATIONS HAS TO BE INTEGRATED DOESN'T DO ANYTHING AT THE MOMENT BESIDES SPIT OUT 50 DAYS IN THE FUTURE/PAST IN CONSOLE.
    
	//25 characters should be more than enough for example Jan 01 2026 would be 12 in length
	//and adding time would be 06:00 would be 18
	
    char current[25]; 
    char future[25];
    char past[25];
    char simulated[25];
    int plusDays, minusDays;
    
    
	
	
	//CURRENTLY SET TO +/-50 DAYS FROM CURRENT DAY CAN UNCOMMENT BELOW AND REMOVE THE PLUSDAYS AND MINUSDAYS BELOW TO TRY DIFFERENT DAYS.
	
	//current calc will have to get the code for the potentiometers to implement properly.
	plusDays = 50;
	minusDays = 50;



    //DEBUGGING FOR MANUAL ENTRY IF WANTED (which will be used with pots for debugging).
    /*printf("Enter number of days to add (+): ");
    scanf("%d", &plusDays);

    printf("Enter number of days to subtract (-): ");
    scanf("%d", &minusDays);
	*/

    timeCalculation(current, future ,past, plusDays, minusDays);

	//DEBUGGING
    printf("Current date:        %s\n", current);
    printf(" + %d days:      %s\n", plusDays, future);
    printf(" - %d days:      %s\n", minusDays, past);
    int simulatedDays = (timeSinceEpoch- (minusDays* SECONDS_IN_DAY));
    formatDate (simulatedDays, simulated);
    
    //END TIME CALCULATIONS.
    

        
    bool running = true; //set running to true this will keep the program running until it's closed within the while loop below.
    SDL_Event event;
    int center[2]; //int center as an array for the center of the screen.
    center[0] = drawWidth /2; // makes it the center of the area without text since the drawWidth is to the left of where the lines are drawn.
    center[1] = HEIGHT/2; //Determines center of the screen
    
    //FONT AND TEXT COLOUR SETUP

    TTF_Font *font = TTF_OpenFont(MY_FONT, 24); // big font for areas with no area constraints.
    
    SDL_Color textColour = {255, 255, 255, 255}; // white color
    

 
    
    
        // THIS FUNCTION WILL SET THE ANGLE STARTING AT MINUSDAYS WILL SIMULATE TO FINISH DAY WITH A 500MS DELAY IT PROBABLY WON'T WORK RIGHT NOW HAVE TO CHANGE DAY AND ANGLE CALCS
          //HAVE TO ADD QUIT COMMAND TO FUNCTION AS WELL FOR Q AND ESCAPE WITH DELAY FUNCTION.
      
      

	
	
    


    while(running) {

        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT){
                running = false;
        } else if (event.type == SDL_KEYDOWN){ //Checks if key was pressed.
            
            if (event.key.keysym.sym == SDLK_q|| event.key.keysym.sym == SDLK_ESCAPE){ //quits if the key pressed was q or escape and exits program if it was.
                running = false;
            } 
            
        }
    }
  
       
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //Clear the window so it becomes black.
        SDL_RenderClear(renderer); //clear the renderer so there is no black leftover or any issues that may come with that.
    SDL_SetRenderDrawColor(renderer,255, 255, 255, 255); // white colour WILL HAVE TO FIX MAGIC NUMBERS BELOW.
    
    //Draw lines on the right side of the screen (will get rid of magic numbers)
      SDL_RenderDrawLine (renderer, drawWidth, 1, drawWidth , HEIGHT);
      drawHeight = 270;
      SDL_RenderDrawLine (renderer, drawWidth, drawHeight, WIDTH, drawHeight );
      drawHeight = 540;
      SDL_RenderDrawLine (renderer, drawWidth, drawHeight, WIDTH, drawHeight);
      drawHeight = 810;
      SDL_RenderDrawLine (renderer, drawWidth, drawHeight, WIDTH, drawHeight);

  if (!font){ //checks to make sure it can get the font. //IF USING MED FONT AT SOME POINT WILL HAVE TO ADD THAT CHECK

    printf("Failed to load font: %s\n", TTF_GetError());
    return EXIT_FAILURE;
  }
  
    //TEXT RENDERER for all besides simulated.
  
  RenderText (renderer, font,current, future, past, textWidth);
  CircleFunction(renderer, center[0], center[1], 25, (SDL_Color){255, 255, 0 , 255}); //Draw the sun at the center of the window
  
        if (button == true){
      simulated[0] = "";
      int start = daysSinceEpoch - minusDays;
      int finish = daysSinceEpoch + plusDays;
      
      while (start < finish) {
      for (int i=0; i<PLANETS;i++){
      planets[i].angle = 0;
      double angleCalculated = fmod(start*planets[i].speed, 360);
              if (angleCalculated < 0) angleCalculated+= 360.0;

        if (angleCalculated < 0){
			angleCalculated+= 360.0;
		}
        planets[i].angle = angleCalculated;
        planetDraw (renderer, planets[i], center[0], center[1]); //draw the planets relative to the sun
        updatePosition(&planets[i]); //update the position of the planets.
        }
        start++;
         SDL_RenderPresent(renderer);
        SDL_Delay(16); //16 will give ~60fps if we are having issues can cut to 33 for 30fps and it will still be fairly smooth.
         simulatedDays = simulatedDays + SECONDS_IN_DAY;
         formatDate (simulatedDays, simulated);
     }
     }
     if (button == false) {
         simulated[25] = "Not Currently Simulating";
         simulatedDate (renderer, font, simulated, textWidth);
        
         for (int i=0; i<PLANETS; i++){
             double angleCalculated = fmod (daysSinceEpoch * planets[i].speed, 360);
             if (angleCalculated < 0) {
                 angleCalculated +=360;
              }
              planets[i].angle = angleCalculated;
              planetDraw (renderer, planets[i], center[0], center[1]);
              updatePosition (&planets[i]);
          }
      }
              

  



SDL_RenderPresent(renderer);
        SDL_Delay(16); //16 will give ~60fps if we are having issues can cut to 33 for 30fps and it will still be fairly smooth.


    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_CloseFont (font);
    TTF_Quit();

    return 0;
}
//MAKE FILE


/*
# A simple Makefile for compiling small SDL projects

# set the compiler
CC := gcc

# set the compiler flags
CFLAGS := `sdl2-config --libs --cflags` -ggdb3 -O0 --std=c99 -Wall -lSDL2_image -lSDL2_ttf -lm

# add header files here
HDRS :=

# add source files here
<<<<<<< HEAD
SRCS := Graphics.c
=======
SRCS := Graphics.c time_calc.c
>>>>>>> 570d761 (Added function that calculats +/- x days)

# generate names of object files
OBJS := $(SRCS:.c=.o)

# name of executable
EXEC := planetarium

# default recipe
all: $(EXEC)

# recipe for building the final executable
$(EXEC): $(OBJS) $(HDRS) makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS)

# recipe for building object files
#$(OBJS): $(@:.o=.c) $(HDRS) makefile
#	$(CC) -o $@ $(@:.o=.c) -c $(CFLAGS)

# recipe to clean the workspace
clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean
*/ 

/*OLD PLANET SCALING FOR TESTING WHERE ITS NOT GOING AT 47,000 KM/H

    Planet planets[] ={ //Initialize the planets with distance from sun, initial angle (will change when adding real time), speed around the sun, size and colour.
        {"Mercury", 60, 0, 0.05, 4, {183, 184, 185, 255}},
        {"Venus", 90, 0, 0.01667, 6 ,{248, 226, 176, 255}},
        {"Earth", 130, 0, 0.01, 6, {0, 100, 255, 255}},
        {"Mars", 170, 0,  0.005, 5, {173,98, 60, 255}},
        {"Jupiter", 240, 0, 0.00084, 14, {227, 220, 209, 255}},
        {"Saturn", 310, 0, 0.0003898, 12, {206, 184, 184, 255}},
        {"Uranus", 380, 0, 0.0001905, 9, {175, 229, 238, 255}},
        {"Neptune", 450,0, 0.00006068, 9, {91, 93, 223, 255}}
    };

*CURRENT PLANET SCALING FOR ACTUAL PROJECT.
*     Planet planets[] ={ //Initialize the planets with distance from sun, initial angle (will change when adding real time), speed around the sun, size and colour.
        {"Mercury", 60, 0, 4.091, 4, {183, 184, 185, 255}},
        {"Venus", 90, 0, 1.6, 6 ,{248, 226, 176, 255}},
        {"Earth", 130, 0, 0.985626, 6, {0, 100, 255, 255}},
        {"Mars", 170, 0, 0.5263, 5,{173,98, 60, 255}},
        {"Jupiter", 240, 0, 0.0831, 14, {227, 220, 209, 255}},
        {"Saturn", 310, 0, 0.03346, 12, {206, 184, 184, 255}},
        {"Uranus", 380, 0, 0.01173, 9, {175, 229, 238, 255}},
        {"Neptune", 450,0, 0.00598, 9, {91, 93, 223, 255}}
    };
    * 
    * */
