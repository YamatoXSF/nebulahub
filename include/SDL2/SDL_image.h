#pragma once
#include "SDL.h"
#ifdef __cplusplus
extern "C" {
#endif

#define IMG_INIT_JPG  0x00000001
#define IMG_INIT_PNG  0x00000002
#define IMG_INIT_TIF  0x00000004
#define IMG_INIT_WEBP 0x00000008

extern int           IMG_Init(int flags);
extern void          IMG_Quit(void);
extern SDL_Surface*  IMG_Load(const char* file);
extern SDL_Texture*  IMG_LoadTexture(SDL_Renderer* renderer, const char* file);
extern const char*   IMG_GetError(void);

#ifdef __cplusplus
}
#endif
