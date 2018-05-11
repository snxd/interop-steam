#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "interoplib.h"
#include "interopstub.h"
#include "dictionaryi.h"

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

int32 SteamUser_IsBehindNAT(void *SteamUserContext)
{
    SteamUserStruct *User = (SteamUserStruct*)SteamUserContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    return SteamUser()->BIsBehindNAT() != 0;
}

int32 SteamUser_IsPhoneVerified(void *SteamUserContext)
{
    SteamUserStruct *User = (SteamUserStruct*)SteamUserContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    return SteamUser()->BIsPhoneVerified() != 0;
}

int32 SteamUser_IsTwoFactorEnabled(void *SteamUserContext)
{
    SteamUserStruct *User = (SteamUserStruct*)SteamUserContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    return SteamUser()->BIsTwoFactorEnabled() != 0;
}

int32 SteamUser_IsPhoneIdentifying(void *SteamUserContext)
{
    SteamUserStruct *User = (SteamUserStruct*)SteamUserContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    return SteamUser()->BIsPhoneIdentifying() != 0;
}

int32 SteamUser_IsPhoneRequiringVerification(void *SteamUserContext)
{
    SteamUserStruct *User = (SteamUserStruct*)SteamUserContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    return SteamUser()->BIsPhoneRequiringVerification() != 0;
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

int32 SteamUser_GetPlayerSteamLevel(void *SteamUserContext, int32 *PlayerSteamLevel)
{
    SteamUserStruct *User = (SteamUserStruct*)SteamUserContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    *PlayerSteamLevel = SteamUser()->GetPlayerSteamLevel();
    return TRUE;
}

int32 SteamUser_StartVoiceRecording(void *SteamUserContext)
{
    SteamUserStruct *User = (SteamUserStruct*)SteamUserContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    SteamUser()->StartVoiceRecording();
    return TRUE;
}

int32 SteamUser_StopVoiceRecording(void *SteamUserContext)
{
    SteamUserStruct *User = (SteamUserStruct*)SteamUserContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    SteamUser()->StopVoiceRecording();
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

int32 SteamUser_Invoke(void *SteamUserContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle)
{
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    SteamUserStruct *User = (SteamUserStruct*)SteamUserContext;
    echandle ItemHandle = NULL;
    int64 Value64 = 0;
    int32 RetVal = FALSE;
    int32 ReturnValue = FALSE;
    int32 Value32 = 0;
    char *Method = NULL;
    char *ValueString = NULL;
    char Value64String[120] = { 0 };

    if (IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "method", &Method) == FALSE)
        return FALSE;

    if (String_Compare(Method, "getSteamId") == TRUE)
    {
        RetVal = SteamUser_GetSteamId(SteamUserContext, (uint64 *)&Value64);
        String_Print(Value64String, Element_Count(Value64String), "%lld", Value64);
        IDictionary_AddString(ReturnDictionaryHandle, "returnValue", Value64String, &ItemHandle);
    }
    else if (String_Compare(Method, "getAuthSessionTicket") == TRUE)
    {
        SteamUser_GetAuthSessionTicket(User, &Value32);
        RetVal = IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    }
    else if (String_Compare(Method, "cancelAuthTicket") == TRUE)
    {
        ReturnValue = SteamUser_CancelAuthTicket(User);
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }
    else if (String_Compare(Method, "isLoggedOn") == TRUE)
    {
        ReturnValue = SteamUser_IsLoggedOn(User);
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }
    else if (String_Compare(Method, "isBehindNAT") == TRUE)
    {
        ReturnValue = SteamUser_IsBehindNAT(User);
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }
    else if (String_Compare(Method, "isPhoneVerified") == TRUE)
    {
        ReturnValue = SteamUser_IsPhoneVerified(User);
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }
    else if (String_Compare(Method, "isTwoFactorEnabled") == TRUE)
    {
        ReturnValue = SteamUser_IsPhoneIdentifying(User);
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }
    else if (String_Compare(Method, "isPhoneIdentifying") == TRUE)
    {
        ReturnValue = SteamUser_IsBehindNAT(User);
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }
    else if (String_Compare(Method, "isPhoneRequiringVerification") == TRUE)
    {
        ReturnValue = SteamUser_IsPhoneRequiringVerification(User);
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }
    else if (String_Compare(Method, "getPlayerSteamLevel") == TRUE)
    {
        RetVal = SteamUser_GetPlayerSteamLevel(User, &Value32);
        IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    }
    else if (String_Compare(Method, "startVoiceRecording") == TRUE)
    {
        ReturnValue = SteamUser_StartVoiceRecording(User);
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }
    else if (String_Compare(Method, "stopVoiceRecording") == TRUE)
    {
        ReturnValue = SteamUser_StopVoiceRecording(User);
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }

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
