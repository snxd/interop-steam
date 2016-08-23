#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "steam_api.h"
//#include "steamclientpublic.h"


#include "interop/interoptypes.h"
#include "interop/interop.h"

#include "jansson/jansson.h"

#include "steam/isteamclient.h"
#include "steam/isteamapps.h"

#include "steamauth.h"

/*********************************************************************/
 
#define MAX_GAMEID_LEN 100

typedef struct SteamAuthStruct
{
    // Interop Storage Data
    char                            InstanceId[38];
    void                            *InteropExecuteUserPtr;
    char                            GameId[MAX_GAMEID_LEN];
    int32                           SteamInitialized;
    Interop_ExecuteCallback         InteropExecute;


} SteamAuthStruct;

/********************************************************************/

#define MAX_PARAMETERS (10)

/********************************************************************/
// Interop Functions

void SteamAuth_SetInstanceId(void *SteamAuthContext, char *String)
{
    SteamAuthStruct *SteamAuth = (SteamAuthStruct*)SteamAuthContext;
    strncpy(SteamAuth->InstanceId, String, 38);
}

void SteamAuth_SetInteropExecuteCallback(void *SteamAuthContext, void *UserPtr, Interop_ExecuteCallback Execute)
{
    // Store these if I want to call from native code to javascript
    SteamAuthStruct *SteamAuth = (SteamAuthStruct*)SteamAuthContext;
    SteamAuth->InteropExecuteUserPtr = UserPtr;
    SteamAuth->InteropExecute = Execute;
}

int32 SteamAuth_ExecuteCallback(void *SteamAuthContext, char *MethodBinding, char *MethodResultString, int32 MethodResultStringLength)
{
    SteamAuthStruct *SteamAuth = (SteamAuthStruct*)SteamAuthContext;

    return SteamAuth->InteropExecute(SteamAuth->InteropExecuteUserPtr, SteamAuth->InstanceId, MethodBinding, MethodResultString, MethodResultStringLength);
}

int32 SteamAuth_Init(void *SteamAuthContext)
{
    char GameEnvVar[_MAX_PATH];
    SteamAuthStruct *SteamAuth = (SteamAuthStruct*)SteamAuthContext;
    int32 RetVal = FALSE;
    if (SteamAuth->SteamInitialized == TRUE)
        RetVal = TRUE;
    else if(SteamAuth->GameId[0] != 0)
    {
        sprintf_s(GameEnvVar,_MAX_PATH,"SteamAppId=%s",SteamAuth->GameId);
        _putenv(GameEnvVar);
        sprintf_s(GameEnvVar,_MAX_PATH,"SteamGameId=%s",SteamAuth->GameId);
        _putenv(GameEnvVar);
        if ( SteamAPI_Init() )
        {
            SteamAuth->SteamInitialized=TRUE;
            RetVal = TRUE;
        }
    }
    return RetVal;
}

int32 SteamAuth_InvokeInstance(void *SteamAuthContext, char *InstanceId, char *Method, char *ResultString, int32 ResultStringLength)
{
    // EVERYTHING is marshalled in AND out as a JSON string, use any type supported by JSON and
    // it should marhsal ok.

    SteamAuthStruct *SteamAuth = (SteamAuthStruct*)SteamAuthContext;
    char *MethodName = NULL;
    char MethodResultString[MAX_JSON_STRINGLENGTH];
    int32 MethodResultInt = 0;
    int64 MethodResultLong = 0;
    uint64 MethodResultULong = 0;
    json_t *Parameter[MAX_PARAMETERS];
    char *JSONDumpString = NULL;
    json_t *JSON = NULL;
    json_t *JSONReturnRoot = NULL;
    json_t *JSONReturn = NULL;
    json_t *JSONMethod = NULL;
    json_error_t JSONError;
    int32 RetVal = FALSE;

    if (strcmp(SteamAuth->InstanceId, InstanceId) != 0)
    {
        // Invalid instance ID

        return FALSE;
    }

    memset(MethodResultString, 0, MAX_JSON_STRINGLENGTH);
    JSON = json_loads(Method, MAX_JSON_STRINGLENGTH, &JSONError);
    if (JSON == FALSE)
    {
        return FALSE;
    }

    RetVal = (JSONMethod = json_object_get(JSON, "method")) != NULL;

    if (RetVal == TRUE)
    {
        RetVal = json_is_string(JSONMethod);
    }

    if (RetVal == TRUE)
    {
        MethodName = (char*)json_string_value(JSONMethod);
        RetVal = MethodName != NULL;
    }

    if (RetVal == TRUE && strcmp(MethodName, "setAppId") == 0)
    {
        RetVal = ((Parameter[0] = json_object_get(JSON, "value")) != NULL);
        if (RetVal == TRUE)
        {
            RetVal = json_is_string(Parameter[0]);
        }
        RetVal = RetVal && (SteamAuth->SteamInitialized == FALSE);
        if (RetVal == TRUE)
        {
            RetVal = strlen(json_string_value(Parameter[0])) < MAX_GAMEID_LEN;
        }
        if (RetVal == TRUE)
        {
            strcpy_s(SteamAuth->GameId,MAX_GAMEID_LEN,json_string_value(Parameter[0]));
            RetVal = (JSONReturn = json_null()) != NULL;
        }
    }
    else if (RetVal == TRUE && strcmp(MethodName, "getSteamId") == 0)
    {
        RetVal = SteamAuth_Init(SteamAuthContext);
        if (RetVal == TRUE)
        {
            SteamAuth_GetSteamId(SteamAuth, &MethodResultULong);
            sprintf_s(MethodResultString,MAX_JSON_STRINGLENGTH,"%llu",MethodResultULong);
            RetVal = (JSONReturn = json_string(MethodResultString)) != NULL;
        }
        else
        {
            RetVal = (JSONReturn = json_string("")) != NULL;
        }
    }
    else if (RetVal == TRUE && strcmp(MethodName, "isLoggedOn") == 0)
    {
        RetVal = SteamAuth_Init(SteamAuthContext);
        if (RetVal == TRUE)
        {
            SteamAuth_IsLoggedOn(SteamAuth, &MethodResultInt);
            RetVal = (JSONReturn = (MethodResultInt == TRUE) ? json_true() : json_false()) != NULL;
        }
        else
        {
            RetVal = (JSONReturn =  json_false()) != NULL;
        }
    }
    else if (RetVal == TRUE && strcmp(MethodName, "getSessionTicket") == 0)
    {
        RetVal = SteamAuth_Init(SteamAuthContext);
        if (RetVal == TRUE)
        {
            SteamAuth_GetSessionTicket(SteamAuth, MethodResultString, MAX_JSON_STRINGLENGTH);
            RetVal = (JSONReturn = json_string(MethodResultString)) != NULL;
        }
        else
        {
            RetVal = (JSONReturn = json_string("")) != NULL;
        }
    }

    // Set json return value
    if (RetVal == TRUE)
    {
        RetVal = (JSONReturnRoot = json_object()) != NULL;
    }

    if (RetVal == TRUE)
    {
        RetVal = (json_object_set_new(JSONReturnRoot, "returnValue", JSONReturn) == 0);
    }

    if (RetVal == TRUE)
    {
        RetVal = (JSONDumpString = json_dumps(JSONReturnRoot, 0)) != NULL;
    }

    if (RetVal == TRUE)
    {
        RetVal = ((signed)strlen(JSONDumpString) < ResultStringLength);
    }

    if (RetVal == TRUE)
    {
        strncpy(ResultString, JSONDumpString, ResultStringLength);
    }

    if (JSONDumpString != NULL) { free(JSONDumpString); }
    if (JSONReturnRoot != NULL) { json_decref(JSONReturnRoot); }
    if (JSON != NULL) { json_decref(JSON); }

    return RetVal;
}

int32 SteamAuth_RemoveInstance(void *SteamAuthContext, char *InstanceId)
{
    SteamAuthStruct *SteamAuth = (SteamAuthStruct*)SteamAuthContext;
    if (strcmp(SteamAuth->InstanceId, InstanceId) != 0)
    {
        return FALSE;
    }

    SteamAuth_Delete((void**)&SteamAuth);
    return TRUE;
}

/*********************************************************************/
// Concrete Functions

#define BASE16SYM                                               ("0123456789ABCDEF")

#define Base16_EncodeLo(b)                                      (BASE16SYM[(b & 0xff) >> 4])
#define Base16_EncodeHi(b)                                      (BASE16SYM[b & 0xF])

int32 ConvertToHex(char *Binary, int32 BinarySize, char *Hex, int32 MaxHex)
{
    int32 ReturnVal = TRUE;

    while ((MaxHex > 2) && (BinarySize > 0))
        {
        Hex[0] = Base16_EncodeLo(*Binary);
        Hex[1] = Base16_EncodeHi(*Binary);

        Hex += 2;
        MaxHex -= 2;
        Binary += 1;
        BinarySize -= 1;
        }

    if (BinarySize > 0)
        ReturnVal = FALSE;

    //Debug_Assert(MaxHex > 0, "Unable to null terminate hex string");
    *Hex = 0;
    return ReturnVal;
}

void SteamAuth_IsLoggedOn(void *SteamAuthContext, int32 *Result)
{
    *Result = SteamUser()->BLoggedOn() != 0;
}

void SteamAuth_GetSteamId(void *SteamAuthContext, uint64 *Result)
{
    CSteamID id = SteamUser()->GetSteamID();
    *Result = id.ConvertToUint64();
}

void SteamAuth_GetSessionTicket(void *SteamAuthContext, char *HexTicket, int32 HexTicketLength)
{
    // These aren't the exact lengths of the ticket data - We need to look up the max values for GetAuthSessionTicket
    // ResultString and MaxResultString length are 
    char AuthTicketData[2048] = { 0 };
    uint32 AuthTicketSize = 0;

    // TODO: Check to see if SteamUser can return NULL if the client isn't connected
    SteamUser()->GetAuthSessionTicket(AuthTicketData, sizeof(AuthTicketData), &AuthTicketSize);
    ConvertToHex(AuthTicketData, AuthTicketSize, HexTicket, HexTicketLength); 
}


/*********************************************************************/
// Creation/Deletion Functions

void SteamAuth_Create(void **SteamAuthContext)
{
    SteamAuthStruct *SteamAuth = NULL;

    SteamAuth = (SteamAuthStruct*)malloc(sizeof(SteamAuthStruct));
    SteamAuth->GameId[0]=0;
    SteamAuth->SteamInitialized=FALSE;
    *SteamAuthContext = SteamAuth;
}

void SteamAuth_Delete(void **SteamAuthContext)
{
    SteamAuthStruct *SteamAuth = (SteamAuthStruct*)*SteamAuthContext;
    if(SteamAuth->SteamInitialized == TRUE)
    {
        SteamAPI_Shutdown();
    }
    free(SteamAuth);
    
    *SteamAuthContext = NULL;
}

/*********************************************************************/
}