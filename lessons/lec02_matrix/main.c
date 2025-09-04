#include <stdio.h>
#include "../../include/la.h"
#ifdef USE_SDL
#include "../../include/viz.h"
#include "../../include/color.h"
#endif

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main(){
  printf("=== Dynamic Matrix Operations Demo ===\n\n");

  // Create various size matrices
  Matrix* A = matrix_create(3, 3);
  Matrix* B = matrix_create(3, 3);
  Matrix* C = matrix_create(2, 3);

  if (!A || !B || !C) {
    printf("Failed to create matrices\n");
    return 1;
  }

  // Initialize matrices
  double data_A[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  double data_B[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
  double data_C[] = {1, 0, 2, 1, 1, 0};

  // Note: We're using A2 for the main operations, A, B, C are just for demo
  // In a real program, you'd assign the result: A = matrix_from_array(3, 3, data_A);

  // Or use matrix_set for individual elements
  Matrix* A2 = matrix_create(3, 3);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      matrix_set(A2, i, j, (i+1) * (j+1));
    }
  }

  printf("Matrix A (3x3):\n");
  matrix_print(A2, "A");

  printf("\nMatrix B (3x3):\n");
  matrix_print(B, "B");

  // Matrix operations
  Matrix* sum = matrix_add(A2, B);
  Matrix* diff = matrix_subtract(A2, B);
  Matrix* scaled = matrix_scale(A2, 2.0);
  Matrix* transposed = matrix_transpose(A2);

  printf("\nMatrix Operations:\n");
  matrix_print(sum, "A + B");
  matrix_print(diff, "A - B");
  matrix_print(scaled, "2 * A");
  matrix_print(transposed, "A^T");

  // Matrix properties
  printf("\nMatrix Properties:\n");
  printf("Trace of A: %.1f\n", matrix_trace(A2));
  printf("Determinant of A: %.3f\n", matrix_determinant(A2));

  // Special matrices
  Matrix* identity = matrix_identity(3);
  Matrix* zeros = matrix_zeros(2, 2);
  Matrix* ones = matrix_ones(2, 2);

  printf("\nSpecial Matrices:\n");
  matrix_print(identity, "3x3 Identity");
  matrix_print(zeros, "2x2 Zeros");
  matrix_print(ones, "2x2 Ones");

  // Transformation matrices
  Matrix* rotation = matrix_rotation_2d(M_PI/4);
  Matrix* scaling = matrix_scaling_2d(1.5, 0.8);
  Matrix* shear = matrix_shear_2d(0.3, 0.2);

  printf("\nTransformation Matrices:\n");
  matrix_print(rotation, "Rotation 45°");
  matrix_print(scaling, "Scaling (1.5x, 0.8x)");
  matrix_print(shear, "Shear (0.3, 0.2)");

  // Vector transformations
  vec2 v = v2(1, 0.5);
  vec2 v_rotated = matrix_transform_vec2(rotation, v);
  vec2 v_scaled = matrix_transform_vec2(scaling, v);

  printf("\nVector Transformations:\n");
  printf("Original vector: (%.3f, %.3f)\n", v.x, v.y);
  printf("Rotated vector: (%.3f, %.3f)\n", v_rotated.x, v_rotated.y);
  printf("Scaled vector: (%.3f, %.3f)\n", v_scaled.x, v_scaled.y);

  // Matrix multiplication
  Matrix* combined = matrix_multiply(rotation, scaling);
  printf("\nMatrix Multiplication:\n");
  matrix_print(combined, "Rotation * Scaling");

  // Clean up
  matrix_free(A);
  matrix_free(A2);
  matrix_free(B);
  matrix_free(C);
  matrix_free(sum);
  matrix_free(diff);
  matrix_free(scaled);
  matrix_free(transposed);
  matrix_free(identity);
  matrix_free(zeros);
  matrix_free(ones);
  matrix_free(rotation);
  matrix_free(scaling);
  matrix_free(shear);
  matrix_free(combined);

  printf("\n=== Demo Complete ===\n");

  // Visual demo with SDL
  float scale = 120.0f;
  float cx = WINDOW_WIDTH / 2.0f, cy = WINDOW_HEIGHT / 2.0f;

#ifdef USE_SDL
  Viz viz;

  if (viz_open(&viz, "dynamic matrix demo", WINDOW_WIDTH, WINDOW_HEIGHT) == 0) {
    int running = 1;
    while (running) {
      SDL_Event e;
      while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = 0;
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) running = 0;
      }

      viz_clear(&viz, rgb(245,245,245));

      // Grid lines
      viz_line(&viz,100,300,700,300, rgb(200,200,200));
      viz_line(&viz,400,50, 400,550, rgb(200,200,200));

      // Vectors
      viz_arrow(&viz, 0,0, v.x, v.y, scale, cx,cy, COLOR_BLACK, "original");
      viz_arrow(&viz, 0,0, v_rotated.x, v_rotated.y, scale, cx,cy, COLOR_BLUE, "rotated");
      viz_arrow(&viz, 0,0, v_scaled.x, v_scaled.y, scale, cx,cy, COLOR_GREEN, "scaled");

      // Info text
      char info[512];
      snprintf(info, sizeof(info),
               "Dynamic Matrix Demo\n"
               "Black: Original (%.1f, %.1f)\n"
               "Blue: Rotated 45°\n"
               "Green: Scaled (1.5x, 0.8x)\n"
               "\n"
               "Console shows full matrix operations!",
               v.x, v.y);
      viz_text(&viz, 12, 12, info, rgb(20,20,20));

      viz_present(&viz);
      SDL_Delay(16);
    }
    viz_close(&viz);
  }
#endif

  return 0;
}