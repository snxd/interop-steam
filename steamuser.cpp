#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "interoplib.h"
#include "interopstub.h"

#include "jansson.h"
#include "jansson_private.h"

#include "steam_api.h"

#include "steamapp.h"
#include "steamuser.h"

/*********************************************************************/

class UserResults
{
public:
    CCallbackManual<UserResults, GetAuthSessionTicketResponse_t, false> AuthSessionTicket;
    void OnAuthSessionTicketResponse(GetAuthSessionTicketResponse_t *Response);

    void                *User;
    HAuthTicket         Ticket;
    char                TicketData[2048];
    uint32              TicketSize;
};

typedef struct SteamUserStruct
{
    ClassStruct         Class;
    UserResults          *Results;
} SteamUserStruct;

/********************************************************************/
// Callback Functions

void UserResults::OnAuthSessionTicketResponse(GetAuthSessionTicketResponse_t *Response)
{
    int32 Successful = FALSE;
    char HexTicket[4096];

    if (Response->m_eResult == k_EResultOK)
        Successful = TRUE;
    if (Response->m_hAuthTicket == this->Ticket)
    {
        String_ConvertToHex(this->TicketData, this->TicketSize, HexTicket, Element_Count(HexTicket));
        NotificationCenter_FireAfterDelayWithJSON("SteamUser", "AuthSessionTicketResponse", this->User, 0,
            "{ \"ticket\": %d, \"ticketData\": \"%s\", \"successful\": %s, \"errorCode\": %d }",
            this->Ticket, HexTicket, Successful ? "true" : "false", Response->m_eResult);
    }
}

/********************************************************************/
// Concrete Functions

int32 SteamUser_IsLoggedOn(void *SteamUserContext)
{
    SteamUserStruct *User = (SteamUserStruct*)SteamUserContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    return SteamUser()->BLoggedOn() != 0;
}

int32 SteamUser_GetSteamId(void *SteamUserContext, uint64 *Result)
{
    SteamUserStruct *User = (SteamUserStruct*)SteamUserContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;

    *Result = SteamUser()->GetSteamID().ConvertToUint64();
    return TRUE;
}

int32 SteamUser_GetAuthSessionTicket(void *SteamUserContext, int32 *Ticket)
{
    SteamUserStruct *User = (SteamUserStruct*)SteamUserContext;

    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;

    User->Results->Ticket = SteamUser()->GetAuthSessionTicket(User->Results->TicketData, 
        Element_Count(User->Results->TicketData), &User->Results->TicketSize);

    *Ticket = User->Results->Ticket;
    return TRUE;
}

int32 SteamUser_CancelAuthTicket(void *SteamUserContext)
{
    SteamUserStruct *User = (SteamUserStruct*)SteamUserContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    SteamUser()->CancelAuthTicket(User->Results->Ticket);
    return TRUE;
}

/*********************************************************************/
// Interop Functions

int32 SteamUser_GetInstanceId(void *SteamUserContext, char *String, int32 MaxString)
{
    SteamUserStruct *User = (SteamUserStruct *)SteamUserContext;
    String_CopyLength(String, Class_InstanceId(User), MaxString);
    return TRUE;
}

int32 SteamUser_Process(void *SteamUserContext)
{
    // This function is called once per tick and can be used to process simple operations and
    // thread synchronization.
    return TRUE;
}

int32 SteamUser_Invoke(void *SteamUserContext, char *Method, char *ResultString, int32 ResultStringLength)
{
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    SteamUserStruct *User = (SteamUserStruct*)SteamUserContext;
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

    if (RetVal == TRUE && String_Compare(MethodName, "getSteamId") == TRUE)
    {
        SteamUser_GetSteamId(User, &MethodResultULong);
        RetVal = (JSONReturn = json_integer(MethodResultULong)) != NULL;
    }
    else if (RetVal == TRUE && String_Compare(MethodName, "isLoggedOn") == TRUE)
    {
        MethodResultInt = SteamUser_IsLoggedOn(User);
        RetVal = (JSONReturn = json_boolean(MethodResultInt)) != NULL;
    }
    else if (RetVal == TRUE && String_Compare(MethodName, "getAuthSessionTicket") == TRUE)
    {
        SteamUser_GetAuthSessionTicket(User, &MethodResultInt);
        RetVal = (JSONReturn = json_integer(MethodResultInt)) != NULL;
    }
    else if (RetVal == TRUE && String_Compare(MethodName, "cancelAuthTicket") == TRUE)
    {
        MethodResultInt = SteamUser_CancelAuthTicket(User);
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

int32 SteamUser_Create(void **SteamUserContext)
{
    SteamUserStruct *User = NULL;

    User = (SteamUserStruct*)malloc(sizeof(SteamUserStruct));
    memset(User, 0, sizeof(SteamUserStruct));
    Interop_GenerateInstanceId(User->Class.InstanceId, 40);

    User->Class.RefCount = 1;

    User->Results = new UserResults();
    User->Results->User = User;
    User->Results->AuthSessionTicket.Register(User->Results, &UserResults::OnAuthSessionTicketResponse);

    *SteamUserContext = User;
    return TRUE;
}

void *SteamUser_AddRef(void *SteamUserContext)
{
    SteamUserStruct *User = (SteamUserStruct *)SteamUserContext;
    User->Class.RefCount += 1;
    return User;
}

int32 SteamUser_Release(void **SteamUserContext)
{
    SteamUserStruct *User = (SteamUserStruct *)*SteamUserContext;

    if (--User->Class.RefCount == 0)
    {
        User->Results->AuthSessionTicket.Unregister();
        delete User->Results;
        free(User);
    }

    *SteamUserContext = NULL;
    return TRUE;
}

/*********************************************************************/
