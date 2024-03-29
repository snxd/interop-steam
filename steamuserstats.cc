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

typedef struct SteamUserStatsStruct {
    ClassStruct Class;
    UserStatsResults *Results;
} SteamUserStatsStruct;

/********************************************************************/

static SteamUserStatsStruct *GlobalSteamUserStats = nullptr;

/********************************************************************/
// Callback functions

void UserStatsResults::OnNumberOfCurrentPlayers(NumberOfCurrentPlayers_t *Result, bool bIOFailure) {
    NotificationCenter_FireAfterDelayWithJSON(
        "SteamUserStats", "NumberOfCurrentPlayersResponse", Class_InstanceId(GlobalSteamUserStats), 0,
        "{ \"successful\": %s,  \"playerCount\": %d }", Result->m_bSuccess ? "true" : "false", Result->m_cPlayers);
    delete this;
}

void UserStatsResults::OnUserStatsReceived(UserStatsReceived_t *Result, bool bIOFailure) {
    char SteamIdString[120] = {0};
    snprintf(SteamIdString, sizeof(SteamIdString), "%" PRIu64, Result->m_steamIDUser.ConvertToUint64());
    NotificationCenter_FireAfterDelayWithJSON(
        "SteamUserStats", "UserStatsReceivedResponse", Class_InstanceId(GlobalSteamUserStats), 0,
        "{ \"successful\": %s, \"result\": %d, \"appID\": %lld,  \"steamId\": \"%s\"}",
        (Result->m_eResult == k_EResultOK) ? "true" : "false", Result->m_eResult, Result->m_nGameID, SteamIdString);
    delete this;
}

void UserStatsResults::OnGlobalAchievementPercentages(GlobalAchievementPercentagesReady_t *Result, bool bIOFailure) {
    NotificationCenter_FireAfterDelayWithJSON(
        "SteamUserStats", "GlobalAchievementPercentagesResponse", Class_InstanceId(GlobalSteamUserStats), 0,
        "{ \"successful\": %s, \"result\": %d, \"appID\": %lld }",
        (Result->m_eResult == k_EResultOK) ? "true" : "false", Result->m_eResult, Result->m_nGameID);
    delete this;
}

/********************************************************************/
// Concrete functions

static bool SteamUserStats_GetNumberOfCurrentPlayers(void) {
    UserStatsResults *ApiCall = new UserStatsResults();

    ApiCall->Call = SteamUserStats()->GetNumberOfCurrentPlayers();
    ApiCall->NumberOfCurrentPlayers.Set(ApiCall->Call, ApiCall, &UserStatsResults::OnNumberOfCurrentPlayers);

    return 0;
}

static bool SteamUserStats_GetNumberOfAchievements(int32 *Achievements) {
    *Achievements = SteamUserStats()->GetNumAchievements();
    return true;
}

static bool SteamUserStats_GetAchievement(const char *Name, int32 *Achieved) {
    bool IsSet = false;
    bool Result = false;
    *Achieved = false;
    Result = SteamUserStats()->GetAchievement(Name, &IsSet);
    if (Result && IsSet)
        *Achieved = true;
    return Result != 0;
}

static bool SteamUserStats_GetAchievementNamePtr(int32 Index, const char **Name) {
    *Name = (char *)SteamUserStats()->GetAchievementName(Index);
    if (!*Name) {
        *Name = "";
        return false;
    }
    return true;
}

static bool SteamUserStats_GetAchievementIcon(const char *Name, int32 *ImageIndex) {
    *ImageIndex = SteamUserStats()->GetAchievementIcon(Name);
    return true;
}

static bool SteamUserStats_GetAchievementDisplayAttributePtr(const char *Name, const char *Key,
                                                             const char **AttributeValue) {
    *AttributeValue = (char *)SteamUserStats()->GetAchievementDisplayAttribute(Name, Key);
    if (!*AttributeValue) {
        *AttributeValue = "";
        return false;
    }
    return true;
}

static bool SteamUserStats_SetAchievement(const char *Name) {
    return SteamUserStats()->SetAchievement(Name) != 0;
}

static bool SteamUserStats_ClearAchievement(const char *Name) {
    return SteamUserStats()->ClearAchievement(Name) != 0;
}

static bool SteamUserStats_GetUserAchievement(uint64_t SteamId, const char *Name, int32_t *Achieved) {
    bool IsSet = false;
    bool Result = false;
    *Achieved = false;
    Result = SteamUserStats()->GetUserAchievement(CSteamID((uint64)SteamId), Name, &IsSet);
    if (Result && IsSet)
        *Achieved = true;
    return Result != 0;
}

static bool SteamUserStats_GetAchievementAchievedPercent(const char *Name, float32_t *PercentAchieved) {
    SteamUserStats()->GetAchievementAchievedPercent(Name, PercentAchieved);
    return true;
}

static bool SteamUserStats_RequestUserStats(uint64_t FriendId) {
    UserStatsResults *ApiCall = new UserStatsResults();

    ApiCall->Call = SteamUserStats()->RequestUserStats(CSteamID((uint64)FriendId));
    ApiCall->UserStatsReceived.Set(ApiCall->Call, ApiCall, &UserStatsResults::OnUserStatsReceived);
    return true;
}

static bool SteamUserStats_RequestGlobalAchievementPercentages(void) {
    UserStatsResults *ApiCall = new UserStatsResults();

    ApiCall->Call = SteamUserStats()->RequestGlobalAchievementPercentages();
    ApiCall->GlobalAchievementPercentages.Set(ApiCall->Call, ApiCall,
                                              &UserStatsResults::OnGlobalAchievementPercentages);
    return true;
}

static bool SteamUserStats_ResetAllStats(bool AchievementsToo) {
    SteamUserStats()->ResetAllStats(AchievementsToo);
    return true;
}

/*********************************************************************/
// Interop functions

bool SteamUserStats_GetInstanceId(char *String, int32 MaxString) {
    SteamUserStatsStruct *UserStats = (SteamUserStatsStruct *)GlobalSteamUserStats;
    strncpy(String, Class_InstanceId(UserStats), MaxString);
    String[MaxString - 1] = 0;
    return true;
}

bool SteamUserStats_Process(void *SteamUserStatsContext) {
    // This function is called once per tick and can be used to process simple operations and
    // thread synchronization.
    return true;
}

bool SteamUserStats_Invoke(void *SteamUserStatsContext, echandle MethodDictionaryHandle,
                           echandle ReturnDictionaryHandle) {
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    echandle ItemHandle = nullptr;
    float32_t ValueFloat32 = 0;
    uint64_t Value64 = 0;
    bool RetVal = false;
    int32_t ReturnValue = false;
    int32_t Value32 = 0;
    bool ValueBool = false;
    const char *Method = nullptr;
    const char *ValueString = nullptr;

    if (!SteamAPI_IsInitialized())
        return false;
    if (!IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "method", &Method))
        return false;

    if (strcmp(Method, "getNumberOfCurrentPlayers") == 0) {
        Value64 = SteamUserStats_GetNumberOfCurrentPlayers();
        RetVal = IDictionary_AddInt(ReturnDictionaryHandle, "returnValue", Value64, &ItemHandle);
    } else if (strcmp(Method, "getNumberOfAchievements") == 0) {
        RetVal = SteamUserStats_GetNumberOfAchievements(&Value32);
        IDictionary_AddInt(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    } else if (strcmp(Method, "getAchievement") == 0) {
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "name", &ValueString);
        if (RetVal)
            RetVal = SteamUserStats_GetAchievement(ValueString, &Value32);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    } else if (strcmp(Method, "getAchievementName") == 0) {
        const char *AchievementName = "";
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "index", &Value32);
        if (RetVal)
            RetVal = SteamUserStats_GetAchievementNamePtr(Value32, &AchievementName);
        IDictionary_AddString(ReturnDictionaryHandle, "returnValue", AchievementName, &ItemHandle);
    } else if (strcmp(Method, "getAchievementIcon") == 0) {
        int32 ImageIndex = 0;
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "name", &ValueString);
        if (RetVal)
            RetVal = SteamUserStats_GetAchievementIcon(ValueString, &ImageIndex);
        IDictionary_AddInt(ReturnDictionaryHandle, "returnValue", ImageIndex, &ItemHandle);
    } else if (strcmp(Method, "getAchievementDisplayAttribute") == 0) {
        const char *KeyString = nullptr;
        const char *AttributeValue = "";
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "name", &ValueString);
        if (RetVal) {
            RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "key", &KeyString);
            if (RetVal)
                RetVal = SteamUserStats_GetAchievementDisplayAttributePtr(ValueString, KeyString, &AttributeValue);
        }
        IDictionary_AddString(ReturnDictionaryHandle, "returnValue", AttributeValue, &ItemHandle);
    } else if (strcmp(Method, "setAchievement") == 0) {
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "name", &ValueString);
        if (RetVal)
            ReturnValue = SteamUserStats_SetAchievement(ValueString);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (strcmp(Method, "clearAchievement") == 0) {
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "name", &ValueString);
        if (RetVal)
            ReturnValue = SteamUserStats_ClearAchievement(ValueString);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (strcmp(Method, "getUserAchievement") == 0) {
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "steamId", &ValueString);
        if (RetVal) {
            Value64 = strtoull(ValueString, nullptr, 10);
            RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "name", &ValueString);
            if (RetVal)
                RetVal = SteamUserStats_GetUserAchievement(Value64, ValueString, &Value32);
        }
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    } else if (strcmp(Method, "getAchievementAchievedPercent") == 0) {
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "name", &ValueString);
        if (RetVal)
            RetVal = SteamUserStats_GetAchievementAchievedPercent(ValueString, &ValueFloat32);
        IDictionary_AddFloat(ReturnDictionaryHandle, "returnValue", ValueFloat32, &ItemHandle);
    } else if (strcmp(Method, "requestUserStats") == 0) {
        int32 ImageIndex = 0;
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "steamId", &ValueString);
        if (RetVal)
            RetVal = SteamUserStats_RequestUserStats(strtoull(ValueString, nullptr, 10));
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", true, &ItemHandle);
    } else if (strcmp(Method, "requestGlobalAchievementPercentages") == 0) {
        RetVal = SteamUserStats_RequestGlobalAchievementPercentages();
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", true, &ItemHandle);
    } else if (strcmp(Method, "resetAllStats") == 0) {
        RetVal = IDictionary_GetBooleanByKey(MethodDictionaryHandle, "achievementsToo", &ValueBool);
        if (RetVal)
            ReturnValue = SteamUserStats_ResetAllStats(ValueBool);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }

    return RetVal;
}

/*********************************************************************/
// Global initialization functions

bool SteamUserStats_Init(void) {
    SteamUserStatsStruct *UserStats = nullptr;

    UserStats = (SteamUserStatsStruct *)malloc(sizeof(SteamUserStatsStruct));
    if (!UserStats)
        return false;
    memset(UserStats, 0, sizeof(SteamUserStatsStruct));
    Interop_GenerateInstanceId(UserStats->Class.InstanceId, sizeof(UserStats->Class.InstanceId));

    UserStats->Class.RefCount = 1;

    GlobalSteamUserStats = UserStats;
    return true;
}

bool SteamUserStats_Remove(void) {
    SteamUserStatsStruct *UserStats = (SteamUserStatsStruct *)GlobalSteamUserStats;

    if (--UserStats->Class.RefCount == 0) {
        free(UserStats);
    }

    GlobalSteamUserStats = nullptr;
    return true;
}

/*********************************************************************/
