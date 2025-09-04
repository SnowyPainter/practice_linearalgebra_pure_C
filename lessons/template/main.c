#include <stdio.h>
#include "../../include/la.h"
#ifdef USE_SDL
#include "../../include/viz.h"
#include "../../include/color.h"
#endif

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main(){
  vec2 v = v2(1,0.5), w = v2(-0.3,1), u = vadd(v, w);
  float scale = 120.0f;         // 1 단위 = 120px
  float cx = WINDOW_WIDTH / 2.0f, cy = WINDOW_HEIGHT / 2.0f;
  printf("u=(%.2f, %.2f)\n", u.x, u.y);

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
      viz_arrow(&viz, 0,0, v.x, v.y, scale, cx,cy, rgb(30,30,30), "v");
      viz_arrow(&viz, 0,0, w.x, w.y, scale, cx,cy, rgb(30,30,30), "w");
      char deg[32]={0};
      viz_angle(&viz, v.x, v.y, w.x, w.y, 0.6f, scale, cx,cy, rgb(60,60,200), deg, sizeof(deg));
      float dot = v.x*w.x + v.y*w.y;
      float nv = sqrtf(v.x*v.x+v.y*v.y), nw = sqrtf(w.x*w.x+w.y*w.y);
      float cos_sim = (nv>1e-6f && nw>1e-6f)? dot/(nv*nw) : 0.f;

      char info[128];
      snprintf(info,sizeof(info),"v·w=%.3f, cosθ=%.3f (%s)", dot, cos_sim, deg);
      viz_text(&viz, 12, 12, info, rgb(20,20,20));

      viz_present(&viz);
      SDL_Delay(16); // ~60fps
    }
    viz_close(&viz);
  }
#endif
  return 0;
}