#include <string.h>

#include "interoplib.h"
#include "interopstub.h"

#include "steamapp.h"
#include "steamuser.h"
#include "steamuserstats.h"
#include "steamutils.h"
#include "steamfriends.h"

/*********************************************************************/

int32 Interop_CreateInstance(char *TypeName, char *InstanceId, int32 InstanceIdLength,
                             void *ExecuteUserPtr, Interop_ExecuteCallback Execute,
                             Interop_InvokeInstanceCallback *InvokeInstance,
                             Interop_ReleaseInstanceCallback *ReleaseInstance,
                             Interop_ProcessInstanceCallback *ProcessInstance,
                             void **UserPtr)
{
    void *Context;

    if (String_Compare(TypeName, "Steam.App") == TRUE)
    {
        *InvokeInstance = SteamApp_Invoke;
        *ReleaseInstance = NULL;
        *ProcessInstance = SteamApp_Process;

        SteamApp_GetInstanceId(InstanceId, InstanceIdLength);
        *UserPtr = NULL;
        return TRUE;
    }
    else if (String_Compare(TypeName, "Steam.User") == TRUE)
    {
        *InvokeInstance = SteamUser_Invoke;
        *ReleaseInstance = SteamUser_Release;
        *ProcessInstance = NULL;

        SteamUser_Create(&Context);
        SteamUser_GetInstanceId(Context, InstanceId, InstanceIdLength);
        *UserPtr = Context;
        return TRUE;
    }
    else if (String_Compare(TypeName, "Steam.UserStats") == TRUE)
    {
        *InvokeInstance = SteamUserStats_Invoke;
        *ReleaseInstance = SteamUserStats_Release;
        *ProcessInstance = NULL;

        SteamUserStats_Create(&Context);
        SteamUserStats_GetInstanceId(Context, InstanceId, InstanceIdLength);
        *UserPtr = Context;
        return TRUE;
    }
    else if (String_Compare(TypeName, "Steam.Utils") == TRUE)
    {
        *InvokeInstance = SteamUtils_Invoke;
        *ReleaseInstance = SteamUtils_Release;
        *ProcessInstance = NULL;

        SteamUtils_Create(&Context);
        SteamUtils_GetInstanceId(Context, InstanceId, InstanceIdLength);
        *UserPtr = Context;
        return TRUE;
    }
    else if (String_Compare(TypeName, "Steam.Friends") == TRUE)
    {
        *InvokeInstance = SteamFriends_Invoke;
        *ReleaseInstance = SteamFriends_Release;
        *ProcessInstance = NULL;

        SteamFriends_Create(&Context);
        SteamFriends_GetInstanceId(Context, InstanceId, InstanceIdLength);
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
    SteamApp_Init();
    return TRUE;
}

int32 Interop_Unload()
{
    SteamApp_Remove();
    return TRUE;
}

/*********************************************************************/
