#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "interoplib.h"
#include "interopstub.h"

#include "jansson.h"
#include "jansson_private.h"

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

int32 SteamApp_Invoke(void *SteamAppContext, char *Method, char *ResultString, int32 ResultStringLength)
{
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    SteamAppStruct *SteamApp = (SteamAppStruct *)GlobalSteamApp;
    char *MethodName = NULL;
    char MethodResultString[INTEROP_MAXSTRING] = { 0 };
    int32 MethodResultInt = 0;
    int64 MethodResultLong = 0;
    uint64 MethodResultULong = 0;
    json_t *Parameter[1];
    char *JSONDumpString = NULL;
    json_t *JSON = NULL;
    json_t *JSONReturnRoot = NULL;
    json_t *JSONReturn = NULL;
    json_t *JSONMethod = NULL;
    json_error_t JSONError;
    int32 RetVal = FALSE;


    JSON = json_loads(Method, INTEROP_MAXSTRING, &JSONError);
    if (JSON == FALSE)
        return FALSE;

    RetVal = (JSONMethod = json_object_get(JSON, "method")) != NULL;

    if (RetVal == TRUE)
        RetVal = json_is_string(JSONMethod);

    if (RetVal == TRUE)
    {
        MethodName = (char*)json_string_value(JSONMethod);
        RetVal = MethodName != NULL;
    }

    if (RetVal == TRUE && String_Compare(MethodName, "setId") == TRUE)
    {
        RetVal = ((Parameter[0] = json_object_get(JSON, "value")) != NULL);
        if (RetVal == TRUE)
            RetVal = json_is_string(Parameter[0]);
        if (RetVal == TRUE)
            MethodResultInt = SteamApp_SetId(SteamApp, (char *)json_string_value(Parameter[0]));
        RetVal = (JSONReturn = json_boolean(MethodResultInt)) != NULL;
    }

    // Set json return value
    if (RetVal == TRUE)
        RetVal = (JSONReturnRoot = json_object()) != NULL;
    if (RetVal == TRUE)
        RetVal = (json_object_set_new(JSONReturnRoot, "returnValue", JSONReturn) == 0);
    if (RetVal == TRUE)
        RetVal = (JSONDumpString = json_dumps(JSONReturnRoot, 0)) != NULL;
    if (RetVal == TRUE)
        RetVal = ((signed)String_Length(JSONDumpString) < ResultStringLength);
    if (RetVal == TRUE)
        String_CopyLength(ResultString, JSONDumpString, ResultStringLength);

    if (JSONDumpString != NULL)
        free(JSONDumpString);
    if (JSONReturnRoot != NULL)
        json_decref(JSONReturnRoot);
    if (JSON != NULL)
        json_decref(JSON);

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
