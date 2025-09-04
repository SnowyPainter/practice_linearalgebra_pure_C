#include <stdio.h>
#include "../../include/la.h"
#ifdef USE_SDL
#include "../../include/viz.h"
#endif

int main(){
  vec2 v = v2(1,0.5), w = v2(-0.3,1), u = vadd(v, w);
  printf("u=(%.2f, %.2f)\n", u.x, u.y);

#ifdef USE_SDL
  Viz viz;
  if (viz_open(&viz, "template", 800, 600) == 0) {
    int running = 1;
    while (running) {
      SDL_Event e;
      while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = 0;
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) running = 0;
      }
      viz_clear(&viz, 245,245,245);
      viz_line(&viz,100,300,700,300, 200,200,200); // x-axis
      viz_line(&viz,400,50, 400,550, 200,200,200); // y-axis
      viz_present(&viz);
      SDL_Delay(16); // ~60fps
    }
    viz_close(&viz);
  }
#endif
  return 0;
}