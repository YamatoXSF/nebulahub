#pragma once
// OVERRIDE for the toolchain's own orbis/Net.h, which only ships bare
// zero-argument placeholder symbols (Sony's real sceNet API is proprietary,
// so the open-source toolchain can't publish real prototypes).
//
// The signatures below are the long-established community-standard
// prototypes used across PS4 homebrew (matching 4PT, FPKGi and other public
// projects) — NOT invented here, but not independently verified against
// this specific build either. Struct field layout in particular should be
// confirmed against real behavior once this runs on actual hardware.

#ifdef __cplusplus
extern "C" {
#endif

#define SCE_NET_CTL_INFO_IP_ADDRESS 14

typedef struct SceNetInitParam {
    void* memory;
    int   size;
    int   flags;
} SceNetInitParam;

typedef union SceNetCtlInfo {
    char ip_address[16];
    char raw[128];   // room for other union members we don't use yet
} SceNetCtlInfo;

int  sceNetInit(SceNetInitParam* param);
int  sceNetTerm(void);
int  sceNetCtlInit(void);
void sceNetCtlTerm(void);
int  sceNetCtlGetInfo(int code, SceNetCtlInfo* info);

#ifdef __cplusplus
}
#endif
