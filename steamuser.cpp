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

    void *User = NULL;
    HAuthTicket Ticket = k_HAuthTicketInvalid;
    char TicketData[2048] = {0};
    uint32_t TicketSize = 0;
};

typedef struct SteamUserStruct {
    ClassStruct Class;
    UserResults *Results;
} SteamUserStruct;

/********************************************************************/

static SteamUserStruct *GlobalSteamUser = NULL;

/********************************************************************/
// Callback functions

void UserResults::OnAuthSessionTicketResponse(GetAuthSessionTicketResponse_t *Response) {
    int32_t Successful = false;
    char HexTicket[4096];

    if (Response->m_eResult == k_EResultOK)
        Successful = true;
    if (Response->m_hAuthTicket == this->Ticket) {
        String_ConvertToHex(this->TicketData, this->TicketSize, HexTicket, Element_Count(HexTicket));
        NotificationCenter_FireAfterDelayWithJSON(
            "SteamUser", "AuthSessionTicketResponse", this->User, 0,
            "{ \"ticket\": %d, \"ticketData\": \"%s\", \"successful\": %s, \"errorCode\": %d }", this->Ticket,
            HexTicket, Successful ? "true" : "false", Response->m_eResult);
    }
}

/********************************************************************/
// Concrete functions

static bool SteamUser_IsLoggedOn(void) {
    return SteamUser()->BLoggedOn() != 0;
}

static bool SteamUser_IsBehindNAT(void) {
    return SteamUser()->BIsBehindNAT() != 0;
}

static bool SteamUser_IsPhoneVerified(void) {
    return SteamUser()->BIsPhoneVerified() != 0;
}

static bool SteamUser_IsTwoFactorEnabled(void) {
    return SteamUser()->BIsTwoFactorEnabled() != 0;
}

static bool SteamUser_IsPhoneIdentifying(void) {
    return SteamUser()->BIsPhoneIdentifying() != 0;
}

static bool SteamUser_IsPhoneRequiringVerification(void) {
    return SteamUser()->BIsPhoneRequiringVerification() != 0;
}

static bool SteamUser_GetSteamId(uint64_t *Result) {
    *Result = SteamUser()->GetSteamID().ConvertToUint64();
    return true;
}

static bool SteamUser_GetAuthSessionTicket(int32_t *Ticket) {
    SteamUserStruct *User = (SteamUserStruct *)GlobalSteamUser;

    User->Results->Ticket = SteamUser()->GetAuthSessionTicket(
        User->Results->TicketData, Element_Count(User->Results->TicketData), &User->Results->TicketSize);

    *Ticket = User->Results->Ticket;
    return true;
}

static bool SteamUser_CancelAuthTicket(void) {
    SteamUserStruct *User = (SteamUserStruct *)GlobalSteamUser;

    SteamUser()->CancelAuthTicket(User->Results->Ticket);
    return true;
}

static bool SteamUser_GetPlayerSteamLevel(int32_t *PlayerSteamLevel) {
    *PlayerSteamLevel = SteamUser()->GetPlayerSteamLevel();
    return true;
}

static bool SteamUser_StartVoiceRecording(void) {
    SteamUser()->StartVoiceRecording();
    return true;
}

static bool SteamUser_StopVoiceRecording(void) {
    SteamUser()->StopVoiceRecording();
    return true;
}

/*********************************************************************/
// Interop functions

bool SteamUser_GetInstanceId(char *String, int32_t MaxString) {
    SteamUserStruct *User = (SteamUserStruct *)GlobalSteamUser;
    String_CopyLength(String, Class_InstanceId(User), MaxString);
    return true;
}

bool SteamUser_Process(void *SteamUserContext) {
    // This function is called once per tick and can be used to process simple operations and
    // thread synchronization.
    return true;
}

bool SteamUser_Invoke(void *SteamUserContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle) {
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    echandle ItemHandle = NULL;
    int64_t Value64 = 0;
    int32_t RetVal = false;
    int32_t ReturnValue = false;
    int32_t Value32 = 0;
    const char *Method = NULL;
    char *ValueString = NULL;
    char Value64String[120] = {0};

    if (SteamAPI_IsInitialized() == false)
        return false;
    if (IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "method", &Method) == false)
        return false;

    if (String_Compare(Method, "getSteamId") == true) {
        RetVal = SteamUser_GetSteamId((uint64 *)&Value64);
        String_Print(Value64String, Element_Count(Value64String), "%lld", Value64);
        IDictionary_AddString(ReturnDictionaryHandle, "returnValue", Value64String, &ItemHandle);
    } else if (String_Compare(Method, "getAuthSessionTicket") == true) {
        SteamUser_GetAuthSessionTicket(&Value32);
        RetVal = IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    } else if (String_Compare(Method, "cancelAuthTicket") == true) {
        ReturnValue = SteamUser_CancelAuthTicket();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "isLoggedOn") == true) {
        ReturnValue = SteamUser_IsLoggedOn();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "isBehindNAT") == true) {
        ReturnValue = SteamUser_IsBehindNAT();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "isPhoneVerified") == true) {
        ReturnValue = SteamUser_IsPhoneVerified();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "isTwoFactorEnabled") == true) {
        ReturnValue = SteamUser_IsPhoneIdentifying();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "isPhoneIdentifying") == true) {
        ReturnValue = SteamUser_IsBehindNAT();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "isPhoneRequiringVerification") == true) {
        ReturnValue = SteamUser_IsPhoneRequiringVerification();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "getPlayerSteamLevel") == true) {
        RetVal = SteamUser_GetPlayerSteamLevel(&Value32);
        IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    } else if (String_Compare(Method, "startVoiceRecording") == true) {
        ReturnValue = SteamUser_StartVoiceRecording();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "stopVoiceRecording") == true) {
        ReturnValue = SteamUser_StopVoiceRecording();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }

    return RetVal;
}

/*********************************************************************/
// Global initialization functions

bool SteamUser_Init(void) {
    SteamUserStruct *User = NULL;

    User = (SteamUserStruct *)malloc(sizeof(SteamUserStruct));
    if (User == NULL)
        return false;
    memset(User, 0, sizeof(SteamUserStruct));
    Interop_GenerateInstanceId(User->Class.InstanceId, 40);

    User->Class.RefCount = 1;

    User->Results = new UserResults();
    User->Results->User = User;
    User->Results->AuthSessionTicket.Register(User->Results, &UserResults::OnAuthSessionTicketResponse);

    GlobalSteamUser = User;
    return true;
}

bool SteamUser_Remove(void) {
    SteamUserStruct *User = (SteamUserStruct *)GlobalSteamUser;

    if (--User->Class.RefCount == 0) {
        User->Results->AuthSessionTicket.Unregister();
        delete User->Results;
        free(User);
    }

    GlobalSteamUser = NULL;
    return true;
}

/*********************************************************************/
