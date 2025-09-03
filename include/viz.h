#pragma once
#ifdef USE_SDL
#include <SDL2/SDL.h>
typedef struct { int w,h; SDL_Window* win; SDL_Renderer* ren; } Viz;
int viz_open(Viz* v, const char* title, int w, int h);
void viz_clear(Viz* v, int r,int g,int b);
void viz_line(Viz* v, int x1,int y1,int x2,int y2, int r,int g,int b);
void viz_present(Viz* v);
void viz_close(Viz* v);
#endif
