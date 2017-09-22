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

#include "steamapp.h"

/*********************************************************************/
 
typedef struct SteamAppStruct
{
    ClassStruct                     Class;
    int32                           IsInitialized;
} SteamAppStruct;

/********************************************************************/

static SteamAppStruct *GlobalSteamApp = NULL;

/********************************************************************/
// Concrete Functions

int32 SteamApp_IsInitialized(void)
{
    SteamAppStruct *SteamApp = (SteamAppStruct *)GlobalSteamApp;
    return SteamApp->IsInitialized;
}

int32 SteamApp_SetId(void *SteamAppContext, char *AppId)
{
    SteamAppStruct *SteamApp = (SteamAppStruct *)GlobalSteamApp;
    int32 RetVal = SteamApp->IsInitialized;
    int32 Result = 0;
    char EnvVar[320] = { 0 };

    if (RetVal == FALSE && AppId[0] != 0)
    {
        String_Print(EnvVar, Element_Count(EnvVar), "SteamAppId=%s", AppId);
        _putenv(EnvVar);
        String_Print(EnvVar, Element_Count(EnvVar), "SteamGameId=%s", AppId);
        _putenv(EnvVar);

        if (SteamAPI_IsSteamRunning() == 0)
        {
            RetVal = FALSE;
        }
        else if (SteamAPI_Init())
        {
            SteamApp->IsInitialized = TRUE;
            RetVal = TRUE;
        }
    }

    return RetVal;
}

/*********************************************************************/
// Interop Functions

int32 SteamApp_GetInstanceId(char *String, int32 MaxString)
{
    SteamAppStruct *SteamApp = (SteamAppStruct *)GlobalSteamApp;
    String_CopyLength(String, Class_InstanceId(SteamApp), MaxString);
    return TRUE;
}

int32 SteamApp_Process(void *SteamAppContext)
{
    // This function is called once per tick and can be used to process simple operations and
    // thread synchronization.

    SteamAPI_RunCallbacks();
    return TRUE;
}

int32 SteamApp_Invoke(void *SteamAppContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle)
{
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    SteamAppStruct *SteamApp = (SteamAppStruct *)GlobalSteamApp;
    echandle ItemHandle = NULL;
    int32 RetVal = FALSE;
    int32 ReturnValue = FALSE;
    char *Method = NULL;
    char *ValueString = NULL;


    if (IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "method", &Method) == FALSE)
        return FALSE;

    if (String_Compare(Method, "setId") == TRUE)
    {
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "value", &ValueString);
        if (RetVal == TRUE)
            ReturnValue = SteamApp_SetId(SteamApp, ValueString);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }

    return RetVal;
}

/*********************************************************************/
// Creation/Deletion Functions

int32 SteamApp_Init(void)
{
    SteamAppStruct *SteamApp = NULL;
    wchar_t Filename[320] = {0};
    wchar_t *FilenamePtr = NULL;
    wchar_t *Slash = 0;

    SteamApp = (SteamAppStruct *)malloc(sizeof(SteamAppStruct));
    memset(SteamApp, 0, sizeof(SteamAppStruct));
    Interop_GenerateInstanceId(SteamApp->Class.InstanceId, 40);

    SteamApp->IsInitialized = FALSE;

#if defined(_WIN32)
    GetModuleFileNameW(NULL, Filename, Element_Count(Filename));

    for (FilenamePtr = Filename; *FilenamePtr != 0; FilenamePtr += 1)
    {
        if (*FilenamePtr == '\\')
            Slash = FilenamePtr;
    }
    *Slash = 0;
    wcscat_s(Filename, Element_Count(Filename), L"\\steam_api.dll");
    LoadLibrary(Filename);
#endif

    GlobalSteamApp = SteamApp;
    return TRUE;
}

int32 SteamApp_Remove(void)
{
    SteamAppStruct *SteamApp = (SteamAppStruct *)GlobalSteamApp;


    if (SteamApp->IsInitialized == TRUE)
        SteamAPI_Shutdown();

    GlobalSteamApp = NULL;
    free(SteamApp);

    return TRUE;
}

/*********************************************************************/
