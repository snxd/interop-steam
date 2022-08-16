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
// Concrete functions

static bool SteamApps_IsAppInstalled(int32_t AppId) {
    return SteamApps()->BIsAppInstalled(AppId) != 0;
}

static bool SteamApps_GetAppInstallDir(int32_t AppId, char *Path, int32_t MaxPath) {
    if (SteamApps()->GetAppInstallDir(AppId, Path, MaxPath) == 0)
        return false;
    return true;
}

static bool SteamApps_GetAppBuildId(void) {
    return SteamApps()->GetAppBuildId();
}

static bool SteamApps_IsDLCInstalled(int32_t AppId) {
    return SteamApps()->BIsDlcInstalled(AppId) != 0;
}

static bool SteamApps_GetDLCCount(void) {
    return SteamApps()->GetDLCCount() != 0;
}

static bool SteamApps_GetDLCData(int32_t Index, int32_t *AppId, int32_t *Available, char *Name, int32_t MaxName) {
    uint32_t AppIdInt = 0;
    bool AvailableInt = false;

    if (SteamApps()->BGetDLCDataByIndex(Index, &AppIdInt, &AvailableInt, Name, MaxName) == 0)
        return false;

    *Available = (AvailableInt);
    *AppId = (int32_t)AppIdInt;
    return true;
}

static bool SteamApps_InstallDLC(int32_t AppId) {
    SteamApps()->InstallDLC(AppId);
    return true;
}

static bool SteamApps_UninstallDLC(int32_t AppId) {
    SteamApps()->UninstallDLC(AppId);
    return true;
}

static bool SteamApps_IsSubscribed(void) {
    return SteamApps()->BIsSubscribed() != 0;
}

static bool SteamApps_IsLowViolence(void) {
    return SteamApps()->BIsLowViolence() != 0;
}

static bool SteamApps_IsCybercafe(void) {
    return SteamApps()->BIsCybercafe() != 0;
}

static bool SteamApps_IsVACBanned(void) {
    return SteamApps()->BIsVACBanned() != 0;
}

static bool SteamApps_IsSubscribedFromFreeWeekend(void) {
    return SteamApps()->BIsSubscribedFromFreeWeekend() != 0;
}

static bool SteamApps_GetCurrentBetaName(char *Name, int32_t MaxName) {
    if (SteamApps()->GetCurrentBetaName(Name, MaxName) == 0)
        return false;
    return true;
}

static bool SteamApps_MarkContentCorrupt(bool MissingFilesOnly) {
    if (SteamApps()->MarkContentCorrupt(MissingFilesOnly) == 0)
        return false;
    return true;
}

/*********************************************************************/
// Interop functions

bool SteamApps_Process(void *SteamAppsContext) {
    // This function is called once per tick and can be used to process simple operations and
    // thread synchronization.

    return true;
}

bool SteamApps_Invoke(void *SteamAppsContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle) {
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    echandle ItemHandle = NULL;
    bool RetVal = false;
    int32_t ReturnValue = false;
    int32_t Value32 = 0;
    bool ValueBool = false;
    const char *Method = NULL;
    char *ValueString = NULL;

    if (SteamAPI_IsInitialized() == false)
        return false;
    if (IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "method", &Method) == false)
        return false;

    if (strcmp(Method, "isAppInstalled") == 0) {
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "id", &Value32);
        if (RetVal == true)
            ReturnValue = SteamApps_IsAppInstalled(Value32);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (strcmp(Method, "getAppInstallDir") == 0) {
        char InstallDir[320] = {0};
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "id", &Value32);
        if (RetVal == true)
            ReturnValue = SteamApps_GetAppInstallDir(Value32, InstallDir, sizeof(InstallDir));
        IDictionary_AddString(ReturnDictionaryHandle, "returnValue", InstallDir, &ItemHandle);
    } else if (strcmp(Method, "getAppBuildId") == 0) {
        ReturnValue = SteamApps_GetAppBuildId();
        RetVal = IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (strcmp(Method, "isDLCInstalled") == 0) {
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "id", &Value32);
        if (RetVal == true)
            ReturnValue = SteamApps_IsDLCInstalled(Value32);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (strcmp(Method, "getDLCCount") == 0) {
        ReturnValue = SteamApps_GetDLCCount();
        RetVal = IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (strcmp(Method, "getDLCData") == 0) {
        char Name[320] = {0};
        int32_t AppId = 0;
        int32_t Available = false;
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "index", &Value32);
        if (RetVal == true)
            RetVal = SteamApps_GetDLCData(Value32, &AppId, &Available, Name, sizeof(Name));
        if (RetVal == true) {
            echandle ReturnObjDictHandle = NULL;

            IDictionary_AddDictionary(ReturnDictionaryHandle, "returnValue", &ReturnObjDictHandle, &ItemHandle);

            IDictionary_AddInt32(ReturnObjDictHandle, "id", AppId, &ItemHandle);
            IDictionary_AddBoolean(ReturnObjDictHandle, "available", Available, &ItemHandle);
            IDictionary_AddString(ReturnObjDictHandle, "name", Name, &ItemHandle);
        }
        if (RetVal == false)
            RetVal = IDictionary_AddNull(ReturnDictionaryHandle, "returnValue", &ItemHandle);
    } else if (strcmp(Method, "installDLC") == 0) {
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "id", &Value32);
        if (RetVal == true)
            ReturnValue = SteamApps_InstallDLC(Value32);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (strcmp(Method, "uninstallDLC") == 0) {
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "id", &Value32);
        if (RetVal == true)
            ReturnValue = SteamApps_UninstallDLC(Value32);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (strcmp(Method, "isVACBanned") == 0) {
        ReturnValue = SteamApps_IsVACBanned();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (strcmp(Method, "isSubscribed") == 0) {
        ReturnValue = SteamApps_IsSubscribed();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (strcmp(Method, "isLowViolence") == 0) {
        ReturnValue = SteamApps_IsLowViolence();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (strcmp(Method, "isCybercafe") == 0) {
        ReturnValue = SteamApps_IsCybercafe();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (strcmp(Method, "isSubscribedFromWeekend") == 0) {
        ReturnValue = SteamApps_IsSubscribedFromFreeWeekend();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (strcmp(Method, "getCurrentBetaName") == 0) {
        char Name[320] = {0};
        SteamApps_GetCurrentBetaName(Name, sizeof(Name));
        RetVal = IDictionary_AddString(ReturnDictionaryHandle, "returnValue", Name, &ItemHandle);
    } else if (strcmp(Method, "markContentCorrupt") == 0) {
        RetVal = IDictionary_GetBooleanByKey(MethodDictionaryHandle, "missingFilesOnly", &ValueBool);
        if (RetVal == true)
            ReturnValue = SteamApps_MarkContentCorrupt(ValueBool);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }

    return RetVal;
}

/*********************************************************************/
