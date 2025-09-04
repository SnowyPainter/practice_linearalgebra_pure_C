// viz3d.h
#pragma once
#ifdef USE_SDL
#include "la.h"
#include "color.h"
#include "viz.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct {
    // 클리핑 간단히: w<=0 이면 뒤쪽 → 스킵
    int sx, sy;   // screen x,y
    double z;     // view-space or NDC z (정렬용)
    bool valid;
} Proj2D;

static inline Proj2D project_point(mat4 MVP, vec3 p, int w, int h){
    vec4 hp = {p.x, p.y, p.z, 1.0};
    vec4 cp = m4_mul_v4(MVP, hp);        // clip space
    Proj2D out = {0};
    if (cp.w <= 0.0001) { out.valid=false; return out; } // 뒤쪽은 버림(간이)
    double invw = 1.0/cp.w;
    double ndc_x = cp.x * invw;          // [-1,1]
    double ndc_y = cp.y * invw;
    double ndc_z = cp.z * invw;          // 필요시 깊이 사용

    // NDC -> screen (y down)
    out.sx = (int)((ndc_x * 0.5 + 0.5) * w);
    out.sy = (int)((-ndc_y* 0.5 + 0.5) * h);
    out.z  = ndc_z;
    out.valid = (ndc_x>=-1 && ndc_x<=1 && ndc_y>=-1 && ndc_y<=1); // 간단 바운드
    return out;
}

int viz3d_line(Viz* v, vec3 a, vec3 b, mat4 MVP, RGB color){
    Proj2D pa = project_point(MVP, a, v->w, v->h);
    Proj2D pb = project_point(MVP, b, v->w, v->h);
    if (!pa.valid && !pb.valid) return 0;
    SDL_SetRenderDrawColor(v->ren, color.r, color.g, color.b, 255);
    return SDL_RenderDrawLine(v->ren, pa.sx, pa.sy, pb.sx, pb.sy);
}

// 인덱스드 메쉬 와이어프레임
int viz3d_mesh_wire(Viz* v,
                    const vec3* vertices, int vcount,
                    const int* indices, int icount,
                    mat4 MVP, RGB color)
{
    SDL_SetRenderDrawColor(v->ren, color.r, color.g, color.b, 255);
    for (int i=0; i<icount; i+=3){
        int i0=indices[i], i1=indices[i+1], i2=indices[i+2];
        Proj2D p0=project_point(MVP, vertices[i0], v->w, v->h);
        Proj2D p1=project_point(MVP, vertices[i1], v->w, v->h);
        Proj2D p2=project_point(MVP, vertices[i2], v->w, v->h);
        if ((!p0.valid && !p1.valid && !p2.valid)) continue;

        // 간단 백페이스 컬링: 화면공간 면적 부호로
        double area = (p1.sx - p0.sx)*(p2.sy - p0.sy) - (p1.sy - p0.sy)*(p2.sx - p0.sx);
        if (area <= 0) continue; // 시계방향이면 스킵(오른손좌표 기준)

        SDL_RenderDrawLine(v->ren, p0.sx,p0.sy, p1.sx,p1.sy);
        SDL_RenderDrawLine(v->ren, p1.sx,p1.sy, p2.sx,p2.sy);
        SDL_RenderDrawLine(v->ren, p2.sx,p2.sy, p0.sx,p0.sy);
    }
    return 0;
}
#endif
