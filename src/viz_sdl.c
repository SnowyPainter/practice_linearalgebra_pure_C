#ifdef USE_SDL
#include "viz.h"
int viz_open(Viz* v,const char* title,int w,int h){
  v->w=w; v->h=h;
  if(SDL_Init(SDL_INIT_VIDEO)) return -1;
  v->win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w,h,0);
  v->ren = SDL_CreateRenderer(v->win,-1,SDL_RENDERER_ACCELERATED);
  return (v->win && v->ren) ? 0 : -1;
}
void viz_clear(Viz* v,int r,int g,int b){ SDL_SetRenderDrawColor(v->ren,r,g,b,255); SDL_RenderClear(v->ren); }
void viz_line(Viz* v,int x1,int y1,int x2,int y2,int r,int g,int b){
  SDL_SetRenderDrawColor(v->ren,r,g,b,255); SDL_RenderDrawLine(v->ren,x1,y1,x2,y2);
}
void viz_present(Viz* v){ SDL_RenderPresent(v->ren); }
void viz_close(Viz* v){ SDL_DestroyRenderer(v->ren); SDL_DestroyWindow(v->win); SDL_Quit(); }
#endif