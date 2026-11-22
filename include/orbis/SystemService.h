#pragma once
typedef struct OrbisSystemServiceParamId { int id; } OrbisSystemServiceParamId;
extern int sceSystemServiceLoadExec(const char* path, char* const argv[]);
extern int sceSystemServiceInstallUpdate(const char* path);
