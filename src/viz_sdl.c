#ifdef USE_SDL
#include "viz.h"
#include "color.h"


int viz_open(Viz *v, const char *title, int w, int h) {
  v->w = w;
  v->h = h;
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("SDL_Init error: %s", SDL_GetError());
    return -1;
  }

  if (TTF_Init() != 0) { SDL_Log("TTF_Init: %s", TTF_GetError()); return -1; }
  v->font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 16);
  if (!v->font) { SDL_Log("TTF_OpenFont: %s", TTF_GetError()); return -1; }

  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");

  v->win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
  if (!v->win) {
    SDL_Log("CreateWindow error: %s", SDL_GetError());
    return -1;
  }

  // 1차: 가속 + vsync
  v->ren = SDL_CreateRenderer(
      v->win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!v->ren) {
    SDL_Log("ACCEL renderer failed: %s", SDL_GetError());
    // 2차: 소프트웨어 폴백
    v->ren = SDL_CreateRenderer(v->win, -1, SDL_RENDERER_SOFTWARE);
    if (!v->ren) {
      SDL_Log("SOFTWARE renderer failed: %s", SDL_GetError());
      return -1;
    }
  }

  v->font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 16);
  if (!v->font)
    return -1;

  // DPI/스케일 이슈 회피(논리 사이즈 고정)
  SDL_RenderSetLogicalSize(v->ren, w, h);

  // 드라이버/기능 정보 로그
  SDL_RendererInfo info;
  SDL_GetRendererInfo(v->ren, &info);
  SDL_Log("Renderer: %s (flags=0x%x)", info.name, info.flags);
  
  return 0;
}

void viz_clear(Viz *v, RGB color) {
  SDL_SetRenderDrawBlendMode(v->ren, SDL_BLENDMODE_NONE);
  SDL_SetRenderDrawColor(v->ren, (Uint8)color.r, (Uint8)color.g, (Uint8)color.b, 255);
  SDL_RenderClear(v->ren);
}

int viz_line(Viz *v, int x1, int y1, int x2, int y2, RGB color) {
  SDL_SetRenderDrawColor(v->ren, (Uint8)color.r, (Uint8)color.g, (Uint8)color.b, 255);

  // 1) 기본 라인
  if (SDL_RenderDrawLine(v->ren, x1, y1, x2, y2) == 0)
    return 0;

  SDL_Log("RenderDrawLine failed: %s. Fallback to DrawLines.", SDL_GetError());

  // 2) DrawLines로 재시도
  SDL_Point pts[2] = {{x1, y1}, {x2, y2}};
  if (SDL_RenderDrawLines(v->ren, pts, 2) == 0)
    return 0;

  SDL_Log("RenderDrawLines failed: %s. Fallback to Bresenham.", SDL_GetError());

  // 3) Bresenham 수동 포인트 드로우 (확정적으로 보이게)
  int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
  int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
  int err = dx + dy, e2;
  int x = x1, y = y1;

  for (;;) {
    SDL_RenderDrawPoint(v->ren, x, y);
    if (x == x2 && y == y2)
      break;
    e2 = 2 * err;
    if (e2 >= dy) {
      err += dy;
      x += sx;
    }
    if (e2 <= dx) {
      err += dx;
      y += sy;
    }
  }
  return 0;
}

void viz_present(Viz *v) { SDL_RenderPresent(v->ren); }

void viz_close(Viz *v) {
  if (v->font)
    TTF_CloseFont(v->font);
  if (v->ren)
    SDL_DestroyRenderer(v->ren);
  if (v->win)
    SDL_DestroyWindow(v->win);
  TTF_Quit();
  SDL_Quit();
}

int viz_text(Viz *v, int x, int y, const char *utf8, RGB color) {
  SDL_Color col = {(Uint8)color.r, (Uint8)color.g, (Uint8)color.b, 255};
  SDL_Surface *s = TTF_RenderUTF8_Blended(v->font, utf8, col);
  if (!s)
    return -1;
  SDL_Texture *t = SDL_CreateTextureFromSurface(v->ren, s);
  if (!t) {
    SDL_FreeSurface(s);
    return -1;
  }
  SDL_Rect dst = {x, y, s->w, s->h};
  SDL_RenderCopy(v->ren, t, NULL, &dst);
  SDL_DestroyTexture(t);
  SDL_FreeSurface(s);
  return 0;
}

void viz_arrow(Viz *v, float ox, float oy, float vx, float vy, float scale,
               float cx, float cy, RGB color, const char *label) {
  // 본체
  int x0, y0, x1, y1;
  world2screen(ox, oy, cx, cy, scale, &x0, &y0);
  world2screen(ox + vx, oy + vy, cx, cy, scale, &x1, &y1);
  viz_line(v, x0, y0, x1, y1, color);

  // 화살촉
  float len = sqrtf(vx * vx + vy * vy);
  if (len > 1e-6f) {
    float ux = vx / len, uy = vy / len;
    float ah = 12.0f;    // 픽셀 길이
    float theta = 0.35f; // 화살촉 각도(rad) ~20도
    // 두 날개 방향
    float rx = ux * cosf(theta) + uy * sinf(theta);
    float ry = -ux * sinf(theta) + uy * cosf(theta);
    float lx = ux * cosf(theta) - uy * sinf(theta);
    float ly = uy * cosf(theta) + ux * sinf(theta);
    int ax, ay, bx, by;
    world2screen(ox + vx - rx * (ah / scale), oy + vy - ry * (ah / scale), cx,
                 cy, scale, &ax, &ay);
    world2screen(ox + vx - lx * (ah / scale), oy + vy - ly * (ah / scale), cx,
                 cy, scale, &bx, &by);
    viz_line(v, x1, y1, ax, ay, color);
    viz_line(v, x1, y1, bx, by, color);
  }

  // 라벨
  if (label && *label) {
    int tx = x1 + 6, ty = y1 - 6;
    viz_text(v, tx, ty, label, color);
  }
}

void viz_angle(Viz *v, float ax, float ay, float bx, float by, float radius,
               float scale, float cx, float cy, RGB color,
               char *out_deg_text, size_t out_len) {
  float na = sqrtf(ax * ax + ay * ay), nb = sqrtf(bx * bx + by * by);
  if (na < 1e-6f || nb < 1e-6f)
    return;
  float cosv = (ax * bx + ay * by) / (na * nb);
  if (cosv > 1)
    cosv = 1;
  else if (cosv < -1)
    cosv = -1;
  float th = acosf(cosv); // 라디안
  if (out_deg_text && out_len) {
    float deg = th * 180.0f / 3.1415926535f;
    snprintf(out_deg_text, out_len, "%.1f°", deg);
  }

  // 시작/끝 각도
  float a0 = atan2f(ay, ax);
  float a1 = a0 + th;
  int segments = 48;
  int lastx = -1, lasty = -1;
  for (int i = 0; i <= segments; i++) {
    float t = (float)i / (float)segments;
    float ang = a0 + t * (a1 - a0);
    float px = cosf(ang) * radius;
    float py = sinf(ang) * radius;
    int sx, sy;
    world2screen(px, py, cx, cy, scale, &sx, &sy);
    if (i > 0)
      viz_line(v, lastx, lasty, sx, sy, color);
    lastx = sx;
    lasty = sy;
  }

  // 각도 텍스트 위치(중간각)
  float mid = (a0 + a1) * 0.5f;
  int tx, ty;
  world2screen(cosf(mid) * (radius + 0.2f), sinf(mid) * (radius + 0.2f), cx, cy,
               scale, &tx, &ty);
  if (out_deg_text && out_len)
    viz_text(v, tx, ty, out_deg_text, color);
}

#endif
