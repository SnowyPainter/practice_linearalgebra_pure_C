#pragma once
#ifdef USE_SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
typedef struct {
    SDL_Window* win;
    SDL_Renderer* ren;
    int w, h;
    TTF_Font* font;
  } Viz;
int viz_open(Viz* v, const char* title, int w, int h);
void viz_clear(Viz* v, int r,int g,int b);
int viz_line(Viz* v, int x1,int y1,int x2,int y2, int r,int g,int b);
void viz_present(Viz* v);
void viz_close(Viz* v);

static inline void world2screen(float x, float y, float cx, float cy, float s, int* sx, int* sy){
    *sx = (int)(cx + x*s);
    *sy = (int)(cy - y*s); // y 위쪽이 + (수학 좌표), 화면은 아래가 +
}

int viz_text(Viz* v, int x, int y, const char* utf8, int r,int g,int b);
void viz_arrow(Viz* v, float ox,float oy, float vx,float vy, float scale, float cx,float cy, int r,int g,int b, const char* label);
void viz_angle(Viz* v, float ax, float ay, float bx, float by, float radius, float scale, float cx,float cy, int r,int g,int b, char* out_deg_text, size_t out_len);

#endif
