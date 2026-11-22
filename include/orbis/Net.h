#pragma once
#define SCE_NET_CTL_INFO_IP_ADDRESS 1
typedef struct { unsigned int size; void* memory; int flags; } SceNetInitParam;
typedef union { char ip_address[16]; } SceNetCtlInfo;
extern int sceNetInit(SceNetInitParam* param);
extern int sceNetTerm(void);
extern int sceNetCtlInit(void);
extern int sceNetCtlTerm(void);
extern int sceNetCtlGetInfo(int code, SceNetCtlInfo* info);
