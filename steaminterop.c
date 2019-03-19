#include <string.h>

#include "interoplib.h"
#include "interopstub.h"

#include "steamapi.h"
#include "steamapps.h"
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
    *InvokeInstance = NULL;
    *ReleaseInstance = NULL;
    *ProcessInstance = NULL;
    *UserPtr = NULL;

    if (String_Compare(TypeName, "Steam.API") == TRUE)
    {
        *InvokeInstance = SteamAPI_Invoke;
        *ProcessInstance = SteamAPI_Process;
        return TRUE;
    }
    else if (String_Compare(TypeName, "Steam.Apps") == TRUE)
    {
        *InvokeInstance = SteamApps_Invoke;
        return TRUE;
    }
    else if (String_Compare(TypeName, "Steam.User") == TRUE)
    {
        *InvokeInstance = SteamUser_Invoke;
        SteamUser_GetInstanceId(InstanceId, InstanceIdLength);
        return TRUE;
    }
    else if (String_Compare(TypeName, "Steam.UserStats") == TRUE)
    {
        *InvokeInstance = SteamUserStats_Invoke;
        SteamUserStats_GetInstanceId(InstanceId, InstanceIdLength);
        return TRUE;
    }
    else if (String_Compare(TypeName, "Steam.Utils") == TRUE)
    {
        *InvokeInstance = SteamUtils_Invoke;
        return TRUE;
    }
    else if (String_Compare(TypeName, "Steam.Friends") == TRUE)
    {
        *InvokeInstance = SteamFriends_Invoke;
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
    SteamAPI_InitLib();
    SteamUser_Init();
    SteamUserStats_Init();
    return TRUE;
}

int32 Interop_Unload()
{
    SteamUserStats_Remove();
    SteamUser_Remove();
    SteamAPI_RemoveLib();
    return TRUE;
}

/*********************************************************************/
