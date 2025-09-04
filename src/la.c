#include "la.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define M_PI if not defined (for portability)
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Vector operations
vec2 v2(double x,double y){ return (vec2){x,y}; }
vec2 vadd(vec2 a,vec2 b){ return v2(a.x+b.x, a.y+b.y); }
vec2 vscale(vec2 a,double s){ return v2(a.x*s, a.y*s); }
double vdot(vec2 a,vec2 b){ return a.x*b.x + a.y*b.y; }
double vlen(vec2 v){ return sqrt(v.x*v.x + v.y*v.y); }
vec2 vnorm(vec2 v){
  double len = vlen(v);
  if (len < 1e-9) return v2(0, 0);
  return vscale(v, 1.0/len);
}

// Matrix operations
mat2 m2(double a,double b,double c,double d){ mat2 M={.m={{a,b},{c,d}}}; return M; }
vec2 mmulv(mat2 M, vec2 v){
  return v2(M.m[0][0]*v.x + M.m[0][1]*v.y, M.m[1][0]*v.x + M.m[1][1]*v.y);
}

// Matrix arithmetic
mat2 madd(mat2 A, mat2 B){
  return m2(A.m[0][0]+B.m[0][0], A.m[0][1]+B.m[0][1],
            A.m[1][0]+B.m[1][0], A.m[1][1]+B.m[1][1]);
}

mat2 mmul(mat2 A, mat2 B){
  return m2(A.m[0][0]*B.m[0][0] + A.m[0][1]*B.m[1][0],
            A.m[0][0]*B.m[0][1] + A.m[0][1]*B.m[1][1],
            A.m[1][0]*B.m[0][0] + A.m[1][1]*B.m[1][0],
            A.m[1][0]*B.m[0][1] + A.m[1][1]*B.m[1][1]);
}

mat2 mscale(mat2 M, double s){
  return m2(M.m[0][0]*s, M.m[0][1]*s, M.m[1][0]*s, M.m[1][1]*s);
}

mat2 mtranspose(mat2 M){
  return m2(M.m[0][0], M.m[1][0], M.m[0][1], M.m[1][1]);
}

// Matrix transformations
mat2 meye(void){
  return m2(1, 0, 0, 1);
}

mat2 mrot(double angle){
  double c = cos(angle), s = sin(angle);
  return m2(c, -s, s, c);
}

mat2 mscale_matrix(double sx, double sy){
  return m2(sx, 0, 0, sy);
}

// Matrix properties
double mdet(mat2 M){
  return M.m[0][0]*M.m[1][1] - M.m[0][1]*M.m[1][0];
}

mat2 minv(mat2 M){
  double det = mdet(M);
  if (fabs(det) < 1e-9) {
    // Return identity matrix if singular
    return meye();
  }
  return m2(M.m[1][1]/det, -M.m[0][1]/det,
            -M.m[1][0]/det, M.m[0][0]/det);
}

bool mequal(mat2 A, mat2 B){
  const double eps = 1e-9;
  return fabs(A.m[0][0]-B.m[0][0]) < eps &&
         fabs(A.m[0][1]-B.m[0][1]) < eps &&
         fabs(A.m[1][0]-B.m[1][0]) < eps &&
         fabs(A.m[1][1]-B.m[1][1]) < eps;
}

// Utilities
void mprint(mat2 M, const char* name){
  if (name) printf("%s = \n", name);
  printf("[%.3f, %.3f]\n", M.m[0][0], M.m[0][1]);
  printf("[%.3f, %.3f]\n", M.m[1][0], M.m[1][1]);
  printf("\n");
}

// ============================================================================
// Dynamic Matrix Implementation
// ============================================================================

// Core functions
Matrix* matrix_create(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return NULL;

    Matrix* m = (Matrix*)malloc(sizeof(Matrix));
    if (!m) return NULL;

    m->rows = rows;
    m->cols = cols;
    m->data = (double*)calloc(rows * cols, sizeof(double));

    if (!m->data) {
        free(m);
        return NULL;
    }

    return m;
}

void matrix_free(Matrix* m) {
    if (m) {
        free(m->data);
        free(m);
    }
}

Matrix* matrix_copy(const Matrix* m) {
    if (!m) return NULL;

    Matrix* copy = matrix_create(m->rows, m->cols);
    if (!copy) return NULL;

    memcpy(copy->data, m->data, m->rows * m->cols * sizeof(double));
    return copy;
}

void matrix_set(Matrix* m, int row, int col, double value) {
    if (!m || row < 0 || row >= m->rows || col < 0 || col >= m->cols) return;
    m->data[row * m->cols + col] = value;
}

double matrix_get(const Matrix* m, int row, int col) {
    if (!m || row < 0 || row >= m->rows || col < 0 || col >= m->cols) return 0.0;
    return m->data[row * m->cols + col];
}

void matrix_print(const Matrix* m, const char* name) {
    if (!m) {
        printf("NULL matrix\n");
        return;
    }

    if (name) printf("%s =\n", name);

    for (int i = 0; i < m->rows; i++) {
        printf("[");
        for (int j = 0; j < m->cols; j++) {
            printf("%.3f", matrix_get(m, i, j));
            if (j < m->cols - 1) printf(", ");
        }
        printf("]\n");
    }
    printf("\n");
}

// Matrix arithmetic
Matrix* matrix_add(const Matrix* A, const Matrix* B) {
    if (!A || !B || A->rows != B->rows || A->cols != B->cols) return NULL;

    Matrix* result = matrix_create(A->rows, A->cols);
    if (!result) return NULL;

    for (int i = 0; i < A->rows * A->cols; i++) {
        result->data[i] = A->data[i] + B->data[i];
    }
    return result;
}

Matrix* matrix_subtract(const Matrix* A, const Matrix* B) {
    if (!A || !B || A->rows != B->rows || A->cols != B->cols) return NULL;

    Matrix* result = matrix_create(A->rows, A->cols);
    if (!result) return NULL;

    for (int i = 0; i < A->rows * A->cols; i++) {
        result->data[i] = A->data[i] - B->data[i];
    }
    return result;
}

Matrix* matrix_multiply(const Matrix* A, const Matrix* B) {
    if (!A || !B || A->cols != B->rows) return NULL;

    Matrix* result = matrix_create(A->rows, B->cols);
    if (!result) return NULL;

    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < B->cols; j++) {
            double sum = 0.0;
            for (int k = 0; k < A->cols; k++) {
                sum += matrix_get(A, i, k) * matrix_get(B, k, j);
            }
            matrix_set(result, i, j, sum);
        }
    }
    return result;
}

Matrix* matrix_scale(const Matrix* m, double scalar) {
    if (!m) return NULL;

    Matrix* result = matrix_copy(m);
    if (!result) return NULL;

    for (int i = 0; i < m->rows * m->cols; i++) {
        result->data[i] *= scalar;
    }
    return result;
}

Matrix* matrix_transpose(const Matrix* m) {
    if (!m) return NULL;

    Matrix* result = matrix_create(m->cols, m->rows);
    if (!result) return NULL;

    for (int i = 0; i < m->rows; i++) {
        for (int j = 0; j < m->cols; j++) {
            matrix_set(result, j, i, matrix_get(m, i, j));
        }
    }
    return result;
}

// Matrix transformations
Matrix* matrix_identity(int size) {
    Matrix* result = matrix_create(size, size);
    if (!result) return NULL;

    for (int i = 0; i < size; i++) {
        matrix_set(result, i, i, 1.0);
    }
    return result;
}

Matrix* matrix_rotation_2d(double angle) {
    Matrix* result = matrix_create(2, 2);
    if (!result) return NULL;

    double c = cos(angle), s = sin(angle);
    matrix_set(result, 0, 0, c);
    matrix_set(result, 0, 1, -s);
    matrix_set(result, 1, 0, s);
    matrix_set(result, 1, 1, c);

    return result;
}

Matrix* matrix_scaling_2d(double sx, double sy) {
    Matrix* result = matrix_create(2, 2);
    if (!result) return NULL;

    matrix_set(result, 0, 0, sx);
    matrix_set(result, 1, 1, sy);

    return result;
}

Matrix* matrix_shear_2d(double kx, double ky) {
    Matrix* result = matrix_create(2, 2);
    if (!result) return NULL;

    matrix_set(result, 0, 0, 1.0);
    matrix_set(result, 0, 1, kx);
    matrix_set(result, 1, 0, ky);
    matrix_set(result, 1, 1, 1.0);

    return result;
}

// Matrix properties
double matrix_determinant(const Matrix* m) {
    if (!m || m->rows != m->cols) return 0.0; // Must be square matrix

    int n = m->rows;

    if (n == 1) {
        return matrix_get(m, 0, 0);
    } else if (n == 2) {
        return matrix_get(m, 0, 0) * matrix_get(m, 1, 1) -
               matrix_get(m, 0, 1) * matrix_get(m, 1, 0);
    } else if (n == 3) {
        // Using Sarrus rule for 3x3
        double a = matrix_get(m, 0, 0), b = matrix_get(m, 0, 1), c = matrix_get(m, 0, 2);
        double d = matrix_get(m, 1, 0), e = matrix_get(m, 1, 1), f = matrix_get(m, 1, 2);
        double g = matrix_get(m, 2, 0), h = matrix_get(m, 2, 1), i = matrix_get(m, 2, 2);

        return a*(e*i - f*h) - b*(d*i - f*g) + c*(d*h - e*g);
    }

    // For larger matrices, we'd need more sophisticated algorithms
    // For now, return 0 as unsupported
    return 0.0;
}

Matrix* matrix_inverse(const Matrix* m) {
    if (!m || m->rows != m->cols) return NULL;

    double det = matrix_determinant(m);
    if (fabs(det) < 1e-12) return NULL; // Singular matrix

    int n = m->rows;
    Matrix* result = matrix_create(n, n);
    if (!result) return NULL;

    if (n == 2) {
        // 2x2 inverse formula
        double a = matrix_get(m, 0, 0), b = matrix_get(m, 0, 1);
        double c = matrix_get(m, 1, 0), d = matrix_get(m, 1, 1);

        matrix_set(result, 0, 0, d / det);
        matrix_set(result, 0, 1, -b / det);
        matrix_set(result, 1, 0, -c / det);
        matrix_set(result, 1, 1, a / det);
    } else if (n == 3) {
        // 3x3 inverse using adjugate matrix
        // This is complex, so for now we'll return NULL
        matrix_free(result);
        return NULL;
    } else {
        // For larger matrices, we'd need Gaussian elimination
        matrix_free(result);
        return NULL;
    }

    return result;
}

double matrix_trace(const Matrix* m) {
    if (!m || m->rows != m->cols) return 0.0;

    double trace = 0.0;
    for (int i = 0; i < m->rows; i++) {
        trace += matrix_get(m, i, i);
    }
    return trace;
}

bool matrix_equal(const Matrix* A, const Matrix* B) {
    if (!A || !B) return false;
    if (A->rows != B->rows || A->cols != B->cols) return false;

    const double eps = 1e-9;
    for (int i = 0; i < A->rows * A->cols; i++) {
        if (fabs(A->data[i] - B->data[i]) > eps) return false;
    }
    return true;
}

// Matrix-vector operations
vec2 matrix_transform_vec2(const Matrix* m, vec2 v) {
    if (!m || m->rows != 2 || m->cols != 2) {
        return v; // Return unchanged if not 2x2
    }

    vec2 result;
    result.x = matrix_get(m, 0, 0) * v.x + matrix_get(m, 0, 1) * v.y;
    result.y = matrix_get(m, 1, 0) * v.x + matrix_get(m, 1, 1) * v.y;
    return result;
}

// Utility functions
Matrix* matrix_from_array(int rows, int cols, const double* data) {
    if (rows <= 0 || cols <= 0 || !data) return NULL;

    Matrix* result = matrix_create(rows, cols);
    if (!result) return NULL;

    memcpy(result->data, data, rows * cols * sizeof(double));
    return result;
}

void matrix_to_array(const Matrix* m, double* out_data) {
    if (!m || !out_data) return;
    memcpy(out_data, m->data, m->rows * m->cols * sizeof(double));
}

// Special matrices
Matrix* matrix_zeros(int rows, int cols) {
    return matrix_create(rows, cols); // calloc already zeros the memory
}

Matrix* matrix_ones(int rows, int cols) {
    Matrix* result = matrix_create(rows, cols);
    if (!result) return NULL;

    for (int i = 0; i < rows * cols; i++) {
        result->data[i] = 1.0;
    }
    return result;
}