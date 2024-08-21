#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#include "interoplib.h"
#include "interopstub.h"
#include "dictionaryi.h"

#include "steam_api.h"

#include "steamapi.h"
#include "steamuser.h"

/*********************************************************************/

class UserResults {
  public:
    CCallbackManual<UserResults, GetAuthSessionTicketResponse_t, false> auth_session_ticket;

    void *user = nullptr;

    HAuthTicket ticket = k_HAuthTicketInvalid;
    uint8_t ticket_data[2048] = {0};
    uint32_t ticket_size = 0;

    void OnAuthSessionTicketResponse(GetAuthSessionTicketResponse_t *response);
};

/********************************************************************/

namespace {

struct SteamUserStruct {
    ClassStruct Class;
    UserResults *results;
};

SteamUserStruct *g_steam_user = nullptr;

/********************************************************************/
// Concrete functions

int32_t GetAuthSessionTicket() {
    auto *user = g_steam_user;

    user->results->ticket = SteamUser()->GetAuthSessionTicket(
        user->results->ticket_data, sizeof(user->results->ticket_data), &user->results->ticket_size);

    return static_cast<int32_t>(user->results->ticket);
}

}  // namespace

/********************************************************************/
// Callback functions

void UserResults::OnAuthSessionTicketResponse(GetAuthSessionTicketResponse_t *response) {
    bool successful = false;

    if (response->m_eResult == k_EResultOK)
        successful = true;
    if (response->m_hAuthTicket == ticket) {
        char hex_ticket[4096];
        String_ConvertToHex(ticket_data, ticket_size, true, hex_ticket, sizeof(hex_ticket));
        NotificationCenter_FireAfterDelayWithJSON(
            "SteamUser", "AuthSessionTicketResponse", Class_InstanceId(static_cast<SteamUserStruct *>(user)), 0,
            "{ \"ticket\": %d, \"ticketData\": \"%s\", \"successful\": %s, \"errorCode\": %d }", ticket, hex_ticket,
            successful ? "true" : "false", response->m_eResult);
    }
}

/*********************************************************************/
// Interop functions

bool SteamUser_GetInstanceId(char *string, int32_t max_string) {
    if (!g_steam_user)
        return false;
    strncpy(string, Class_InstanceId(g_steam_user), max_string);
    string[max_string - 1] = 0;
    return true;
}

bool SteamUser_Invoke(void *handle, echandle method_dictionary_handle, echandle return_dictionary_handle) {
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    int32_t ret = false;
    const char *method = nullptr;

    if (!SteamAPI_IsInitialized())
        return false;
    if (!IDictionary_GetStringPtrByKey(method_dictionary_handle, "method", &method))
        return false;

    if (strcmp(method, "getSteamId") == 0) {
        const auto steam_id = SteamUser()->GetSteamID().ConvertToUint64();
        ret = steam_id != 0;
        char steam_id_string[120]{};
        snprintf(steam_id_string, sizeof(steam_id_string), "%" PRIu64, static_cast<uint64_t>(steam_id));
        IDictionary_AddString(return_dictionary_handle, "returnValue", steam_id_string, nullptr);
    } else if (strcmp(method, "getAuthSessionTicket") == 0) {
        const auto ticket = GetAuthSessionTicket();
        ret = IDictionary_AddInt(return_dictionary_handle, "returnValue", ticket, nullptr);
    } else if (strcmp(method, "cancelAuthTicket") == 0) {
        bool cancelled = false;
        if (g_steam_user) {
            SteamUser()->CancelAuthTicket(g_steam_user->results->ticket);
            cancelled = true;
        }
        ret = IDictionary_AddBoolean(return_dictionary_handle, "returnValue", cancelled, nullptr);
    } else if (strcmp(method, "isLoggedOn") == 0) {
        const auto is_logged_on = SteamUser()->BLoggedOn();
        ret = IDictionary_AddBoolean(return_dictionary_handle, "returnValue", is_logged_on, nullptr);
    } else if (strcmp(method, "isBehindNAT") == 0) {
        const auto is_behind_nat = SteamUser()->BIsBehindNAT();
        ret = IDictionary_AddBoolean(return_dictionary_handle, "returnValue", is_behind_nat, nullptr);
    } else if (strcmp(method, "isPhoneVerified") == 0) {
        const auto is_verified = SteamUser()->BIsPhoneVerified();
        ret = IDictionary_AddBoolean(return_dictionary_handle, "returnValue", is_verified, nullptr);
    } else if (strcmp(method, "isTwoFactorEnabled") == 0) {
        const auto is_enabled = SteamUser()->BIsTwoFactorEnabled();
        ret = IDictionary_AddBoolean(return_dictionary_handle, "returnValue", is_enabled, nullptr);
    } else if (strcmp(method, "isPhoneIdentifying") == 0) {
        const auto is_identifying = SteamUser()->BIsPhoneIdentifying();
        ret = IDictionary_AddBoolean(return_dictionary_handle, "returnValue", is_identifying, nullptr);
    } else if (strcmp(method, "isPhoneRequiringVerification") == 0) {
        const auto is_requiring_verification = SteamUser()->BIsPhoneRequiringVerification();
        ret = IDictionary_AddBoolean(return_dictionary_handle, "returnValue", is_requiring_verification, nullptr);
    } else if (strcmp(method, "getPlayerSteamLevel") == 0) {
        const auto player_level = SteamUser()->GetPlayerSteamLevel();
        ret = IDictionary_AddInt(return_dictionary_handle, "returnValue", player_level, nullptr);
    } else if (strcmp(method, "startVoiceRecording") == 0) {
        SteamUser()->StartVoiceRecording();
        ret = IDictionary_AddBoolean(return_dictionary_handle, "returnValue", true, nullptr);
    } else if (strcmp(method, "stopVoiceRecording") == 0) {
        SteamUser()->StopVoiceRecording();
        ret = IDictionary_AddBoolean(return_dictionary_handle, "returnValue", true, nullptr);
    }

    return ret;
}

/*********************************************************************/
// Global initialization functions

bool SteamUser_Init(void) {
    auto *user = reinterpret_cast<SteamUserStruct *>(calloc(sizeof(SteamUserStruct), 1));
    if (!user)
        return false;
    Interop_GenerateInstanceId(user->Class.InstanceId, sizeof(user->Class.InstanceId));

    user->Class.RefCount = 1;

    user->results = new UserResults();
    user->results->user = user;
    user->results->auth_session_ticket.Register(user->results, &UserResults::OnAuthSessionTicketResponse);

    g_steam_user = user;
    return true;
}

bool SteamUser_Remove(void) {
    SteamUserStruct *user = g_steam_user;

    if (--user->Class.RefCount == 0) {
        user->results->auth_session_ticket.Unregister();
        delete user->results;
        free(user);
    }

    g_steam_user = nullptr;
    return true;
}

/*********************************************************************/
