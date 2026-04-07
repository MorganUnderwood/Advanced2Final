
#include "render_text.h"


void RenderText(SDL_Renderer *renderer, TTF_Font *font,const char *current,const char *future,const char *past,int textWidth)
{
    // Create surface with rendered text

    SDL_Color textColour = {255, 255, 255, 255};

    SDL_Surface *quitSurface = TTF_RenderText_Solid(font, "Press Q or Escape to Exit", textColour);
    SDL_Surface *currentDateSurfaceHeader = TTF_RenderText_Blended_Wrapped(font, "The current date is: ", textColour, 305);
    SDL_Surface *futureDateSurfaceHeader = TTF_RenderText_Blended_Wrapped(font, "The current simulated future date is: ", textColour, 305);
    SDL_Surface *pastDateSurfaceHeader = TTF_RenderText_Blended_Wrapped(font, "The current simulated past date is: ", textColour, 305);




    SDL_Surface *currentDateSurface = TTF_RenderText_Solid(font, current, textColour);
    SDL_Surface *futureDateSurface = TTF_RenderText_Solid(font, future, textColour);
    SDL_Surface *pastDateSurface = TTF_RenderText_Solid(font, past, textColour);


    // Create texture from surface
    SDL_Texture *quitTexture = SDL_CreateTextureFromSurface(renderer, quitSurface);
    SDL_Texture *currentDateHeaderTexture = SDL_CreateTextureFromSurface(renderer, currentDateSurfaceHeader);
    SDL_Texture *futureDateHeaderTexture = SDL_CreateTextureFromSurface(renderer, futureDateSurfaceHeader);
    SDL_Texture *pastDateHeaderTexture = SDL_CreateTextureFromSurface(renderer, pastDateSurfaceHeader);




    SDL_Texture *currentDateTexture = SDL_CreateTextureFromSurface(renderer, currentDateSurface);
    SDL_Texture *futureDateTexture = SDL_CreateTextureFromSurface(renderer, futureDateSurface);
    SDL_Texture *pastDateTexture = SDL_CreateTextureFromSurface(renderer, pastDateSurface);

    // Render text
    SDL_Rect quitRect = {50, 50, quitSurface->w, quitSurface->h};
    SDL_RenderCopy(renderer, quitTexture, NULL, &quitRect);

    SDL_Rect currentDateHeaderRect = {textWidth, 50, currentDateSurfaceHeader->w, currentDateSurfaceHeader->h};
    SDL_RenderCopy(renderer, currentDateHeaderTexture, NULL, &currentDateHeaderRect);

    SDL_Rect futureDateHeaderRect = {textWidth, 320, futureDateSurfaceHeader->w, futureDateSurfaceHeader->h};
    SDL_RenderCopy(renderer, futureDateHeaderTexture, NULL, &futureDateHeaderRect);

    SDL_Rect pastDateHeaderRect = {textWidth, 590, pastDateSurfaceHeader->w, pastDateSurfaceHeader->h};
    SDL_RenderCopy(renderer, pastDateHeaderTexture, NULL, &pastDateHeaderRect);


    SDL_Rect currentDateRect = {textWidth, 185, currentDateSurface->w, currentDateSurface->h};
    SDL_RenderCopy(renderer, currentDateTexture, NULL, &currentDateRect);

    SDL_Rect futureDateRect = {textWidth, 455, futureDateSurface->w, futureDateSurface->h};
    SDL_RenderCopy(renderer, futureDateTexture, NULL, &futureDateRect);

    SDL_Rect pastDateRect = {textWidth, 725, pastDateSurface->w, pastDateSurface->h};
    SDL_RenderCopy(renderer, pastDateTexture, NULL, &pastDateRect);



}
void simulatedDate (SDL_Renderer *renderer, TTF_Font *font,const char *simulated, int textWidth){

    SDL_Color textColour = {255, 255, 255, 255};
	    SDL_Surface *currentSimDateSurfaceHeader = TTF_RenderText_Blended_Wrapped(font, "The current simulated date is: ", textColour, 305);
	        SDL_Surface *currentSimDateSurface = TTF_RenderText_Solid(font, simulated, textColour);
	            SDL_Texture *currentSimHeaderTexture = SDL_CreateTextureFromSurface(renderer, currentSimDateSurfaceHeader);
	                SDL_Texture *currentSimDateTexture = SDL_CreateTextureFromSurface(renderer, currentSimDateSurface);
	                
    SDL_Rect currentSimHeaderRect = {textWidth, 860, currentSimDateSurfaceHeader->w, currentSimDateSurfaceHeader->h};
    SDL_RenderCopy(renderer, currentSimHeaderTexture, NULL, &currentSimHeaderRect);
        SDL_Rect currentSimDateRect = {textWidth, 995, currentSimDateSurface->w, currentSimDateSurface->h};
    SDL_RenderCopy(renderer, currentSimDateTexture, NULL, &currentSimDateRect);
}
	
