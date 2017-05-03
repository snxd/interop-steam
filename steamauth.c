#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "interoplib.h"
#include "interopstub.h"

#include "jansson.h"
#include "jansson_private.h"

#include "steam_api.h"
#include "isteamclient.h"
#include "isteamapps.h"

#include "steamauth.h"

/*********************************************************************/
 
#define STEAMAUTH_MAXGAMEIDLENGTH       (100)
#define STEAMAUTH_MAXPARAMETERS         (10)

/*********************************************************************/

typedef struct SteamAuthStruct
{
    ClassStruct                     Class;
    void                            *InteropExecuteUserPtr;
    Interop_ExecuteCallback         InteropExecute;
    char                            GameId[STEAMAUTH_MAXGAMEIDLENGTH];
    int32                           SteamInitialized;
} SteamAuthStruct;

/********************************************************************/
// Concrete Functions

int32 SteamAuth_IsLoggedOn(void *SteamAuthContext, int32 *Result)
{
    *Result = SteamUser()->BLoggedOn() != 0;
    return TRUE;
}

int32 SteamAuth_GetSteamId(void *SteamAuthContext, uint64 *Result)
{
    CSteamID id = SteamUser()->GetSteamID();
    *Result = id.ConvertToUint64();
    return TRUE;
}

int32 SteamAuth_GetSessionTicket(void *SteamAuthContext, char *HexTicket, int32 HexTicketLength)
{
    // These aren't the exact lengths of the ticket data - We need to look up the max values for GetAuthSessionTicket
    // ResultString and MaxResultString length are 
    char AuthTicketData[2048] = { 0 };
    uint32 AuthTicketSize = 0;

    // TODO: Check to see if SteamUser can return NULL if the client isn't connected
    SteamUser()->GetAuthSessionTicket(AuthTicketData, sizeof(AuthTicketData), &AuthTicketSize);
    String_ConvertToHex(AuthTicketData, AuthTicketSize, HexTicket, HexTicketLength);
    return TRUE;
}

/*********************************************************************/
// Interop Functions

static int32 SteamAuth_ExecuteCallback(void *SteamAuthContext, char *MethodBinding, char *MethodResultString, int32 MethodResultStringLength)
{
    SteamAuthStruct *SteamAuth = (SteamAuthStruct*)SteamAuthContext;
    if (SteamAuth->InteropExecute(SteamAuth->InteropExecuteUserPtr, Class_InstanceId(SteamAuth), MethodBinding, MethodResultString, MethodResultStringLength) == FALSE)
        return FALSE;
    return TRUE;
}

int32 SteamAuth_GetInstanceId(void *SteamAuthContext, char *String, int32 MaxString)
{
    SteamAuthStruct *SteamAuth = (SteamAuthStruct *)SteamAuthContext;
    String_CopyLength(String, Class_InstanceId(SteamAuth), MaxString);
    return TRUE;
}

int32 SteamAuth_SetInteropExecuteCallback(void *SteamAuthContext, void *UserPtr, Interop_ExecuteCallback Execute)
{
    // Store these if I want to call from native code to javascript
    SteamAuthStruct *SteamAuth = (SteamAuthStruct*)SteamAuthContext;
    SteamAuth->InteropExecuteUserPtr = UserPtr;
    SteamAuth->InteropExecute = Execute;
    return TRUE;
}

int32 SteamAuth_Process(void *SteamAuthContext)
{
    // This function is called once per tick and can be used to process simple operations and
    // thread synchronization.

    return TRUE;
}

int32 SteamAuth_Init(void *SteamAuthContext)
{
    SteamAuthStruct *SteamAuth = (SteamAuthStruct*)SteamAuthContext;
    int32 RetVal = SteamAuth->SteamInitialized;
    char GameEnvVar[320] = { 0 };

    if (RetVal == FALSE && SteamAuth->GameId[0] != 0)
    {
        String_Print(GameEnvVar, Element_Count(GameEnvVar), "SteamAppId=%s", SteamAuth->GameId);
        _putenv(GameEnvVar);
        String_Print(GameEnvVar, Element_Count(GameEnvVar), "SteamGameId=%s", SteamAuth->GameId);
        _putenv(GameEnvVar);

        if (SteamAPI_Init())
        {
            SteamAuth->SteamInitialized = TRUE;
            RetVal = TRUE;
        }
    }
    return RetVal;
}

int32 SteamAuth_Invoke(void *SteamAuthContext, char *Method, char *ResultString, int32 ResultStringLength)
{
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    SteamAuthStruct *SteamAuth = (SteamAuthStruct*)SteamAuthContext;
    char *MethodName = NULL;
    char MethodResultString[INTEROP_MAXSTRING];
    int32 MethodResultInt = 0;
    int64 MethodResultLong = 0;
    uint64 MethodResultULong = 0;
    json_t *Parameter[STEAMAUTH_MAXPARAMETERS];
    char *JSONDumpString = NULL;
    json_t *JSON = NULL;
    json_t *JSONReturnRoot = NULL;
    json_t *JSONReturn = NULL;
    json_t *JSONMethod = NULL;
    json_error_t JSONError;
    int32 RetVal = FALSE;


    memset(MethodResultString, 0, INTEROP_MAXSTRING);
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

    if (RetVal == TRUE && String_Compare(MethodName, "setAppId") == TRUE)
    {
        RetVal = ((Parameter[0] = json_object_get(JSON, "value")) != NULL);
        if (RetVal == TRUE)
            RetVal = json_is_string(Parameter[0]);
        RetVal = RetVal && (SteamAuth->SteamInitialized == FALSE);
        if (RetVal == TRUE)
            RetVal = String_Length(json_string_value(Parameter[0])) < STEAMAUTH_MAXGAMEIDLENGTH;
        if (RetVal == TRUE)
        {
            String_CopyLength(SteamAuth->GameId, json_string_value(Parameter[0]), Element_Count(SteamAuth->GameId));
            RetVal = (JSONReturn = json_null()) != NULL;
        }
    }
    else if (RetVal == TRUE && String_Compare(MethodName, "getSteamId") == TRUE)
    {
        RetVal = SteamAuth_Init(SteamAuthContext);
        if (RetVal == TRUE)
        {
            SteamAuth_GetSteamId(SteamAuth, &MethodResultULong);
            String_Print(MethodResultString, Element_Count(MethodResultString), "%llu", MethodResultULong);
            RetVal = (JSONReturn = json_string(MethodResultString)) != NULL;
        }
        else
        {
            RetVal = (JSONReturn = json_string("")) != NULL;
        }
    }
    else if (RetVal == TRUE && String_Compare(MethodName, "isLoggedOn") == TRUE)
    {
        RetVal = SteamAuth_Init(SteamAuthContext);
        if (RetVal == TRUE)
            SteamAuth_IsLoggedOn(SteamAuth, &MethodResultInt);
        RetVal = (JSONReturn = json_boolean(MethodResultInt == TRUE)) != NULL;
    }
    else if (RetVal == TRUE && String_Compare(MethodName, "getSessionTicket") == TRUE)
    {
        RetVal = SteamAuth_Init(SteamAuthContext);
        if (RetVal == TRUE)
        {
            SteamAuth_GetSessionTicket(SteamAuth, MethodResultString, Element_Count(MethodResultString));
            RetVal = (JSONReturn = json_string(MethodResultString)) != NULL;
        }
        else
        {
            RetVal = (JSONReturn = json_string("")) != NULL;
        }
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

int32 SteamAuth_Create(void **SteamAuthContext)
{
    SteamAuthStruct *SteamAuth = NULL;

    SteamAuth = (SteamAuthStruct*)malloc(sizeof(SteamAuthStruct));
    Interop_GenerateInstanceId(SteamAuth->Class.InstanceId, 40);

    SteamAuth->Class.RefCount = 1;
    SteamAuth->GameId[0] = 0;
    SteamAuth->SteamInitialized = FALSE;

    *SteamAuthContext = SteamAuth;
    return TRUE;
}

void *SteamAuth_AddRef(void *SteamAuthContext)
{
    SteamAuthStruct *SteamAuth = (SteamAuthStruct *)SteamAuthContext;
    SteamAuth->Class.RefCount += 1;
    return SteamAuth;
}

int32 SteamAuth_Release(void **SteamAuthContext)
{
    SteamAuthStruct *SteamAuth = (SteamAuthStruct *)*SteamAuthContext;

    if (--SteamAuth->Class.RefCount == 0)
    {
        if (SteamAuth->SteamInitialized == TRUE)
            SteamAPI_Shutdown();
        free(SteamAuth);
    }

    *SteamAuthContext = NULL;
    return TRUE;
}

/*********************************************************************/
