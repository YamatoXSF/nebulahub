#pragma once
#define SCE_HTTP_VERSION_1_1 1
#define SCE_TRUE  1
#define SCE_FALSE 0
#define SCE_HTTP_METHOD_GET 0
extern int sceHttpInit(unsigned int poolSize);
extern int sceHttpTerm(void);
extern int sceHttpCreateTemplate(const char* userAgent, int httpVer, int isAutoProxyConf);
extern int sceHttpDeleteTemplate(int tmplId);
extern int sceHttpSetConnectTimeOut(int id, unsigned int usec);
extern int sceHttpSetSendTimeOut(int id, unsigned int usec);
extern int sceHttpSetRecvTimeOut(int id, unsigned int usec);
extern int sceHttpCreateConnectionWithURL(int tmplId, const char* url, int isEnableKeepalive);
extern int sceHttpDeleteConnection(int connId);
extern int sceHttpCreateRequestWithURL(int connId, int method, const char* url, unsigned long long contentLength);
extern int sceHttpDeleteRequest(int reqId);
extern int sceHttpSendRequest(int reqId, const void* postData, unsigned int size);
extern int sceHttpGetStatusCode(int reqId, int* statusCode);
extern int sceHttpGetContentLength(int reqId, unsigned long long* contentLength);
extern int sceHttpReadData(int reqId, void* data, unsigned int size);
