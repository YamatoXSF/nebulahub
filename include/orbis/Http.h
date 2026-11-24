#pragma once
// OVERRIDE for the toolchain's own orbis/Http.h — see Net.h for why.
// Signatures match long-established community PS4 homebrew convention.

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SCE_HTTP_VERSION_1_1    1
#define SCE_TRUE                1
#define SCE_FALSE               0
#define SCE_HTTP_METHOD_GET     0

int  sceHttpInit(unsigned int poolSize);
int  sceHttpTerm(void);

int  sceHttpCreateTemplate(const char* userAgent, int httpVer, int isAutoProxyConf);
int  sceHttpDeleteTemplate(int tmplId);

int  sceHttpSetConnectTimeOut(int id, unsigned int usec);
int  sceHttpSetSendTimeOut(int id, unsigned int usec);
int  sceHttpSetRecvTimeOut(int id, unsigned int usec);

int  sceHttpCreateConnectionWithURL(int tmplId, const char* url, int isEnableKeepalive);
int  sceHttpDeleteConnection(int connId);

int  sceHttpCreateRequestWithURL(int connId, int method, const char* url,
                                  uint64_t contentLength);
int  sceHttpDeleteRequest(int reqId);

int  sceHttpSendRequest(int reqId, const void* postData, unsigned int size);

int  sceHttpGetStatusCode(int reqId, int* statusCode);
int  sceHttpGetContentLength(int reqId, uint64_t* contentLength);

int  sceHttpReadData(int reqId, void* data, unsigned int size);

#ifdef __cplusplus
}
#endif
