#include <math.h>
#include "../../include/la.h"
#include "../../include/viz.h"
#include "../../include/viz3d.h"

int main(){
#ifdef USE_SDL
    Viz v={0};
    if (viz_open(&v, "3D Wireframe Cube", 960, 720) != 0) return 1;

    // 정점/인덱스
    vec3 verts[] = {
        {-1,-1,-1},{ 1,-1,-1},{ 1, 1,-1},{-1, 1,-1}, // back
        {-1,-1, 1},{ 1,-1, 1},{ 1, 1, 1},{-1, 1, 1}, // front
    };
    int tris[] = {
        0,1,2, 0,2,3,   // back
        4,6,5, 4,7,6,   // front
        0,4,5, 0,5,1,   // bottom
        3,2,6, 3,6,7,   // top
        1,5,6, 1,6,2,   // right
        0,3,7, 0,7,4    // left
    };

    double angle=0;
    for(;;){
        SDL_Event e; bool quit=false;
        while(SDL_PollEvent(&e)){
            if (e.type==SDL_QUIT) quit=true;
        }
        if (quit) break;

        viz_clear(&v, (RGB){20,20,24});

        // 카메라/프로젝션
        vec3 eye = v3(0,0,4), target = v3(0,0,0), up=v3(0,1,0);
        mat4 V = m4_look_at(eye, target, up);
        double aspect = (double)v.w / (double)v.h;
        mat4 P = m4_perspective(M_PI/3.0, aspect, 0.1, 100.0);

        // 모델: 회전 + 스케일
        angle += 0.01;
        mat4 R = m4_mul(m4_rot_y(angle), m4_rot_x(angle*0.7));
        mat4 S = m4_scale(0.8,0.8,0.8);
        mat4 T = m4_identity();
        mat4 M = m4_mul(T, m4_mul(R, S));

        mat4 MVP = m4_mul(P, m4_mul(V, M));

        viz3d_mesh_wire(&v, verts, 8, tris, sizeof(tris)/sizeof(tris[0]), MVP, (RGB){180,220,255});
        viz_present(&v);
        SDL_Delay(16);
    }
    viz_close(&v);
#endif
    return 0;
}
