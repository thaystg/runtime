#define MONO_DLL_EXPORT

#include <mono\utils\mono-publib.h>
#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#endif

/*1   0x00007DA0    CoreCLRCreateCordbObject
2   0x00007CA0    CoreCLRCreateCordbObjectEx
3   0x00007C60    CreateCordbObject
4   0x00007DC0    DllGetClassObjectInternal
5   0x0000D0C0    OpenVirtualProcess
6   0x0000D070    OpenVirtualProcess2
7   0x0000CE30    OpenVirtualProcessImpl
8   0x0000CFF0    OpenVirtualProcessImpl2*/

MONO_API CoreCLRCreateCordbObjectEx(int iDebuggerVersion, DWORD pid, LPCWSTR lpApplicationGroupId, HMODULE hmodTargetCLR, void** ppCordb)
{
	printf("CoreCLRCreateCordbObjectEx - cheguei aqui hein thays\n");
}


MONO_API CreateCordbObject(int iDebuggerVersion, void** ppCordb)
{
	printf("CreateCordbObject - cheguei aqui hein thays\n");
}
