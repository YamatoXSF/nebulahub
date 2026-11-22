#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

// SDL init flags
#define SDL_INIT_VIDEO  0x00000020u
#define SDL_INIT_AUDIO  0x00000010u
#define SDL_INIT_EVENTS 0x00004000u

// SDL window flags
#define SDL_WINDOW_FULLSCREEN       0x00000001u
#define SDL_WINDOW_OPENGL           0x00000002u
#define SDL_WINDOW_SHOWN            0x00000004u
#define SDL_WINDOW_RESIZABLE        0x00000020u

// SDL renderer flags
#define SDL_RENDERER_ACCELERATED    0x00000002u
#define SDL_RENDERER_PRESENTVSYNC   0x00000004u

// SDL blend modes
#define SDL_BLENDMODE_NONE  0x00000000
#define SDL_BLENDMODE_BLEND 0x00000001
#define SDL_BLENDMODE_ADD   0x00000002
#define SDL_BLENDMODE_MOD   0x00000004

// Window positioning
#define SDL_WINDOWPOS_CENTERED  0x2FFF0000u
#define SDL_WINDOWPOS_UNDEFINED 0x1FFF0000u

// Event types
#define SDL_QUIT        0x100
#define SDL_KEYDOWN     0x300
#define SDL_KEYUP       0x301

typedef struct SDL_Window   SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct SDL_Texture  SDL_Texture;
typedef struct SDL_Surface  SDL_Surface;
typedef struct SDL_RWops    SDL_RWops;

typedef int SDL_bool;
#define SDL_FALSE 0
#define SDL_TRUE  1

typedef struct SDL_Rect {
    int x, y, w, h;
} SDL_Rect;

typedef struct SDL_Point {
    int x, y;
} SDL_Point;

typedef struct SDL_Color {
    uint8_t r, g, b, a;
} SDL_Color;

typedef struct SDL_Event {
    uint32_t type;
    uint8_t  padding[52];
} SDL_Event;

typedef uint32_t SDL_PixelFormatEnum;
#define SDL_PIXELFORMAT_RGBA8888 0x16462004u
#define SDL_PIXELFORMAT_ARGB8888 0x16362004u

struct SDL_Surface {
    uint32_t flags;
    void*    format;
    int      w, h;
    int      pitch;
    void*    pixels;
    void*    userdata;
    int      locked;
    void*    lock_data;
    SDL_Rect clip_rect;
    void*    map;
    int      refcount;
};

// Core SDL functions
extern int           SDL_Init(uint32_t flags);
extern void          SDL_Quit(void);
extern void          SDL_Delay(uint32_t ms);
extern const char*   SDL_GetError(void);
extern void          SDL_ClearError(void);
extern uint64_t      SDL_GetTicks64(void);
extern uint32_t      SDL_GetTicks(void);
extern int           SDL_PollEvent(SDL_Event* event);

// Window
extern SDL_Window*   SDL_CreateWindow(const char* title, int x, int y, int w, int h, uint32_t flags);
extern void          SDL_DestroyWindow(SDL_Window* window);
extern void          SDL_SetWindowTitle(SDL_Window* window, const char* title);
extern void          SDL_GetWindowSize(SDL_Window* window, int* w, int* h);

// Renderer
extern SDL_Renderer* SDL_CreateRenderer(SDL_Window* window, int index, uint32_t flags);
extern void          SDL_DestroyRenderer(SDL_Renderer* renderer);
extern int           SDL_RenderClear(SDL_Renderer* renderer);
extern void          SDL_RenderPresent(SDL_Renderer* renderer);
extern int           SDL_SetRenderDrawColor(SDL_Renderer* renderer, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
extern int           SDL_RenderFillRect(SDL_Renderer* renderer, const SDL_Rect* rect);
extern int           SDL_RenderDrawRect(SDL_Renderer* renderer, const SDL_Rect* rect);
extern int           SDL_RenderDrawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2);
extern int           SDL_RenderDrawPoint(SDL_Renderer* renderer, int x, int y);
extern int           SDL_RenderCopy(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect);
extern int           SDL_RenderSetLogicalSize(SDL_Renderer* renderer, int w, int h);
extern int           SDL_SetRenderDrawBlendMode(SDL_Renderer* renderer, int blendMode);

// Texture
extern SDL_Texture*  SDL_CreateTexture(SDL_Renderer* renderer, uint32_t format, int access, int w, int h);
extern SDL_Texture*  SDL_CreateTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface);
extern void          SDL_DestroyTexture(SDL_Texture* texture);
extern int           SDL_SetTextureAlphaMod(SDL_Texture* texture, uint8_t alpha);
extern int           SDL_SetTextureBlendMode(SDL_Texture* texture, int blendMode);
extern int           SDL_QueryTexture(SDL_Texture* texture, uint32_t* format, int* access, int* w, int* h);

// Surface
extern void          SDL_FreeSurface(SDL_Surface* surface);
extern SDL_Surface*  SDL_CreateRGBSurface(uint32_t flags, int w, int h, int depth, uint32_t Rmask, uint32_t Gmask, uint32_t Bmask, uint32_t Amask);

#ifdef __cplusplus
}
#endif
