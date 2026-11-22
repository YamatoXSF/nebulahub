#pragma once
typedef int OrbisUserServiceUserId;
typedef struct { OrbisUserServiceUserId userId; } OrbisUserServiceInitialUser;
typedef struct OrbisUserServiceInitializeParams { int priority; } OrbisUserServiceInitializeParams;
extern int sceUserServiceInitialize(const OrbisUserServiceInitializeParams* params);
extern int sceUserServiceGetInitialUser(OrbisUserServiceInitialUser* user);
#define sceUserServiceGetInitialUser(x) (((OrbisUserServiceInitialUser*)(x))->userId = 1, 0)
