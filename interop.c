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

bool Interop_CreateInstance(const char *TypeName, char *InstanceId, int32_t InstanceIdLength, void *ExecuteUserPtr,
                            Interop_ExecuteCallback Execute, Interop_InvokeInstanceCallback *InvokeInstance,
                            Interop_ReleaseInstanceCallback *ReleaseInstance,
                            Interop_ProcessInstanceCallback *ProcessInstance, void **UserPtr) {
    *InvokeInstance = NULL;
    *ReleaseInstance = NULL;
    *ProcessInstance = NULL;
    *UserPtr = NULL;

    if (strcmp(TypeName, "Steam.API") == 0) {
        *InvokeInstance = SteamAPI_Invoke;
        *ProcessInstance = SteamAPI_Process;
        return true;
    } else if (strcmp(TypeName, "Steam.Apps") == 0) {
        *InvokeInstance = SteamApps_Invoke;
        return true;
    } else if (strcmp(TypeName, "Steam.User") == 0) {
        *InvokeInstance = SteamUser_Invoke;
        SteamUser_GetInstanceId(InstanceId, InstanceIdLength);
        return true;
    } else if (strcmp(TypeName, "Steam.UserStats") == 0) {
        *InvokeInstance = SteamUserStats_Invoke;
        SteamUserStats_GetInstanceId(InstanceId, InstanceIdLength);
        return true;
    } else if (strcmp(TypeName, "Steam.Utils") == 0) {
        *InvokeInstance = SteamUtils_Invoke;
        return true;
    } else if (strcmp(TypeName, "Steam.Friends") == 0) {
        *InvokeInstance = SteamFriends_Invoke;
        return true;
    }
    return false;
}

bool Interop_SetOverride(const char *Key, void *Value) {
    InteropLib_SetOverride(Key, Value);
    return true;
}

bool Interop_SetOption(const char *Key, void *Value) {
    return true;
}

bool Interop_Load(void) {
    SteamAPI_InitLib();
    SteamUser_Init();
    SteamUserStats_Init();
    return true;
}

bool Interop_Unload(void) {
    SteamUserStats_Remove();
    SteamUser_Remove();
    SteamAPI_RemoveLib();
    return true;
}

/*********************************************************************/
