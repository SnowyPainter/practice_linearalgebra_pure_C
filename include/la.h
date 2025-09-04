#pragma once
#include <stdbool.h>
#include <math.h>

typedef struct { double x, y; } vec2;
typedef struct { double m[2][2]; } mat2;

// Dynamic matrix structure
typedef struct {
    int rows, cols;
    double* data;  // Row-major order
} Matrix;

// Vector operations
vec2 v2(double x, double y);
vec2 vadd(vec2 a, vec2 b);
vec2 vscale(vec2 a, double s);
double vdot(vec2 a, vec2 b);
double vlen(vec2 v);
vec2 vnorm(vec2 v);

// Matrix operations
mat2 m2(double a, double b, double c, double d);
vec2 mmulv(mat2 M, vec2 v);

// Matrix arithmetic
mat2 madd(mat2 A, mat2 B);
mat2 mmul(mat2 A, mat2 B);
mat2 mscale(mat2 M, double s);
mat2 mtranspose(mat2 M);

// Matrix transformations
mat2 meye(void);                    // Identity matrix
mat2 mrot(double angle);            // Rotation matrix
mat2 mscale_matrix(double sx, double sy); // Scaling matrix

// Matrix properties
double mdet(mat2 M);                // Determinant
mat2 minv(mat2 M);                  // Inverse matrix
bool mequal(mat2 A, mat2 B);        // Matrix equality

// Utilities
void mprint(mat2 M, const char* name); // Print matrix

// Dynamic Matrix operations
Matrix* matrix_create(int rows, int cols);
void matrix_free(Matrix* m);
Matrix* matrix_copy(const Matrix* m);
void matrix_set(Matrix* m, int row, int col, double value);
double matrix_get(const Matrix* m, int row, int col);
void matrix_print(const Matrix* m, const char* name);
Matrix* matrix_get_column(const Matrix* m, int col);
Matrix* matrix_get_row(const Matrix* m, int row);
// Matrix arithmetic (dynamic)
Matrix* matrix_add(const Matrix* A, const Matrix* B);
Matrix* matrix_subtract(const Matrix* A, const Matrix* B);
Matrix* matrix_multiply(const Matrix* A, const Matrix* B);
Matrix* matrix_scale(const Matrix* m, double scalar);
Matrix* matrix_transpose(const Matrix* m);

// Matrix transformations (dynamic)
Matrix* matrix_identity(int size);
Matrix* matrix_rotation_2d(double angle);
Matrix* matrix_scaling_2d(double sx, double sy);
Matrix* matrix_shear_2d(double kx, double ky);

// Matrix properties (dynamic)
double matrix_determinant(const Matrix* m);
Matrix* matrix_inverse(const Matrix* m);
double matrix_trace(const Matrix* m);
bool matrix_equal(const Matrix* A, const Matrix* B);

// Matrix-vector operations
vec2 matrix_transform_vec2(const Matrix* m, vec2 v);

// Utility functions
Matrix* matrix_from_array(int rows, int cols, const double* data);
void matrix_to_array(const Matrix* m, double* out_data);

// Special matrices
Matrix* matrix_zeros(int rows, int cols);
Matrix* matrix_ones(int rows, int cols);

// --- 3D Types ---
typedef struct { double x, y, z; } vec3;
typedef struct { double x, y, z, w; } vec4;
typedef struct { double m[4][4]; } mat4;

// --- vec3 ops ---
static inline vec3 v3(double x,double y,double z){ return (vec3){x,y,z}; }
static inline vec3 v3_add(vec3 a, vec3 b){ return v3(a.x+b.x, a.y+b.y, a.z+b.z); }
static inline vec3 v3_sub(vec3 a, vec3 b){ return v3(a.x-b.x, a.y-b.y, a.z-b.z); }
static inline vec3 v3_scale(vec3 a, double s){ return v3(a.x*s, a.y*s, a.z*s); }
static inline double v3_dot(vec3 a, vec3 b){ return a.x*b.x + a.y*b.y + a.z*b.z; }
static inline vec3 v3_cross(vec3 a, vec3 b){
    return v3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}
static inline double v3_len(vec3 a){ return sqrt(v3_dot(a,a)); }
static inline vec3 v3_norm(vec3 a){ double L=v3_len(a); return L>0? v3_scale(a,1.0/L):a; }

// --- mat4 helpers ---
static inline mat4 m4_identity(void){
    mat4 M = { .m={{1,0,0,0},
                   {0,1,0,0},
                   {0,0,1,0},
                   {0,0,0,1}} };
    return M;
}
static inline mat4 m4_mul(mat4 A, mat4 B){
    mat4 R = {0};
    for(int i=0;i<4;i++) for(int j=0;j<4;j++){
        double s=0; for(int k=0;k<4;k++) s += A.m[i][k]*B.m[k][j];
        R.m[i][j]=s;
    }
    return R;
}
static inline vec4 m4_mul_v4(mat4 M, vec4 v){
    vec4 r;
    r.x = M.m[0][0]*v.x + M.m[0][1]*v.y + M.m[0][2]*v.z + M.m[0][3]*v.w;
    r.y = M.m[1][0]*v.x + M.m[1][1]*v.y + M.m[1][2]*v.z + M.m[1][3]*v.w;
    r.z = M.m[2][0]*v.x + M.m[2][1]*v.y + M.m[2][2]*v.z + M.m[2][3]*v.w;
    r.w = M.m[3][0]*v.x + M.m[3][1]*v.y + M.m[3][2]*v.z + M.m[3][3]*v.w;
    return r;
}

// --- transforms ---
static inline mat4 m4_translate(double tx,double ty,double tz){
    mat4 M = m4_identity();
    M.m[0][3]=tx; M.m[1][3]=ty; M.m[2][3]=tz;
    return M;
}
static inline mat4 m4_scale(double sx,double sy,double sz){
    mat4 M = { .m={{sx,0,0,0},{0,sy,0,0},{0,0,sz,0},{0,0,0,1}} };
    return M;
}
static inline mat4 m4_rot_x(double a){
    double c=cos(a), s=sin(a);
    mat4 M=m4_identity();
    M.m[1][1]=c; M.m[1][2]=-s; M.m[2][1]=s; M.m[2][2]=c;
    return M;
}
static inline mat4 m4_rot_y(double a){
    double c=cos(a), s=sin(a);
    mat4 M=m4_identity();
    M.m[0][0]=c; M.m[0][2]=s; M.m[2][0]=-s; M.m[2][2]=c;
    return M;
}
static inline mat4 m4_rot_z(double a){
    double c=cos(a), s=sin(a);
    mat4 M=m4_identity();
    M.m[0][0]=c; M.m[0][1]=-s; M.m[1][0]=s; M.m[1][1]=c;
    return M;
}

// --- camera/projection ---
static inline mat4 m4_look_at(vec3 eye, vec3 target, vec3 up){
    vec3 z = v3_norm(v3_sub(eye, target));    // camera forward (right-handed)
    vec3 x = v3_norm(v3_cross(up, z));        // right
    vec3 y = v3_cross(z, x);                  // up
    mat4 R = m4_identity();
    R.m[0][0]=x.x; R.m[0][1]=x.y; R.m[0][2]=x.z; R.m[0][3]=-v3_dot(x,eye);
    R.m[1][0]=y.x; R.m[1][1]=y.y; R.m[1][2]=y.z; R.m[1][3]=-v3_dot(y,eye);
    R.m[2][0]=z.x; R.m[2][1]=z.y; R.m[2][2]=z.z; R.m[2][3]=-v3_dot(z,eye);
    return R;
}
// standard perspective (right-handed, NDC z in [-1,1])
static inline mat4 m4_perspective(double fov_y_rad,double aspect,double znear,double zfar){
    double f = 1.0 / tan(fov_y_rad*0.5);
    mat4 P = {0};
    P.m[0][0]=f/aspect;
    P.m[1][1]=f;
    P.m[2][2]=(zfar+znear)/(znear - zfar);
    P.m[2][3]=(2*zfar*znear)/(znear - zfar);
    P.m[3][2]=-1.0;
    return P;
}
