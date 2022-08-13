#include <string.h>
#include <malloc.h>
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
// Concrete Functions

static int32 SteamApps_IsAppInstalled(int32 AppId) {
    return SteamApps()->BIsAppInstalled(AppId) != 0;
}

static int32 SteamApps_GetAppInstallDir(int32 AppId, char *Path, int32 MaxPath) {
    if (SteamApps()->GetAppInstallDir(AppId, Path, MaxPath) == 0)
        return FALSE;
    return TRUE;
}

static int32 SteamApps_GetAppBuildId(void) {
    return SteamApps()->GetAppBuildId();
}

static int32 SteamApps_IsDLCInstalled(int32 AppId) {
    return SteamApps()->BIsDlcInstalled(AppId) != 0;
}

static int32 SteamApps_GetDLCCount(void) {
    return SteamApps()->GetDLCCount() != 0;
}

static int32 SteamApps_GetDLCData(int32 Index, int32 *AppId, int32 *Available, char *Name, int32 MaxName) {
    uint32 AppIdInt = 0;
    bool AvailableInt = false;

    if (SteamApps()->BGetDLCDataByIndex(Index, &AppIdInt, &AvailableInt, Name, MaxName) == 0)
        return FALSE;

    *Available = (AvailableInt);
    *AppId = (int32)AppIdInt;
    return TRUE;
}

static int32 SteamApps_InstallDLC(int32 AppId) {
    SteamApps()->InstallDLC(AppId);
    return TRUE;
}

static int32 SteamApps_UninstallDLC(int32 AppId) {
    SteamApps()->UninstallDLC(AppId);
    return TRUE;
}

static int32 SteamApps_IsSubscribed(void) {
    return SteamApps()->BIsSubscribed() != 0;
}

static int32 SteamApps_IsLowViolence(void) {
    return SteamApps()->BIsLowViolence() != 0;
}

static int32 SteamApps_IsCybercafe(void) {
    return SteamApps()->BIsCybercafe() != 0;
}

static int32 SteamApps_IsVACBanned(void) {
    return SteamApps()->BIsVACBanned() != 0;
}

static int32 SteamApps_IsSubscribedFromFreeWeekend(void) {
    return SteamApps()->BIsSubscribedFromFreeWeekend() != 0;
}

static int32 SteamApps_GetCurrentBetaName(char *Name, int32 MaxName) {
    if (SteamApps()->GetCurrentBetaName(Name, MaxName) == 0)
        return FALSE;
    return TRUE;
}

static int32 SteamApps_MarkContentCorrupt(int32 MissingFilesOnly) {
    if (SteamApps()->MarkContentCorrupt(MissingFilesOnly) == 0)
        return FALSE;
    return TRUE;
}

/*********************************************************************/
// Interop Functions

int32 SteamApps_Process(void *SteamAppsContext) {
    // This function is called once per tick and can be used to process simple operations and
    // thread synchronization.

    return TRUE;
}

int32 SteamApps_Invoke(void *SteamAppsContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle) {
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    echandle ItemHandle = NULL;
    int32 RetVal = FALSE;
    int32 ReturnValue = FALSE;
    int32 Value32 = 0;
    char *Method = NULL;
    char *ValueString = NULL;

    if (SteamAPI_IsInitialized() == FALSE)
        return FALSE;
    if (IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "method", &Method) == FALSE)
        return FALSE;

    if (String_Compare(Method, "isAppInstalled") == TRUE) {
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "id", &Value32);
        if (RetVal == TRUE)
            ReturnValue = SteamApps_IsAppInstalled(Value32);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "getAppInstallDir") == TRUE) {
        char InstallDir[320] = {0};
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "id", &Value32);
        if (RetVal == TRUE)
            ReturnValue = SteamApps_GetAppInstallDir(Value32, InstallDir, Element_Count(InstallDir));
        IDictionary_AddString(ReturnDictionaryHandle, "returnValue", InstallDir, &ItemHandle);
    } else if (String_Compare(Method, "getAppBuildId") == TRUE) {
        ReturnValue = SteamApps_GetAppBuildId();
        RetVal = IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "isDLCInstalled") == TRUE) {
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "id", &Value32);
        if (RetVal == TRUE)
            ReturnValue = SteamApps_IsDLCInstalled(Value32);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "getDLCCount") == TRUE) {
        ReturnValue = SteamApps_GetDLCCount();
        RetVal = IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "getDLCData") == TRUE) {
        char Name[320] = {0};
        int32 AppId = 0;
        int32 Available = FALSE;
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "index", &Value32);
        if (RetVal == TRUE)
            RetVal = SteamApps_GetDLCData(Value32, &AppId, &Available, Name, Element_Count(Name));
        if (RetVal == TRUE) {
            echandle ReturnObjDictHandle = NULL;

            IDictionary_AddDictionary(ReturnDictionaryHandle, "returnValue", &ReturnObjDictHandle, &ItemHandle);

            IDictionary_AddInt32(ReturnObjDictHandle, "id", AppId, &ItemHandle);
            IDictionary_AddBoolean(ReturnObjDictHandle, "available", Available, &ItemHandle);
            IDictionary_AddString(ReturnObjDictHandle, "name", Name, &ItemHandle);
        }
        if (RetVal == FALSE)
            RetVal = IDictionary_AddNull(ReturnDictionaryHandle, "returnValue", &ItemHandle);
    } else if (String_Compare(Method, "installDLC") == TRUE) {
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "id", &Value32);
        if (RetVal == TRUE)
            ReturnValue = SteamApps_InstallDLC(Value32);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "uninstallDLC") == TRUE) {
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "id", &Value32);
        if (RetVal == TRUE)
            ReturnValue = SteamApps_UninstallDLC(Value32);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "isVACBanned") == TRUE) {
        ReturnValue = SteamApps_IsVACBanned();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "isSubscribed") == TRUE) {
        ReturnValue = SteamApps_IsSubscribed();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "isLowViolence") == TRUE) {
        ReturnValue = SteamApps_IsLowViolence();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "isCybercafe") == TRUE) {
        ReturnValue = SteamApps_IsCybercafe();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "isSubscribedFromWeekend") == TRUE) {
        ReturnValue = SteamApps_IsSubscribedFromFreeWeekend();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "getCurrentBetaName") == TRUE) {
        char Name[320] = {0};
        SteamApps_GetCurrentBetaName(Name, Element_Count(Name));
        RetVal = IDictionary_AddString(ReturnDictionaryHandle, "returnValue", Name, &ItemHandle);
    } else if (String_Compare(Method, "markContentCorrupt") == TRUE) {
        RetVal = IDictionary_GetBooleanByKey(MethodDictionaryHandle, "missingFilesOnly", &Value32);
        if (RetVal == TRUE)
            ReturnValue = SteamApps_MarkContentCorrupt(Value32);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }

    return RetVal;
}

/*********************************************************************/
