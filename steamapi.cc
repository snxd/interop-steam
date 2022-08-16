#include <string.h>
#include <stdlib.h>

#include "interoplib.h"
#include "interopstub.h"
#include "dictionaryi.h"

#if defined(_WIN32)
#include <windows.h>
#define putenv _putenv
#endif

#include "steam_api.h"

#include "steamapi.h"

/*********************************************************************/

static bool GlobalSteamIsInitialized = false;

/********************************************************************/
// Concrete functions

static bool SteamAPI_InitializeInt() {
    if (SteamAPI_IsSteamRunning() == true) {
        if (SteamAPI_Init())
            GlobalSteamIsInitialized = true;
        else
            GlobalSteamIsInitialized = false;
    } else
        GlobalSteamIsInitialized = false;

    return GlobalSteamIsInitialized;
}

bool SteamAPI_IsInitialized() {
    return GlobalSteamIsInitialized;
}

static bool SteamAPI_SetAppId(int32_t AppId) {
    char EnvVar[320] = {0};

    if (SteamAPI_IsInitialized() == true)
        return false;
    if (AppId == 0)
        return false;

    String_Print(EnvVar, sizeof(EnvVar), "SteamAppId=%d", AppId);
    putenv(EnvVar);
    String_Print(EnvVar, sizeof(EnvVar), "SteamGameId=%d", AppId);
    putenv(EnvVar);
    return true;
}

/*********************************************************************/
// Interop functions

bool SteamAPI_Process(void *SteamAPIContext) {
    // This function is called once per tick and can be used to process simple operations and
    // thread synchronization.

    SteamAPI_RunCallbacks();
    return true;
}

bool SteamAPI_Invoke(void *SteamAPIContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle) {
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    echandle ItemHandle = NULL;
    bool RetVal = false;
    int32_t ReturnValue = false;
    int32_t Value32 = 0;
    const char *Method = NULL;
    char *ValueString = NULL;

    if (IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "method", &Method) == false)
        return false;

    if (String_Compare(Method, "initialize") == true) {
        ReturnValue = SteamAPI_InitializeInt();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "isSteamRunning") == true) {
        ReturnValue = (int32)SteamAPI_IsSteamRunning();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "isInitialized") == true) {
        ReturnValue = SteamAPI_IsInitialized();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "restartAppIfNecessary") == true) {
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "id", &Value32);
        if (RetVal == true)
            ReturnValue = (int32)SteamAPI_RestartAppIfNecessary(Value32);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "setAppId") == true) {
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "id", &Value32);
        if (RetVal == true)
            ReturnValue = SteamAPI_SetAppId(Value32);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }

    return RetVal;
}

/*********************************************************************/
// Global initialization functions

bool SteamAPI_InitLib(void) {
    wchar_t Filename[320] = {0};
    wchar_t *FilenamePtr = NULL;
    wchar_t *Slash = 0;

#if defined(_WIN32)
    GetModuleFileNameW(NULL, Filename, sizeof(Filename));

    for (FilenamePtr = Filename; *FilenamePtr != 0; FilenamePtr += 1) {
        if (*FilenamePtr == L'\\')
            Slash = FilenamePtr;
    }
    if (Slash)
        *Slash = 0;
#if defined(_WIN64)
    wcscat_s(Filename, sizeof(Filename), L"\\steam_api64.dll");
#else
    wcscat_s(Filename, sizeof(Filename), L"\\steam_api.dll");
#endif
    LoadLibraryW(Filename);
#endif
    return true;
}

bool SteamAPI_RemoveLib(void) {
    if (SteamAPI_IsInitialized() == true)
        SteamAPI_Shutdown();

    return true;
}

/*********************************************************************/
