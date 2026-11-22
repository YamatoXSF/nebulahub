#pragma once
#include "SDL.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct TTF_Font TTF_Font;

#define TTF_STYLE_NORMAL    0x00
#define TTF_STYLE_BOLD      0x01
#define TTF_STYLE_ITALIC    0x02

extern int          TTF_Init(void);
extern void         TTF_Quit(void);
extern TTF_Font*    TTF_OpenFont(const char* file, int ptsize);
extern void         TTF_CloseFont(TTF_Font* font);
extern SDL_Surface* TTF_RenderUTF8_Blended(TTF_Font* font, const char* text, SDL_Color fg);
extern SDL_Surface* TTF_RenderText_Blended(TTF_Font* font, const char* text, SDL_Color fg);
extern int          TTF_SizeUTF8(TTF_Font* font, const char* text, int* w, int* h);
extern int          TTF_SizeText(TTF_Font* font, const char* text, int* w, int* h);
extern const char*  TTF_GetError(void);

#ifdef __cplusplus
}
#endif
