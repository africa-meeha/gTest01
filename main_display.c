/**
 * main_display.c - Main display window
 */

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "shared_mem.h"

const int WINDOW_WIDTH = 1920;//1080*1.5;
const int WINDOW_HEIGHT = 1080;//720*1.5;



SDL_Window *win1;
SDL_Renderer *rend1;
TTF_Font *font1;
SDL_Color WHITE = {255, 255, 255};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

int ipow(int base, int exp)
{
    int result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}


void fix_border_pos(SDL_Rect *border) {
    border->x += 1;
    border->y += 2;
}

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
    int right;
    int bottom;
} grid;



// speed in pixels/second
// #define SPEED (100)


void draw_grid(SDL_Renderer *rend, grid *pgrid){ //int x, int y, int cell_w, int cell_h, int xcells, int ycells) {
    SDL_Rect outer_rect;

    outer_rect.x =pgrid->x; outer_rect.y =pgrid->y;
    outer_rect.w = pgrid->cell_w * pgrid->xcells +1;
    outer_rect.h = pgrid->cell_h * pgrid->ycells +1;

    pgrid->right = outer_rect.x + outer_rect.w;
    pgrid->bottom = outer_rect.y + outer_rect.h;


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

grid ref_header_grid, table_header_grid;
grid ref_grid, table_grid;

SDL_Texture *tex_from_text(char *text1) {
    //char *text1;// = "hello";
    //text1 = (char*)malloc((NVAL+1)*sizeof(char));
    //sprintf(text1,"%d",int1);
    SDL_Surface* surface = TTF_RenderText_Solid(font1, text1, WHITE); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
    //free(text1);

    if (!surface)
    {
        printf("error creating text surface: %s\n", SDL_GetError());
        //TTF_CloseFont(font1);
        return 0;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend1, surface);
    SDL_FreeSurface(surface);

    if (!tex)
    {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend1);
        SDL_DestroyWindow(win1);
        SDL_Quit();
        return 0;
    }

    return tex;
}

int init_grids() {

    int int1 = 0;
    for (int i =0; i<NVAL; i++) {
        int1+= ipow(10,i) * 8;
    }
    char *text1;// = "hello";
    text1 = (char*)malloc((NVAL+1)*sizeof(char));
    sprintf(text1,"%d",int1);
    SDL_Surface* surface = TTF_RenderText_Solid(font1, text1, WHITE); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
    free(text1);

    if (!surface)
    {
        printf("error creating text surface: %s\n", SDL_GetError());
        TTF_CloseFont(font1);
        return 1;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend1, surface);
    SDL_FreeSurface(surface);
    if (!tex)
    {
        printf("error creating texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(rend1);
        SDL_DestroyWindow(win1);
        SDL_Quit();
        return 1;
    }


    SDL_Rect border;

    // get and scale the dimensions of texture
    SDL_QueryTexture(tex, NULL, NULL, &border.w, &border.h);
    SDL_DestroyTexture(tex);

    fix_border(&border);

    border.w +=20;
    border.h +=20;

    SDL_SetRenderDrawColor(rend1, 255,0,0,255); //Red

    grid grid1 = {125,25, border.w, border.h, 3, 1};
    ref_header_grid = grid1;

    draw_grid(rend1, &ref_header_grid);

    grid grid2 = { ref_header_grid.x,ref_header_grid.bottom+4, border.w, border.h, 3, MAX+1};
    ref_grid = grid2;

    draw_grid(rend1, &ref_grid);


    SDL_SetRenderDrawColor(rend1, 0,255,0,255); //Green


    grid grid3 = { ref_header_grid.right + 3,ref_header_grid.y, border.w, border.h, NVAL, 1};
    table_header_grid = grid3;

    draw_grid(rend1, &table_header_grid);

    grid grid4 = { table_header_grid.x,ref_grid.y, border.w, border.h, NVAL, MAX+1};
    table_grid = grid4;

    draw_grid(rend1, &table_grid);

    //char *text1;// = "hello";
    text1 = (char*)malloc((NVAL+1)*sizeof(char));

    SDL_Rect bg_table;
    bg_table.w = table_header_grid.cell_w-20;
    bg_table.h = table_header_grid.cell_h-20;
    //bg_table.x = table_grid.x + (j * table_grid.cell_w) + 10;
    bg_table.y = table_header_grid.y + (0 * table_header_grid.cell_h) + 10;
    //border.x = table_grid.x + (j * table_grid.cell_w) + (table_grid.cell_w-border.w)/2;
    border.y = 8+table_header_grid.y + (0 * table_header_grid.cell_h) + (table_header_grid.cell_h-border.h)/2 ;


    for(int j=0; j<NVAL; j++) {
        //printf("%d  \t", aTable[i][j]);
        sprintf(text1,"%d",j+1);
        SDL_Texture *tex = tex_from_text(text1);

        SDL_QueryTexture(tex, NULL, NULL, &border.w, &border.h);

        //fix_border_size(&border);

        bg_table.x = table_header_grid.x + (j * table_header_grid.cell_w) + 10;
        //bg_table.y = table_grid.y + (i * table_grid.cell_h) + 10;
        border.x = table_header_grid.x + (j * table_header_grid.cell_w) + (table_header_grid.cell_w-border.w)/2;
        //border.y = table_grid.y + (i * table_grid.cell_h) + (table_grid.cell_h-border.h)/2 ;

        //fix_border_pos(&border);

        SDL_SetRenderDrawColor(rend1, 0,0,0,255);
        SDL_RenderFillRect(rend1, &bg_table);
        SDL_RenderCopy(rend1, tex, NULL, &border);

        SDL_DestroyTexture(tex); // Wittness memory leak without this line
    }

    free(text1);
}




int (*RefSet)[3];
int (*aTable)[NVAL];

void render_tables() {

    int int1 = 0;
    char *text1;// = "hello";
    text1 = (char*)malloc((NVAL+1)*sizeof(char));

    SDL_Rect border, bg_ref, bg_table;
    bg_ref.w = ref_grid.cell_w-20;
    bg_ref.h = ref_grid.cell_h-20;
    bg_table.w = table_grid.cell_w-20;
    bg_table.h = table_grid.cell_h-20;



    for (int i=0;i<=MAX;i++) {
        for(int j=0; j<3; j++) {
            //printf("%d  \t", RefSet[i][j]);
            sprintf(text1,"%d",RefSet[i][j]);
            SDL_Texture *tex = tex_from_text(text1);

            SDL_QueryTexture(tex, NULL, NULL, &border.w, &border.h);

            bg_ref.x = ref_grid.x + ((2-j) * ref_grid.cell_w) + 10;
            bg_ref.y = ref_grid.y + (i * ref_grid.cell_h) + 10;
            border.x = ref_grid.x + ((2-j) * ref_grid.cell_w) + (ref_grid.cell_w-border.w)/2;
            border.y = ref_grid.y + (i * ref_grid.cell_h) + (ref_grid.cell_h-border.h)/2 ;

            fix_border_pos(&border);

            SDL_SetRenderDrawColor(rend1, 0,0,0,255); //Black
            SDL_RenderFillRect(rend1, &bg_ref);
            SDL_RenderCopy(rend1, tex, NULL, &border);

            SDL_DestroyTexture(tex); // Wittness memory leak without this line

        }

        //printf("    \t");

        for(int j=0; j<NVAL; j++) {
            //printf("%d  \t", aTable[i][j]);
            sprintf(text1,"%d",aTable[i][j]);
            SDL_Texture *tex = tex_from_text(text1);

            SDL_QueryTexture(tex, NULL, NULL, &border.w, &border.h);

            //fix_border_size(&border);

            bg_table.x = table_grid.x + (j * table_grid.cell_w) + 10;
            bg_table.y = table_grid.y + (i * table_grid.cell_h) + 10;
            border.x = table_grid.x + (j * table_grid.cell_w) + (table_grid.cell_w-border.w)/2;
            border.y = table_grid.y + (i * table_grid.cell_h) + (table_grid.cell_h-border.h)/2 ;

            fix_border_pos(&border);

            SDL_SetRenderDrawColor(rend1, 0,0,0,255);
            SDL_RenderFillRect(rend1, &bg_table);
            SDL_RenderCopy(rend1, tex, NULL, &border);

            SDL_DestroyTexture(tex); // Wittness memory leak without this line
        }
        //printf("?\n");
    }

    //SDL_RectEmpty(&border);
    free(text1);
}

int main(int argc, char *argv[])
{
    int table1[MAX+1][3];
    RefSet = table1;
    int table2[MAX+1][NVAL];
    aTable = table2;
    init_tables();
    //randomize_tables(MAX+1);
    //print_tables();

    // attempt to initialize graphics and timer system
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    win1 = SDL_CreateWindow("Main Display",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       WINDOW_WIDTH, WINDOW_HEIGHT,0);
    if (!win1)
    {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // create a renderer, which sets up the graphics hardware
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    rend1 = SDL_CreateRenderer(win1, -1, render_flags);
    if (!rend1)
    {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(win1);
        SDL_Quit();
        return 1;
    }

    TTF_Init();
    font1 = TTF_OpenFont("resources/consola.ttf", 24); //this opens a font style and sets a size

    if (!font1)
    {
        printf("error creating font\n");
        SDL_DestroyRenderer(rend1);
        SDL_DestroyWindow(win1);
        SDL_Quit();
        return 1;
    }

    // clear the window
    SDL_SetRenderDrawColor(rend1, 0, 0, 0, 255);
    SDL_RenderClear(rend1);

    init_grids();

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

        randomize_tables(9999);
        render_tables();


//        // draw the image to the window
//        SDL_RenderCopy(rend, tex, NULL, &dest);
//
//        SDL_SetRenderDrawColor(rend, 255,0,0,255);
//        SDL_RenderDrawRect(rend, &border);
//
//        grid grid1 = {25,25, border.w+20, border.h+20, 4, 16};


        SDL_RenderPresent(rend1);

        // wait 1/60th of a second
        SDL_Delay(1000/30);
    }

    // clean up resources before exiting
    //SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(rend1);
    SDL_DestroyWindow(win1);
    SDL_Quit();
}
