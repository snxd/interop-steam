#include <string.h>
#include "interop/interoptypes.h"
typedef __int32 int32;
typedef unsigned __int32 uint32;
typedef int32 fixed32;
#include "interop/interop.h"
#include "steam/isteamclient.h"
#include "steam/steam_api.h"
#include <stdlib.h>
#include <Windows.h>

#include "steamauth.h"

int32 Interop_CreateInstance(char *TypeName, char *InstanceId, int32 InstanceIdLength,
                             void *ExecuteUserPtr, Interop_ExecuteCallback Execute,
                             Interop_InvokeInstanceCallback *InvokeInstance,
                             Interop_RemoveInstanceCallback *RemoveInstance,
                             Interop_ProcessInstanceCallback *ProcessInstance,
                             void **UserPtr)
{
    void *Context;

    if (strcmp(TypeName, "Steam.Auth") == 0)
    {
        *InvokeInstance = SteamAuth_InvokeInstance;
        *RemoveInstance = SteamAuth_RemoveInstance;
        *ProcessInstance = NULL;

        SteamAuth_Create(&Context);
        SteamAuth_SetInstanceId(Context, InstanceId);
        SteamAuth_SetInteropExecuteCallback(Context, ExecuteUserPtr, Execute);
        *UserPtr = Context;
        return TRUE;
    }
    return FALSE;
}

int32 Interop_SetOption(char *Key, char *Value)
{
    return TRUE;
}

int32 Interop_Init(InteropHostStruct *InteropHost)
{
    wchar_t Filename[_MAX_PATH] = {0};
    GetModuleFileNameW(NULL, Filename, _MAX_PATH);
    wchar_t *Slash = 0;
    for(wchar_t *c = Filename; *c != 0; c++)
    {
        if(*c == '\\')
            Slash = c;
    }
    *Slash = 0;
    wcscat_s(Filename,_MAX_PATH,L"\\steam_api.dll");

    LoadLibrary(Filename);
    return TRUE;
}

int32 Interop_Remove()
{
    return TRUE;
}

}