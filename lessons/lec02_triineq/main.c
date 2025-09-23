#include <stdio.h>
#include "../../include/la.h"
#ifdef USE_SDL
#include "../../include/viz.h"
#include "../../include/color.h"
#endif

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int SCALE = 20;

int main(){
  
  vec2 a = v2(3, 3);
  vec2 b = v2(3, -3);
  vec2 c = vadd(a, b);

#ifdef USE_SDL
  Viz viz;
  
  if (viz_open(&viz, "vector", WINDOW_WIDTH, WINDOW_HEIGHT) == 0) {
    int running = 1;
    while (running) {
      SDL_Event e;
      while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = 0;
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) running = 0;
      }
      viz_clear(&viz, rgb(245,245,245));
      viz_line(&viz,100,300,700,300, rgb(200,200,200));
      viz_line(&viz,400,50, 400,550, rgb(200,200,200));
      
      viz_arrow(&viz, 0,0, a.x, a.y, SCALE, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f, rgb(30,30,30), "a");
      viz_arrow(&viz, a.x,a.y, b.x, b.y, SCALE, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f, rgb(30,30,30), "b");
      viz_arrow(&viz, 0,0, c.x, c.y, SCALE, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f, rgb(30,30,30), "c");
      
      viz_present(&viz);
      SDL_Delay(16); // ~60fps
    }
    viz_close(&viz);
  }
#endif
  return 0;
}