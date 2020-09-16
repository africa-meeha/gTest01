/**
 * main_display.c - Main display window
 */

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH (1080)
#define WINDOW_HEIGHT (720)

// speed in pixels/second
#define SPEED (100)

void fix_border(SDL_Rect *border) {
    border->x -= 1+1;
    border->y -= 1-(border->h / 11);
    border->w += 1 -(border->w / 60)+1+1;
    border->h += 1-(33 * border->h / 100) +1;
}

typedef struct grid {
    int x;
    int y;
    int cell_w;
    int cell_h;
    int xcells;
    int ycells;
} grid;


void draw_grid(SDL_Renderer *rend, grid *pgrid){ //int x, int y, int cell_w, int cell_h, int xcells, int ycells) {
    SDL_Rect outer_rect;

    outer_rect.x =pgrid->x; outer_rect.y =pgrid->y;
    outer_rect.w = pgrid->cell_w * pgrid->xcells +1;
    outer_rect.h = pgrid->cell_h * pgrid->ycells +1;

    SDL_SetRenderDrawColor(rend, 255,0,0,255);
    SDL_RenderDrawRect(rend, &outer_rect);

    //SDL_RenderDrawLine(rend, x+cell_w,y+1,x+cell_w,y+outer_rect.h-2);

    int i=1;
    for (i=1 ; i < pgrid->xcells ; i++) {
        SDL_RenderDrawLine(rend, pgrid->x + (pgrid->cell_w * i), pgrid->y + 1, pgrid->x + (pgrid->cell_w * i),
                           pgrid->y + outer_rect.h - 2);
    }

    for (i=1 ; i < pgrid->ycells ; i++) {
        SDL_RenderDrawLine(rend, pgrid->x + 1, pgrid->y + (pgrid->cell_h * i), pgrid->x + outer_rect.w - 2,
                           pgrid->y + (pgrid->cell_h * i));
    }

}

int main(int argc, char *argv[])
{
    // attempt to initialize graphics and timer system
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("Hello, CS50!",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       WINDOW_WIDTH, WINDOW_HEIGHT,0);
    if (!win)
    {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // create a renderer, which sets up the graphics hardware
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);
    if (!rend)
    {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

//    SDL_Renderer *rend_grid = SDL_CreateRenderer(win, -1, render_flags);
//    if (!rend_grid)
//    {
//        printf("error creating renderer for grid: %s\n", SDL_GetError());
//        SDL_DestroyWindow(win);
//        SDL_Quit();
//        return 1;
//    }

    TTF_Init();
    TTF_Font* myfont = TTF_OpenFont("resources/consola.ttf", 24); //this opens a font style and sets a size
    SDL_Color White = {255, 255, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

    if (!myfont)
    {
        printf("error creating font\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }



    SDL_Surface* surface = TTF_RenderText_Solid(myfont, "1010", White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

    // load the image into memory using SDL_image library function
//    SDL_Surface* surface = IMG_Load("resources/hello.png");
    if (!surface)
    {
        printf("error creating surface\n");
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    // load the image data into the graphics hardware's memory
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);
    if (!tex)
    {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    // struct to hold the position and size of the sprite
    SDL_Rect dest, border;

    // get and scale the dimensions of texture
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    //dest.w /= 4;
    //dest.h /= 4;

    // start sprite in center of screen
    dest.x = (WINDOW_WIDTH - dest.w) / 2;
    dest.y = (WINDOW_HEIGHT - dest.h) / 2;

    border = dest;
//    border.x -= 1+1;
//    border.y -= 1-(border.h / 11);
//    border.w += 1 -(border.w / 60)+1+1;
//    border.h += 1-(33 * border.h / 100) +1;
    fix_border(&border);

    // set to 1 when window close button is pressed
    int close_requested = 0;

    // animation loop
    while (!close_requested)
    {
        // process events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                close_requested = 1;
            }
        }



        // clear the window
        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        SDL_RenderClear(rend);

        // draw the image to the window
        SDL_RenderCopy(rend, tex, NULL, &dest);

        SDL_SetRenderDrawColor(rend, 255,0,0,255);
        SDL_RenderDrawRect(rend, &border);

        grid grid1 = {25,25, border.w+20, border.h+20, 4, 16};

        draw_grid(rend, &grid1);

        //*rend = *rend_grid;

        SDL_RenderPresent(rend);

        // wait 1/60th of a second
        SDL_Delay(1000/60);
    }

    // clean up resources before exiting
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
