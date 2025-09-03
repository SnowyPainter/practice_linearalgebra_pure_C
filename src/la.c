#include "la.h"
vec2 v2(double x,double y){ return (vec2){x,y}; }
vec2 vadd(vec2 a,vec2 b){ return v2(a.x+b.x, a.y+b.y); }
vec2 vscale(vec2 a,double s){ return v2(a.x*s, a.y*s); }
double vdot(vec2 a,vec2 b){ return a.x*b.x + a.y*b.y; }
mat2 m2(double a,double b,double c,double d){ mat2 M={.m={{a,b},{c,d}}}; return M; }
vec2 mmulv(mat2 M, vec2 v){
  return v2(M.m[0][0]*v.x + M.m[0][1]*v.y, M.m[1][0]*v.x + M.m[1][1]*v.y);
}