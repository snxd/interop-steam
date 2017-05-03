#include <string.h>

#include "interoplib.h"
#include "interopstub.h"

#include "steamauth.h"

/*********************************************************************/

int32 Interop_CreateInstance(char *TypeName, char *InstanceId, int32 InstanceIdLength,
                             void *ExecuteUserPtr, Interop_ExecuteCallback Execute,
                             Interop_InvokeInstanceCallback *InvokeInstance,
                             Interop_ReleaseInstanceCallback *ReleaseInstance,
                             Interop_ProcessInstanceCallback *ProcessInstance,
                             void **UserPtr)
{
    void *Context;

    if (String_Compare(TypeName, "Steam.Auth") == TRUE)
    {
        *InvokeInstance = SteamAuth_Invoke;
        *ReleaseInstance = SteamAuth_Release;
        *ProcessInstance = NULL;

        // Comment this line in if you want the process call
        // *ProcessInstance = SteamAuth_Process;

        SteamAuth_Create(&Context);
        SteamAuth_GetInstanceId(Context, InstanceId, InstanceIdLength);
        SteamAuth_SetInteropExecuteCallback(Context, ExecuteUserPtr, Execute);
        *UserPtr = Context;
        return TRUE;
    }
    return FALSE;
}

int32 Interop_SetOverride(char *Key, void *Value)
{
    InteropLib_SetOverride(Key, Value);
    return TRUE;
}

int32 Interop_SetOption(char *Key, char *Value)
{
    return TRUE;
}

int32 Interop_Load()
{
    return TRUE;
}

int32 Interop_Unload()
{
    return TRUE;
}

/*********************************************************************/
