#pragma once

typedef struct {
    int r, g, b;
} RGB;

// 기본 색상 상수들
static const RGB COLOR_BLACK = {0, 0, 0};
static const RGB COLOR_WHITE = {255, 255, 255};
static const RGB COLOR_RED = {255, 0, 0};
static const RGB COLOR_GREEN = {0, 255, 0};
static const RGB COLOR_BLUE = {0, 0, 255};
static const RGB COLOR_YELLOW = {255, 255, 0};
static const RGB COLOR_CYAN = {0, 255, 255};
static const RGB COLOR_MAGENTA = {255, 0, 255};
static const RGB COLOR_GRAY = {128, 128, 128};
static const RGB COLOR_DARK_GRAY = {64, 64, 64};
static const RGB COLOR_LIGHT_GRAY = {192, 192, 192};
static const RGB COLOR_ORANGE = {255, 165, 0};
static const RGB COLOR_PURPLE = {128, 0, 128};
static const RGB COLOR_PINK = {255, 192, 203};

// 유틸리티 함수들
static inline RGB rgb(int r, int g, int b) {
    RGB color = {r, g, b};
    return color;
}

static inline RGB rgb_hex(int hex) {
    RGB color = {(hex >> 16) & 0xFF, (hex >> 8) & 0xFF, hex & 0xFF};
    return color;
}

// 밝기 조절 함수들
static inline RGB rgb_darken(RGB color, float factor) {
    RGB result = {
        (int)(color.r * (1.0f - factor)),
        (int)(color.g * (1.0f - factor)),
        (int)(color.b * (1.0f - factor))
    };
    return result;
}

static inline RGB rgb_lighten(RGB color, float factor) {
    RGB result = {
        (int)(color.r + (255 - color.r) * factor),
        (int)(color.g + (255 - color.g) * factor),
        (int)(color.b + (255 - color.b) * factor)
    };
    return result;
}

// 알파 블렌딩 (미래 확장용)
static inline RGB rgb_alpha(RGB color, float alpha) {
    RGB result = {
        (int)(color.r * alpha),
        (int)(color.g * alpha),
        (int)(color.b * alpha)
    };
    return result;
}
