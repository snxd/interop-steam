#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#include "interoplib.h"
#include "interopstub.h"
#include "dictionaryi.h"

#include "steam_api.h"

#include "steamapi.h"
#include "steamuserstats.h"

/*********************************************************************/

class UserStatsResults {
  public:
    CCallResult<UserStatsResults, NumberOfCurrentPlayers_t> NumberOfCurrentPlayers;
    void OnNumberOfCurrentPlayers(NumberOfCurrentPlayers_t *Result, bool bIOFailure);
    CCallResult<UserStatsResults, UserStatsReceived_t> UserStatsReceived;
    void OnUserStatsReceived(UserStatsReceived_t *Result, bool bIOFailure);
    CCallResult<UserStatsResults, GlobalAchievementPercentagesReady_t> GlobalAchievementPercentages;
    void OnGlobalAchievementPercentages(GlobalAchievementPercentagesReady_t *Result, bool bIOFailure);

    SteamAPICall_t Call = k_uAPICallInvalid;
};

/********************************************************************/

namespace {

struct SteamUserStatsStruct {
    ClassStruct Class;
    UserStatsResults *results;
};

SteamUserStatsStruct *g_steam_user_stats = nullptr;

}  // namespace

/********************************************************************/
// Callback functions

void UserStatsResults::OnNumberOfCurrentPlayers(NumberOfCurrentPlayers_t *result, bool io_failure) {
    NotificationCenter_FireAfterDelayWithJSON(
        "SteamUserStats", "NumberOfCurrentPlayersResponse", Class_InstanceId(g_steam_user_stats), 0,
        "{ \"successful\": %s,  \"playerCount\": %d }", result->m_bSuccess ? "true" : "false", result->m_cPlayers);
    delete this;
}

void UserStatsResults::OnUserStatsReceived(UserStatsReceived_t *result, bool io_failure) {
    char steam_id_string[120] = {0};
    snprintf(steam_id_string, sizeof(steam_id_string), "%" PRIu64,
             static_cast<uint64_t>(result->m_steamIDUser.ConvertToUint64()));
    NotificationCenter_FireAfterDelayWithJSON(
        "SteamUserStats", "UserStatsReceivedResponse", Class_InstanceId(g_steam_user_stats), 0,
        "{ \"successful\": %s, \"result\": %d, \"appID\": %lld,  \"steamId\": \"%s\"}",
        (result->m_eResult == k_EResultOK) ? "true" : "false", result->m_eResult, result->m_nGameID, steam_id_string);
    delete this;
}

void UserStatsResults::OnGlobalAchievementPercentages(GlobalAchievementPercentagesReady_t *result, bool io_failure) {
    NotificationCenter_FireAfterDelayWithJSON(
        "SteamUserStats", "GlobalAchievementPercentagesResponse", Class_InstanceId(g_steam_user_stats), 0,
        "{ \"successful\": %s, \"result\": %d, \"appID\": %lld }",
        (result->m_eResult == k_EResultOK) ? "true" : "false", result->m_eResult, result->m_nGameID);
    delete this;
}

/********************************************************************/
// Concrete functions

namespace {

bool GetNumberOfCurrentPlayers(void) {
    auto *api_call = new UserStatsResults();

    api_call->Call = SteamUserStats()->GetNumberOfCurrentPlayers();
    api_call->NumberOfCurrentPlayers.Set(api_call->Call, api_call, &UserStatsResults::OnNumberOfCurrentPlayers);
    return 0;
}

bool GetAchievement(const char *name, bool *achieved) {
    *achieved = false;
    bool is_set = false;
    bool result = SteamUserStats()->GetAchievement(name, &is_set);
    if (result && is_set)
        *achieved = true;
    return result != 0;
}

bool GetAchievementNamePtr(int32 index, const char **name) {
    *name = SteamUserStats()->GetAchievementName(index);
    if (!*name) {
        *name = "";
        return false;
    }
    return true;
}

bool GetAchievementDisplayAttributePtr(const char *name, const char *key, const char **attribute_value) {
    *attribute_value = SteamUserStats()->GetAchievementDisplayAttribute(name, key);
    if (!*attribute_value) {
        *attribute_value = "";
        return false;
    }
    return true;
}

bool GetUserAchievement(uint64_t user_id, const char *name, bool *achieved) {
    *achieved = false;
    bool is_set = false;
    bool result = SteamUserStats()->GetUserAchievement(CSteamID(static_cast<uint64>(user_id)), name, &is_set);
    if (result && is_set)
        *achieved = true;
    return result != 0;
}

bool RequestUserStats(uint64_t user_id) {
    auto *api_call = new UserStatsResults();

    api_call->Call = SteamUserStats()->RequestUserStats(CSteamID(static_cast<uint64>(user_id)));
    api_call->UserStatsReceived.Set(api_call->Call, api_call, &UserStatsResults::OnUserStatsReceived);
    return true;
}

bool RequestGlobalAchievementPercentages(void) {
    auto *api_call = new UserStatsResults();

    api_call->Call = SteamUserStats()->RequestGlobalAchievementPercentages();
    api_call->GlobalAchievementPercentages.Set(api_call->Call, api_call,
                                               &UserStatsResults::OnGlobalAchievementPercentages);
    return true;
}

}  // namespace

/*********************************************************************/
// Interop functions

bool SteamUserStats_GetInstanceId(char *String, int32 MaxString) {
    SteamUserStatsStruct *UserStats = (SteamUserStatsStruct *)g_steam_user_stats;
    strncpy(String, Class_InstanceId(UserStats), MaxString);
    String[MaxString - 1] = 0;
    return true;
}

bool SteamUserStats_Invoke(void *handle, echandle method_dictionary_handle, echandle return_dictionary_handle) {
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    bool ret = false;
    const char *method = nullptr;

    if (!SteamAPI_IsInitialized())
        return false;
    if (!IDictionary_GetStringPtrByKey(method_dictionary_handle, "method", &method))
        return false;

    if (strcmp(method, "getNumberOfCurrentPlayers") == 0) {
        const auto return_value = GetNumberOfCurrentPlayers();
        ret = IDictionary_AddInt(return_dictionary_handle, "returnValue", return_value, nullptr);
    } else if (strcmp(method, "getNumberOfAchievements") == 0) {
        const auto achievement_count = SteamUserStats()->GetNumAchievements();
        ret = IDictionary_AddInt(return_dictionary_handle, "returnValue", achievement_count, nullptr);
    } else if (strcmp(method, "getAchievement") == 0) {
        const char *name = nullptr;
        bool achieved = false;
        ret = IDictionary_GetStringPtrByKey(method_dictionary_handle, "name", &name);
        if (ret)
            ret = GetAchievement(name, &achieved);
        IDictionary_AddBoolean(return_dictionary_handle, "returnValue", achieved, nullptr);
    } else if (strcmp(method, "getAchievementName") == 0) {
        const char *achievement_name = "";
        int32_t index = 0;
        ret = IDictionary_GetInt32ByKey(method_dictionary_handle, "index", &index);
        if (ret)
            ret = GetAchievementNamePtr(index, &achievement_name);
        IDictionary_AddString(return_dictionary_handle, "returnValue", achievement_name, nullptr);
    } else if (strcmp(method, "getAchievementIcon") == 0) {
        bool return_value = false;
        const char *name = nullptr;
        ret = IDictionary_GetStringPtrByKey(method_dictionary_handle, "name", &name);
        if (ret)
            return_value = SteamUserStats()->GetAchievementIcon(name);
        IDictionary_AddInt(return_dictionary_handle, "returnValue", return_value, nullptr);
    } else if (strcmp(method, "getAchievementDisplayAttribute") == 0) {
        const char *key_string = nullptr;
        const char *attribute_value = "";
        const char *name = nullptr;
        ret = IDictionary_GetStringPtrByKey(method_dictionary_handle, "name", &name);
        if (ret) {
            ret = IDictionary_GetStringPtrByKey(method_dictionary_handle, "key", &key_string);
            if (ret)
                ret = GetAchievementDisplayAttributePtr(name, key_string, &attribute_value);
        }
        IDictionary_AddString(return_dictionary_handle, "returnValue", attribute_value, nullptr);
    } else if (strcmp(method, "setAchievement") == 0) {
        bool return_value = false;
        const char *name = nullptr;
        ret = IDictionary_GetStringPtrByKey(method_dictionary_handle, "name", &name);
        if (ret)
            return_value = SteamUserStats()->SetAchievement(name);
        IDictionary_AddBoolean(return_dictionary_handle, "returnValue", return_value, nullptr);
    } else if (strcmp(method, "clearAchievement") == 0) {
        bool return_value = false;
        const char *name = nullptr;
        ret = IDictionary_GetStringPtrByKey(method_dictionary_handle, "name", &name);
        if (ret)
            return_value = SteamUserStats()->ClearAchievement(name);
        IDictionary_AddBoolean(return_dictionary_handle, "returnValue", return_value, nullptr);
    } else if (strcmp(method, "getUserAchievement") == 0) {
        bool achieved = false;
        const char *steam_id = nullptr;
        ret = IDictionary_GetStringPtrByKey(method_dictionary_handle, "steamId", &steam_id);
        if (ret) {
            const char *name = nullptr;
            const auto user_id = strtoull(steam_id, nullptr, 10);
            ret = IDictionary_GetStringPtrByKey(method_dictionary_handle, "name", &name);
            if (ret)
                ret = GetUserAchievement(user_id, name, &achieved);
        }
        IDictionary_AddBoolean(return_dictionary_handle, "returnValue", achieved, nullptr);
    } else if (strcmp(method, "getAchievementAchievedPercent") == 0) {
        float32_t percent_achieved = 0.0;
        const char *name = nullptr;
        ret = IDictionary_GetStringPtrByKey(method_dictionary_handle, "name", &name);
        if (ret)
            ret = SteamUserStats()->GetAchievementAchievedPercent(name, &percent_achieved);
        IDictionary_AddFloat(return_dictionary_handle, "returnValue", percent_achieved, nullptr);
    } else if (strcmp(method, "requestUserStats") == 0) {
        int32 ImageIndex = 0;
        const char *steam_id = nullptr;
        ret = IDictionary_GetStringPtrByKey(method_dictionary_handle, "steamId", &steam_id);
        if (ret) {
            const auto user_id = strtoull(steam_id, nullptr, 10);
            ret = RequestUserStats(user_id);
        }
        IDictionary_AddBoolean(return_dictionary_handle, "returnValue", true, nullptr);
    } else if (strcmp(method, "requestGlobalAchievementPercentages") == 0) {
        ret = RequestGlobalAchievementPercentages();
        IDictionary_AddBoolean(return_dictionary_handle, "returnValue", true, nullptr);
    } else if (strcmp(method, "resetAllStats") == 0) {
        bool return_value = false;
        bool achievements_too = false;
        ret = IDictionary_GetBooleanByKey(method_dictionary_handle, "achievementsToo", &achievements_too);
        if (ret)
            return_value = SteamUserStats()->ResetAllStats(achievements_too);
        IDictionary_AddBoolean(return_dictionary_handle, "returnValue", return_value, nullptr);
    }

    return ret;
}

/*********************************************************************/
// Global initialization functions

bool SteamUserStats_Init(void) {
    auto *user_stats = reinterpret_cast<SteamUserStatsStruct *>(calloc(sizeof(SteamUserStatsStruct), 1));
    if (!user_stats)
        return false;

    Interop_GenerateInstanceId(user_stats->Class.InstanceId, sizeof(user_stats->Class.InstanceId));

    user_stats->Class.RefCount = 1;

    g_steam_user_stats = user_stats;
    return true;
}

bool SteamUserStats_Remove(void) {
    auto *user_stats = g_steam_user_stats;

    if (--user_stats->Class.RefCount == 0) {
        free(user_stats);
    }

    g_steam_user_stats = nullptr;
    return true;
}

/*********************************************************************/
