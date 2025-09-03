#pragma once
typedef struct { double x, y; } vec2;
typedef struct { double m[2][2]; } mat2;

vec2 v2(double x, double y);
vec2 vadd(vec2 a, vec2 b);
vec2 vscale(vec2 a, double s);
double vdot(vec2 a, vec2 b);
mat2 m2(double a, double b, double c, double d);
vec2 mmulv(mat2 M, vec2 v);