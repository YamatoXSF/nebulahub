#pragma once
// OVERRIDE for the toolchain's own orbis/Ssl.h — see Net.h for why.

#ifdef __cplusplus
extern "C" {
#endif

int sceSslInit(unsigned int poolSize);
int sceSslTerm(void);

#ifdef __cplusplus
}
#endif
