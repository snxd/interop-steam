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

namespace {

bool g_steam_is_initialized = false;

bool Initialize() {
    if (SteamAPI_IsSteamRunning())
        g_steam_is_initialized = SteamAPI_Init();
    else
        g_steam_is_initialized = false;

    return g_steam_is_initialized;
}

bool SetAppId(int32_t app_id) {
    char env_var[320]{};

    if (SteamAPI_IsInitialized())
        return false;
    if (app_id == 0)
        return false;

    snprintf(env_var, sizeof(env_var), "SteamAppId=%d", app_id);
    putenv(env_var);
    snprintf(env_var, sizeof(env_var), "SteamGameId=%d", app_id);
    putenv(env_var);
    return true;
}

}  // namespace

/********************************************************************/
// Concrete functions

bool SteamAPI_IsInitialized() {
    return g_steam_is_initialized;
}

/*********************************************************************/
// Interop functions

bool SteamAPI_Process(void *handle) {
    // This function is called once per tick and can be used to process simple operations and
    // thread synchronization.

    SteamAPI_RunCallbacks();
    return true;
}

bool SteamAPI_Invoke(void *handle, echandle method_dictionary_handle, echandle return_dictionary_handle) {
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.
    bool ret = false;
    const char *method = nullptr;

    if (!IDictionary_GetStringPtrByKey(method_dictionary_handle, "method", &method))
        return false;

    if (strcmp(method, "initialize") == 0) {
        const auto return_value = Initialize();
        ret = IDictionary_AddBoolean(return_dictionary_handle, "returnValue", return_value, nullptr);
    } else if (strcmp(method, "isSteamRunning") == 0) {
        const auto is_running = SteamAPI_IsSteamRunning();
        ret = IDictionary_AddBoolean(return_dictionary_handle, "returnValue", is_running, nullptr);
    } else if (strcmp(method, "isInitialized") == 0) {
        const auto is_initialized = SteamAPI_IsInitialized();
        ret = IDictionary_AddBoolean(return_dictionary_handle, "returnValue", is_initialized, nullptr);
    } else if (strcmp(method, "restartAppIfNecessary") == 0) {
        bool return_value = false;
        int32_t value = 0;
        ret = IDictionary_GetInt32ByKey(method_dictionary_handle, "id", &value);
        if (ret)
            return_value = SteamAPI_RestartAppIfNecessary(value);
        IDictionary_AddBoolean(return_dictionary_handle, "returnValue", return_value, nullptr);
    } else if (strcmp(method, "setAppId") == 0) {
        bool return_value = false;
        int32_t value = 0;
        ret = IDictionary_GetInt32ByKey(method_dictionary_handle, "id", &value);
        if (ret)
            return_value = SetAppId(value);
        IDictionary_AddBoolean(return_dictionary_handle, "returnValue", return_value, nullptr);
    }

    return ret;
}

/*********************************************************************/
// Global initialization functions

bool SteamAPI_InitLib(void) {
    wchar_t filename[320]{};
    wchar_t *filename_ptr = nullptr;
    wchar_t *slash = 0;

#if defined(_WIN32)
    GetModuleFileNameW(nullptr, filename, sizeof(filename));

    for (filename_ptr = filename; *filename_ptr != 0; filename_ptr += 1) {
        if (*filename_ptr == L'\\')
            slash = filename_ptr;
    }
    if (slash)
        *slash = 0;
#if defined(_WIN64)
    wcscat_s(filename, _countof(filename), L"\\steam_api64.dll");
#else
    wcscat_s(filename, _countof(filename), L"\\steam_api.dll");
#endif
    LoadLibraryW(filename);
#endif
    return true;
}

bool SteamAPI_RemoveLib(void) {
    if (SteamAPI_IsInitialized())
        SteamAPI_Shutdown();

    return true;
}

/*********************************************************************/
