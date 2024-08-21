#include <string.h>
#include <stdlib.h>

#include "interoplib.h"
#include "interopstub.h"
#include "dictionaryi.h"

#if defined(_WIN32)
#include <windows.h>
#endif

#include "steam_api.h"

#include "steamapi.h"
#include "steamapps.h"

/*********************************************************************/
// Interop functions

bool SteamApps_Invoke(void *handle, echandle method_dictionary_handle, echandle return_dictionary_handle) {
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    bool ret = false;
    const char *method = nullptr;

    if (!SteamAPI_IsInitialized())
        return false;
    if (!IDictionary_GetStringPtrByKey(method_dictionary_handle, "method", &method))
        return false;

    if (strcmp(method, "isAppInstalled") == 0) {
        bool is_installed = false;
        int32_t app_id = 0;
        ret = IDictionary_GetInt32ByKey(method_dictionary_handle, "id", &app_id);
        if (ret)
            is_installed = SteamApps()->BIsAppInstalled(app_id);
        IDictionary_AddBoolean(return_dictionary_handle, "returnValue", is_installed, nullptr);
    } else if (strcmp(method, "getAppInstallDir") == 0) {
        int32_t app_id = 0;
        char install_dir[320] = {0};
        ret = IDictionary_GetInt32ByKey(method_dictionary_handle, "id", &app_id);
        if (ret)
            SteamApps()->GetAppInstallDir(app_id, install_dir, sizeof(install_dir));
        IDictionary_AddString(return_dictionary_handle, "returnValue", install_dir, nullptr);
    } else if (strcmp(method, "getAppBuildId") == 0) {
        const auto return_value = SteamApps()->GetAppBuildId();
        ret = IDictionary_AddInt(return_dictionary_handle, "returnValue", return_value, nullptr);
    } else if (strcmp(method, "isDLCInstalled") == 0) {
        bool is_installed = false;
        int32_t app_id = 0;
        ret = IDictionary_GetInt32ByKey(method_dictionary_handle, "id", &app_id);
        if (ret)
            is_installed = SteamApps()->BIsDlcInstalled(app_id);
        IDictionary_AddBoolean(return_dictionary_handle, "returnValue", is_installed, nullptr);
    } else if (strcmp(method, "getDLCCount") == 0) {
        const auto dlc_count = SteamApps()->GetDLCCount();
        ret = IDictionary_AddInt(return_dictionary_handle, "returnValue", dlc_count, nullptr);
    } else if (strcmp(method, "getDLCData") == 0) {
        char name[320]{};
        int32_t index = 0;
        uint32_t app_id = 0;
        bool available = false;
        ret = IDictionary_GetInt32ByKey(method_dictionary_handle, "index", &index);
        if (ret)
            ret = SteamApps()->BGetDLCDataByIndex(index, &app_id, &available, name, sizeof(name));
        if (ret) {
            echandle return_obj_dict_handle = nullptr;

            IDictionary_AddDictionary(return_dictionary_handle, "returnValue", &return_obj_dict_handle, nullptr);

            IDictionary_AddInt(return_obj_dict_handle, "id", app_id, nullptr);
            IDictionary_AddBoolean(return_obj_dict_handle, "available", available, nullptr);
            IDictionary_AddString(return_obj_dict_handle, "name", name, nullptr);
        }
        if (!ret)
            ret = IDictionary_AddNull(return_dictionary_handle, "returnValue", nullptr);
    } else if (strcmp(method, "installDLC") == 0) {
        int32_t app_id = 0;
        ret = IDictionary_GetInt32ByKey(method_dictionary_handle, "id", &app_id);
        if (ret)
            SteamApps()->InstallDLC(app_id);
        IDictionary_AddBoolean(return_dictionary_handle, "returnValue", ret, nullptr);
    } else if (strcmp(method, "uninstallDLC") == 0) {
        int32_t app_id = 0;
        ret = IDictionary_GetInt32ByKey(method_dictionary_handle, "id", &app_id);
        if (ret)
            SteamApps()->UninstallDLC(app_id);
        IDictionary_AddBoolean(return_dictionary_handle, "returnValue", ret, nullptr);
    } else if (strcmp(method, "isVACBanned") == 0) {
        const auto vac_banned = SteamApps()->BIsVACBanned();
        ret = IDictionary_AddBoolean(return_dictionary_handle, "returnValue", vac_banned, nullptr);
    } else if (strcmp(method, "isSubscribed") == 0) {
        const auto subscribed = SteamApps()->BIsSubscribed();
        ret = IDictionary_AddBoolean(return_dictionary_handle, "returnValue", subscribed, nullptr);
    } else if (strcmp(method, "isLowViolence") == 0) {
        const auto low_violence = SteamApps()->BIsLowViolence();
        ret = IDictionary_AddBoolean(return_dictionary_handle, "returnValue", low_violence, nullptr);
    } else if (strcmp(method, "isCybercafe") == 0) {
        const auto cybercafe = SteamApps()->BIsCybercafe();
        ret = IDictionary_AddBoolean(return_dictionary_handle, "returnValue", cybercafe, nullptr);
    } else if (strcmp(method, "isSubscribedFromWeekend") == 0) {
        const auto subscribed = SteamApps()->BIsSubscribedFromFreeWeekend();
        ret = IDictionary_AddBoolean(return_dictionary_handle, "returnValue", subscribed, nullptr);
    } else if (strcmp(method, "getCurrentBetaName") == 0) {
        char name[320]{};
        SteamApps()->GetCurrentBetaName(name, sizeof(name));
        ret = IDictionary_AddString(return_dictionary_handle, "returnValue", name, nullptr);
    } else if (strcmp(method, "markContentCorrupt") == 0) {
        bool return_value = false;
        bool missing_files_only = false;
        ret = IDictionary_GetBooleanByKey(method_dictionary_handle, "missingFilesOnly", &missing_files_only);
        if (ret)
            return_value = SteamApps()->MarkContentCorrupt(missing_files_only);
        IDictionary_AddBoolean(return_dictionary_handle, "returnValue", return_value, nullptr);
    }

    return ret;
}

/*********************************************************************/
