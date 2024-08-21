#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#include "interoplib.h"
#include "interopstub.h"
#include "dictionaryi.h"

#include "steam_api.h"

#include "steamapi.h"
#include "steamfriends.h"

/*********************************************************************/
// Concrete functions

namespace {

bool GetFriendPersonaName(uint64_t friend_id, char *persona_name, int32_t max_persona_name) {
    strncpy(persona_name, SteamFriends()->GetFriendPersonaName(CSteamID(static_cast<uint64>(friend_id))),
            max_persona_name);
    persona_name[max_persona_name - 1] = 0;
    return true;
}

}  // namespace

/*********************************************************************/
// Interop functions

bool SteamFriends_Invoke(void *handle, echandle method_dictionary_handle, echandle return_dictionary_handle) {
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    bool ret = true;
    const char *method = nullptr;

    if (!SteamAPI_IsInitialized())
        return false;
    if (!IDictionary_GetStringPtrByKey(method_dictionary_handle, "method", &method))
        return false;

    if (strcmp(method, "getFriendCount") == 0) {
        const auto count = SteamFriends()->GetFriendCount(k_EFriendFlagAll);
        IDictionary_AddInt(return_dictionary_handle, "returnValue", count, nullptr);
    } else if (strcmp(method, "getFriendByIndex") == 0) {
        int32_t index = 0;
        uint64_t friend_id = 0;
        ret = IDictionary_GetInt32ByKey(method_dictionary_handle, "index", &index);
        if (ret)
            friend_id = SteamFriends()->GetFriendByIndex(index, k_EFriendFlagAll).ConvertToUint64();
        char friend_id_string[120]{};
        snprintf(friend_id_string, sizeof(friend_id_string), "%" PRIu64, static_cast<uint64_t>(friend_id));
        IDictionary_AddString(return_dictionary_handle, "returnValue", friend_id_string, nullptr);
    } else if (strcmp(method, "getFriendPersonaName") == 0) {
        char persona_name[320] = {0};
        const char *steam_id = nullptr;
        ret = IDictionary_GetStringPtrByKey(method_dictionary_handle, "steamId", &steam_id);
        if (ret) {
            const auto friend_id = strtoull(steam_id, nullptr, 10);
            GetFriendPersonaName(friend_id, persona_name, sizeof(persona_name));
        }
        IDictionary_AddString(return_dictionary_handle, "returnValue", persona_name, nullptr);
    } else if (strcmp(method, "getFriendPersonaState") == 0) {
        int32_t person_state = 0;
        const char *steam_id = nullptr;
        ret = IDictionary_GetStringPtrByKey(method_dictionary_handle, "steamId", &steam_id);
        if (ret) {
            const auto friend_id = strtoull(steam_id, nullptr, 10);
            person_state = SteamFriends()->GetFriendPersonaState(CSteamID(static_cast<uint64>(friend_id)));
        }
        IDictionary_AddInt(return_dictionary_handle, "returnValue", person_state, nullptr);
    } else if (strcmp(method, "getFriendRelationship") == 0) {
        int32_t relationship = 0;
        const char *steam_id = nullptr;
        ret = IDictionary_GetStringPtrByKey(method_dictionary_handle, "steamId", &steam_id);
        if (ret) {
            const auto friend_id = strtoull(steam_id, nullptr, 10);
            relationship = SteamFriends()->GetFriendRelationship(CSteamID(static_cast<uint64>(friend_id)));
        }
        IDictionary_AddInt(return_dictionary_handle, "returnValue", relationship, nullptr);
    } else if (strcmp(method, "getSmallFriendAvatar") == 0) {
        int32_t friend_avatar = 0;
        const char *steam_id = nullptr;
        ret = IDictionary_GetStringPtrByKey(method_dictionary_handle, "steamId", &steam_id);
        if (ret) {
            const auto friend_id = strtoull(steam_id, nullptr, 10);
            friend_avatar = SteamFriends()->GetSmallFriendAvatar(CSteamID(static_cast<uint64>(friend_id)));
        }
        IDictionary_AddInt(return_dictionary_handle, "returnValue", friend_avatar, nullptr);
    } else if (strcmp(method, "getMediumFriendAvatar") == 0) {
        int32_t friend_avatar = 0;
        const char *steam_id = nullptr;
        ret = IDictionary_GetStringPtrByKey(method_dictionary_handle, "steamId", &steam_id);
        if (ret) {
            const auto friend_id = strtoull(steam_id, nullptr, 10);
            friend_avatar = SteamFriends()->GetMediumFriendAvatar(friend_id);
        }
        IDictionary_AddInt(return_dictionary_handle, "returnValue", friend_avatar, nullptr);
    } else if (strcmp(method, "getLargeFriendAvatar") == 0) {
        int32_t friend_avatar = 0;
        const char *steam_id = nullptr;
        ret = IDictionary_GetStringPtrByKey(method_dictionary_handle, "steamId", &steam_id);
        if (ret) {
            const auto friend_id = strtoull(steam_id, nullptr, 10);
            friend_avatar = SteamFriends()->GetLargeFriendAvatar(friend_id);
        }
        IDictionary_AddInt(return_dictionary_handle, "returnValue", friend_avatar, nullptr);
    } else if (strcmp(method, "activateGameOverlayToWebPage") == 0) {
        const char *url = nullptr;
        ret = IDictionary_GetStringPtrByKey(method_dictionary_handle, "url", &url);
        if (ret)
            SteamFriends()->ActivateGameOverlayToWebPage(url);
        IDictionary_AddBoolean(return_dictionary_handle, "returnValue", true, nullptr);
    } else {
        return false;
    }

    return ret;
}

/*********************************************************************/
