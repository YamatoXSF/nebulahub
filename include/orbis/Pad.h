#pragma once
#define ORBIS_PAD_PORT_TYPE_STANDARD 0
typedef struct { unsigned char x, y; } OrbisPadAnalogStick;
typedef struct {
    unsigned int buttons;
    OrbisPadAnalogStick leftStick;
    OrbisPadAnalogStick rightStick;
    unsigned char padding[64];
} OrbisPadData;
typedef int OrbisUserServiceUserId;
extern int scePadInit(void);
extern int scePadOpen(OrbisUserServiceUserId userId, int type, int index, void* param);
extern int scePadClose(int handle);
extern int scePadReadState(int handle, OrbisPadData* data);
