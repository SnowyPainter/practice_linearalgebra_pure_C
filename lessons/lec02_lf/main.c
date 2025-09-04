#include <stdio.h>
#include "../../include/la.h"
#ifdef USE_SDL
#include "../../include/viz.h"
#include "../../include/color.h"
#endif

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const float a = 3.5;
const float b = -1.2;

vec2 linear(vec2 x) {
  const Matrix* a = matrix_from_array(2, 2, (double[]){1, 2, -1, 1.5});
  return matrix_transform_vec2(a, x);
}

vec2 affine(vec2 x) {
  const Matrix* a = matrix_from_array(2, 2, (double[]){1, 2, -1, 1.5});
  const vec2 b = v2(3, 5);
  return vadd(matrix_transform_vec2(a, x), b);
}

Matrix* lt(const Matrix* col, float scalar) {
  return matrix_scale(col, scalar);
}

int main(){  
  // 2차원 벡터에 대해 선형 변환 (선형함수의 a != 1)
  vec2 v = v2(1, 2);
  vec2 w = v2(-1, 3);

  vec2 lc = linear(
    vadd(vscale(v, a), vscale(w, b))
  );
  vec2 lc2 = vadd(vscale(linear(v), a), vscale(linear(w), b));
  
  printf("lc = %f, %f\n", lc.x, lc.y);
  printf("lc2 = %f, %f\n", lc2.x, lc2.y);

  const Matrix* a = matrix_from_array(3, 2, (double[]){
    1, 1,
    1, -1,
    2, 3
  });

  const Matrix* b = matrix_from_array(2, 1, (double[]){
    2,
    1
  });

  Matrix* multiplied = matrix_create(3, 1);
  for (int i = 0;i < 2;i++) {
    float scalar = matrix_get(b, i, 0);
    const Matrix* col = matrix_get_column(a, i);
    Matrix* c = lt(col, scalar);
    for(int j = 0;j < 3;j++) {
      matrix_set(multiplied, j, 0, matrix_get(multiplied, j, 0) + matrix_get(c, j, 0));
    }
  }
  matrix_print(multiplied, "(linear transform) multiplied ");
  matrix_print(matrix_multiply(a, b), "(matrix multiply) multiplied");

  matrix_free(multiplied);

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
      


      viz_present(&viz);
      SDL_Delay(16); // ~60fps
    }
    viz_close(&viz);
  }
#endif
  return 0;
}