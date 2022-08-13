#include <stdlib.h>
#include <string.h>

#include "interoplib.h"
#include "interopstub.h"
#include "dictionaryi.h"

#include "steam_api.h"

#include "steamapi.h"
#include "steamuser.h"

/*********************************************************************/

class UserResults {
  public:
    CCallbackManual<UserResults, GetAuthSessionTicketResponse_t, false> AuthSessionTicket;
    void OnAuthSessionTicketResponse(GetAuthSessionTicketResponse_t *Response);

    void *User;
    HAuthTicket Ticket;
    char TicketData[2048];
    uint32 TicketSize;
};

typedef struct SteamUserStruct {
    ClassStruct Class;
    UserResults *Results;
} SteamUserStruct;

/********************************************************************/

static SteamUserStruct *GlobalSteamUser = NULL;

/********************************************************************/
// Callback Functions

void UserResults::OnAuthSessionTicketResponse(GetAuthSessionTicketResponse_t *Response) {
    int32 Successful = FALSE;
    char HexTicket[4096];

    if (Response->m_eResult == k_EResultOK)
        Successful = TRUE;
    if (Response->m_hAuthTicket == this->Ticket) {
        String_ConvertToHex(this->TicketData, this->TicketSize, HexTicket, Element_Count(HexTicket));
        NotificationCenter_FireAfterDelayWithJSON(
            "SteamUser", "AuthSessionTicketResponse", this->User, 0,
            "{ \"ticket\": %d, \"ticketData\": \"%s\", \"successful\": %s, \"errorCode\": %d }", this->Ticket,
            HexTicket, Successful ? "true" : "false", Response->m_eResult);
    }
}

/********************************************************************/
// Concrete Functions

static int32 SteamUser_IsLoggedOn(void) {
    return SteamUser()->BLoggedOn() != 0;
}

static int32 SteamUser_IsBehindNAT(void) {
    return SteamUser()->BIsBehindNAT() != 0;
}

static int32 SteamUser_IsPhoneVerified(void) {
    return SteamUser()->BIsPhoneVerified() != 0;
}

static int32 SteamUser_IsTwoFactorEnabled(void) {
    return SteamUser()->BIsTwoFactorEnabled() != 0;
}

static int32 SteamUser_IsPhoneIdentifying(void) {
    return SteamUser()->BIsPhoneIdentifying() != 0;
}

static int32 SteamUser_IsPhoneRequiringVerification(void) {
    return SteamUser()->BIsPhoneRequiringVerification() != 0;
}

static int32 SteamUser_GetSteamId(uint64 *Result) {
    *Result = SteamUser()->GetSteamID().ConvertToUint64();
    return TRUE;
}

static int32 SteamUser_GetAuthSessionTicket(int32 *Ticket) {
    SteamUserStruct *User = (SteamUserStruct *)GlobalSteamUser;

    User->Results->Ticket = SteamUser()->GetAuthSessionTicket(
        User->Results->TicketData, Element_Count(User->Results->TicketData), &User->Results->TicketSize);

    *Ticket = User->Results->Ticket;
    return TRUE;
}

static int32 SteamUser_CancelAuthTicket(void) {
    SteamUserStruct *User = (SteamUserStruct *)GlobalSteamUser;

    SteamUser()->CancelAuthTicket(User->Results->Ticket);
    return TRUE;
}

static int32 SteamUser_GetPlayerSteamLevel(int32 *PlayerSteamLevel) {
    *PlayerSteamLevel = SteamUser()->GetPlayerSteamLevel();
    return TRUE;
}

static int32 SteamUser_StartVoiceRecording(void) {
    SteamUser()->StartVoiceRecording();
    return TRUE;
}

static int32 SteamUser_StopVoiceRecording(void) {
    SteamUser()->StopVoiceRecording();
    return TRUE;
}

/*********************************************************************/
// Interop Functions

int32 SteamUser_GetInstanceId(char *String, int32 MaxString) {
    SteamUserStruct *User = (SteamUserStruct *)GlobalSteamUser;
    String_CopyLength(String, Class_InstanceId(User), MaxString);
    return TRUE;
}

int32 SteamUser_Process(void *SteamUserContext) {
    // This function is called once per tick and can be used to process simple operations and
    // thread synchronization.
    return TRUE;
}

int32 SteamUser_Invoke(void *SteamUserContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle) {
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    echandle ItemHandle = NULL;
    int64 Value64 = 0;
    int32 RetVal = FALSE;
    int32 ReturnValue = FALSE;
    int32 Value32 = 0;
    char *Method = NULL;
    char *ValueString = NULL;
    char Value64String[120] = {0};

    if (SteamAPI_IsInitialized() == FALSE)
        return FALSE;
    if (IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "method", &Method) == FALSE)
        return FALSE;

    if (String_Compare(Method, "getSteamId") == TRUE) {
        RetVal = SteamUser_GetSteamId((uint64 *)&Value64);
        String_Print(Value64String, Element_Count(Value64String), "%lld", Value64);
        IDictionary_AddString(ReturnDictionaryHandle, "returnValue", Value64String, &ItemHandle);
    } else if (String_Compare(Method, "getAuthSessionTicket") == TRUE) {
        SteamUser_GetAuthSessionTicket(&Value32);
        RetVal = IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    } else if (String_Compare(Method, "cancelAuthTicket") == TRUE) {
        ReturnValue = SteamUser_CancelAuthTicket();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "isLoggedOn") == TRUE) {
        ReturnValue = SteamUser_IsLoggedOn();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "isBehindNAT") == TRUE) {
        ReturnValue = SteamUser_IsBehindNAT();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "isPhoneVerified") == TRUE) {
        ReturnValue = SteamUser_IsPhoneVerified();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "isTwoFactorEnabled") == TRUE) {
        ReturnValue = SteamUser_IsPhoneIdentifying();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "isPhoneIdentifying") == TRUE) {
        ReturnValue = SteamUser_IsBehindNAT();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "isPhoneRequiringVerification") == TRUE) {
        ReturnValue = SteamUser_IsPhoneRequiringVerification();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "getPlayerSteamLevel") == TRUE) {
        RetVal = SteamUser_GetPlayerSteamLevel(&Value32);
        IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    } else if (String_Compare(Method, "startVoiceRecording") == TRUE) {
        ReturnValue = SteamUser_StartVoiceRecording();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "stopVoiceRecording") == TRUE) {
        ReturnValue = SteamUser_StopVoiceRecording();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }

    return RetVal;
}

/*********************************************************************/
// Creation/Deletion Functions

int32 SteamUser_Init(void) {
    SteamUserStruct *User = NULL;

    User = (SteamUserStruct *)malloc(sizeof(SteamUserStruct));
    memset(User, 0, sizeof(SteamUserStruct));
    Interop_GenerateInstanceId(User->Class.InstanceId, 40);

    User->Class.RefCount = 1;

    User->Results = new UserResults();
    User->Results->User = User;
    User->Results->AuthSessionTicket.Register(User->Results, &UserResults::OnAuthSessionTicketResponse);

    GlobalSteamUser = User;
    return TRUE;
}

int32 SteamUser_Remove(void) {
    SteamUserStruct *User = (SteamUserStruct *)GlobalSteamUser;

    if (--User->Class.RefCount == 0) {
        User->Results->AuthSessionTicket.Unregister();
        delete User->Results;
        free(User);
    }

    GlobalSteamUser = NULL;
    return TRUE;
}

/*********************************************************************/
