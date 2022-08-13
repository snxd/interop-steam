#include <stdlib.h>
#include <string.h>

#include "interoplib.h"
#include "interopstub.h"
#include "dictionaryi.h"

#if defined(_WIN32)
#include <windows.h>
#endif

#include "steam_api.h"

#include "steamapi.h"

/*********************************************************************/

static int32 GlobalSteamIsInitialized = FALSE;

/********************************************************************/
// Concrete Functions

static int32 SteamAPI_InitializeInt()
{
    if (SteamAPI_IsSteamRunning() == TRUE)
    {
        if (SteamAPI_Init())
            GlobalSteamIsInitialized = TRUE;
        else
            GlobalSteamIsInitialized = FALSE;
    }
    else
        GlobalSteamIsInitialized = FALSE;

    return GlobalSteamIsInitialized;
}

int32 SteamAPI_IsInitialized()
{
    return GlobalSteamIsInitialized;
}

static int32 SteamAPI_SetAppId(int32 AppId)
{
    char EnvVar[320] = { 0 };

    if (SteamAPI_IsInitialized() == TRUE)
        return FALSE;
    if (AppId == 0)
        return FALSE;

    String_Print(EnvVar, Element_Count(EnvVar), "SteamAppId=%d", AppId);
    _putenv(EnvVar);
    String_Print(EnvVar, Element_Count(EnvVar), "SteamGameId=%d", AppId);
    _putenv(EnvVar);
    return TRUE;
}

/*********************************************************************/
// Interop Functions

int32 SteamAPI_Process(void *SteamAPIContext)
{
    // This function is called once per tick and can be used to process simple operations and
    // thread synchronization.

    SteamAPI_RunCallbacks();
    return TRUE;
}

int32 SteamAPI_Invoke(void *SteamAPIContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle)
{
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    echandle ItemHandle = NULL;
    int32 RetVal = FALSE;
    int32 ReturnValue = FALSE;
    int32 Value32 = 0;
    char *Method = NULL;
    char *ValueString = NULL;


    if (IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "method", &Method) == FALSE)
        return FALSE;

    if (String_Compare(Method, "initialize") == TRUE)
    {
        ReturnValue = SteamAPI_InitializeInt();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }
    else if (String_Compare(Method, "isSteamRunning") == TRUE)
    {
        ReturnValue = (int32)SteamAPI_IsSteamRunning();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }
    else if (String_Compare(Method, "isInitialized") == TRUE)
    {
        ReturnValue = SteamAPI_IsInitialized();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }
    else if (String_Compare(Method, "restartAppIfNecessary") == TRUE)
    {
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "id", &Value32);
        if (RetVal == TRUE)
            ReturnValue = (int32)SteamAPI_RestartAppIfNecessary(Value32);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }
    else if (String_Compare(Method, "setAppId") == TRUE)
    {
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "id", &Value32);
        if (RetVal == TRUE)
            ReturnValue = SteamAPI_SetAppId(Value32);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }

    return RetVal;
}

/*********************************************************************/
// Creation/Deletion Functions

int32 SteamAPI_InitLib(void)
{
    wchar_t Filename[320] = {0};
    wchar_t *FilenamePtr = NULL;
    wchar_t *Slash = 0;

#if defined(_WIN32)
    GetModuleFileNameW(NULL, Filename, Element_Count(Filename));

    for (FilenamePtr = Filename; *FilenamePtr != 0; FilenamePtr += 1)
    {
        if (*FilenamePtr == L'\\')
            Slash = FilenamePtr;
    }
    *Slash = 0;
    wcscat_s(Filename, Element_Count(Filename), L"\\steam_api.dll");
    LoadLibraryW(Filename);
#endif
    return TRUE;
}

int32 SteamAPI_RemoveLib(void)
{
    if (SteamAPI_IsInitialized() == TRUE)
        SteamAPI_Shutdown();

    return TRUE;
}

/*********************************************************************/
