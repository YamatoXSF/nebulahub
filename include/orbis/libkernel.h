#pragma once
typedef int OrbisKernelEqueue;
typedef unsigned int OrbisKernelUseconds;
extern int  sceKernelUsleep(OrbisKernelUseconds useconds);
extern unsigned long long sceKernelGetProcessTimeCounter(void);
extern unsigned long long sceKernelGetProcessTimeCounterFrequency(void);
extern int  sceKernelAllocateDirectMemory(long long searchStart, long long searchEnd, long long len, long long alignment, int memoryType, long long* physAddrOut);
