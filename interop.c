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

bool Interop_CreateInstance(const char *type_name, char *instance_id, int32_t max_instance_id, void *execute_user_ptr,
                            Interop_ExecuteCallback execute, Interop_InvokeInstanceCallback *invoke_instance,
                            Interop_ReleaseInstanceCallback *release_instance,
                            Interop_ProcessInstanceCallback *process_instance, void **user_ptr) {
    *invoke_instance = NULL;
    *release_instance = NULL;
    *process_instance = NULL;
    *user_ptr = NULL;

    if (strcmp(type_name, "Steam.API") == 0) {
        *invoke_instance = SteamAPI_Invoke;
        *process_instance = SteamAPI_Process;
        return true;
    } else if (strcmp(type_name, "Steam.Apps") == 0) {
        *invoke_instance = SteamApps_Invoke;
        return true;
    } else if (strcmp(type_name, "Steam.User") == 0) {
        *invoke_instance = SteamUser_Invoke;
        SteamUser_GetInstanceId(instance_id, max_instance_id);
        return true;
    } else if (strcmp(type_name, "Steam.UserStats") == 0) {
        *invoke_instance = SteamUserStats_Invoke;
        SteamUserStats_GetInstanceId(instance_id, max_instance_id);
        return true;
    } else if (strcmp(type_name, "Steam.Utils") == 0) {
        *invoke_instance = SteamUtils_Invoke;
        return true;
    } else if (strcmp(type_name, "Steam.Friends") == 0) {
        *invoke_instance = SteamFriends_Invoke;
        return true;
    }
    return false;
}

bool Interop_SetOption(const char *key, void *value) {
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
