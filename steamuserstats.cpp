#include <string.h>
#include <stdlib.h>

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

    SteamAPICall_t Call;
};

typedef struct SteamUserStatsStruct {
    ClassStruct Class;
    UserStatsResults *Results;
} SteamUserStatsStruct;

/********************************************************************/

static SteamUserStatsStruct *GlobalSteamUserStats = NULL;

/********************************************************************/
// Callback functions

void UserStatsResults::OnNumberOfCurrentPlayers(NumberOfCurrentPlayers_t *Result, bool bIOFailure) {
    NotificationCenter_FireAfterDelayWithJSON("SteamUserStats", "NumberOfCurrentPlayersResponse", GlobalSteamUserStats,
                                              0, "{ \"successful\": %s,  \"playerCount\": %d }",
                                              Result->m_bSuccess ? "true" : "false", Result->m_cPlayers);
    delete this;
}

void UserStatsResults::OnUserStatsReceived(UserStatsReceived_t *Result, bool bIOFailure) {
    char SteamIdString[120] = {0};
    String_Print(SteamIdString, Element_Count(SteamIdString), "%lld", Result->m_steamIDUser.ConvertToUint64());
    NotificationCenter_FireAfterDelayWithJSON(
        "SteamUserStats", "UserStatsReceivedResponse", GlobalSteamUserStats, 0,
        "{ \"successful\": %s, \"result\": %d, \"appID\": %lld,  \"steamId\": \"%s\"}",
        (Result->m_eResult == k_EResultOK) ? "true" : "false", Result->m_eResult, Result->m_nGameID, SteamIdString);
    delete this;
}

void UserStatsResults::OnGlobalAchievementPercentages(GlobalAchievementPercentagesReady_t *Result, bool bIOFailure) {
    NotificationCenter_FireAfterDelayWithJSON(
        "SteamUserStats", "GlobalAchievementPercentagesResponse", GlobalSteamUserStats, 0,
        "{ \"successful\": %s, \"result\": %d, \"appID\": %lld }",
        (Result->m_eResult == k_EResultOK) ? "true" : "false", Result->m_eResult, Result->m_nGameID);
    delete this;
}

/********************************************************************/
// Concrete functions

static int32 SteamUserStats_GetNumberOfCurrentPlayers(void) {
    UserStatsResults *ApiCall = new UserStatsResults();

    ApiCall->Call = SteamUserStats()->GetNumberOfCurrentPlayers();
    ApiCall->NumberOfCurrentPlayers.Set(ApiCall->Call, ApiCall, &UserStatsResults::OnNumberOfCurrentPlayers);

    return 0;
}

static int32 SteamUserStats_GetNumberOfAchievements(int32 *Achievements) {
    *Achievements = SteamUserStats()->GetNumAchievements();
    return TRUE;
}

static int32 SteamUserStats_GetAchievement(char *Name, int32 *Achieved) {
    bool IsSet = false;
    bool Result = false;
    *Achieved = FALSE;
    Result = SteamUserStats()->GetAchievement(Name, &IsSet);
    if (Result && IsSet)
        *Achieved = TRUE;
    return Result != 0;
}

static int32 SteamUserStats_GetAchievementNamePtr(int32 Index, char **Name) {
    *Name = (char *)SteamUserStats()->GetAchievementName(Index);
    if (*Name == NULL) {
        *Name = "";
        return FALSE;
    }
    return TRUE;
}

static int32 SteamUserStats_GetAchievementIcon(char *Name, int32 *ImageIndex) {
    *ImageIndex = SteamUserStats()->GetAchievementIcon(Name);
    return TRUE;
}

static int32 SteamUserStats_GetAchievementDisplayAttributePtr(char *Name, char *Key, char **AttributeValue) {
    *AttributeValue = (char *)SteamUserStats()->GetAchievementDisplayAttribute(Name, Key);
    if (*AttributeValue == NULL) {
        *AttributeValue = "";
        return FALSE;
    }
    return TRUE;
}

static int32 SteamUserStats_SetAchievement(char *Name) {
    return SteamUserStats()->SetAchievement(Name) != 0;
}

static int32 SteamUserStats_ClearAchievement(char *Name) {
    return SteamUserStats()->ClearAchievement(Name) != 0;
}

static int32 SteamUserStats_GetUserAchievement(uint64 SteamId, char *Name, int32 *Achieved) {
    bool IsSet = false;
    bool Result = false;
    *Achieved = FALSE;
    Result = SteamUserStats()->GetUserAchievement(CSteamID(SteamId), Name, &IsSet);
    if (Result && IsSet)
        *Achieved = TRUE;
    return Result != 0;
}

static int32 SteamUserStats_GetAchievementAchievedPercent(char *Name, float32 *PercentAchieved) {
    SteamUserStats()->GetAchievementAchievedPercent(Name, PercentAchieved);
    return TRUE;
}

static int32 SteamUserStats_RequestUserStats(uint64 FriendId) {
    UserStatsResults *ApiCall = new UserStatsResults();

    ApiCall->Call = SteamUserStats()->RequestUserStats(CSteamID(FriendId));
    ApiCall->UserStatsReceived.Set(ApiCall->Call, ApiCall, &UserStatsResults::OnUserStatsReceived);
    return TRUE;
}

static int32 SteamUserStats_RequestGlobalAchievementPercentages(void) {
    UserStatsResults *ApiCall = new UserStatsResults();

    ApiCall->Call = SteamUserStats()->RequestGlobalAchievementPercentages();
    ApiCall->GlobalAchievementPercentages.Set(ApiCall->Call, ApiCall,
                                              &UserStatsResults::OnGlobalAchievementPercentages);
    return TRUE;
}

static int32 SteamUserStats_ResetAllStats(int32 AchievementsToo) {
    SteamUserStats()->ResetAllStats(AchievementsToo);
    return TRUE;
}

/*********************************************************************/
// Interop functions

int32 SteamUserStats_GetInstanceId(char *String, int32 MaxString) {
    SteamUserStatsStruct *UserStats = (SteamUserStatsStruct *)GlobalSteamUserStats;
    String_CopyLength(String, Class_InstanceId(UserStats), MaxString);
    return TRUE;
}

int32 SteamUserStats_Process(void *SteamUserStatsContext) {
    // This function is called once per tick and can be used to process simple operations and
    // thread synchronization.
    return TRUE;
}

int32 SteamUserStats_Invoke(void *SteamUserStatsContext, echandle MethodDictionaryHandle,
                            echandle ReturnDictionaryHandle) {
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    echandle ItemHandle = NULL;
    float32 ValueFloat32 = 0;
    int64 Value64 = 0;
    int32 RetVal = FALSE;
    int32 ReturnValue = FALSE;
    int32 Value32 = 0;
    char *Method = NULL;
    char *ValueString = NULL;

    if (SteamAPI_IsInitialized() == FALSE)
        return FALSE;
    if (IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "method", &Method) == FALSE)
        return FALSE;

    if (String_Compare(Method, "getNumberOfCurrentPlayers") == TRUE) {
        Value64 = SteamUserStats_GetNumberOfCurrentPlayers();
        RetVal = IDictionary_AddInt64(ReturnDictionaryHandle, "returnValue", Value64, &ItemHandle);
    } else if (String_Compare(Method, "getNumberOfAchievements") == TRUE) {
        RetVal = SteamUserStats_GetNumberOfAchievements(&Value32);
        IDictionary_AddInt64(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    } else if (String_Compare(Method, "getAchievement") == TRUE) {
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "name", &ValueString);
        if (RetVal == TRUE)
            RetVal = SteamUserStats_GetAchievement(ValueString, &Value32);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    } else if (String_Compare(Method, "getAchievementName") == TRUE) {
        char *AchievementName = "";
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "index", &Value32);
        if (RetVal == TRUE)
            RetVal = SteamUserStats_GetAchievementNamePtr(Value32, &AchievementName);
        IDictionary_AddString(ReturnDictionaryHandle, "returnValue", AchievementName, &ItemHandle);
    } else if (String_Compare(Method, "getAchievementIcon") == TRUE) {
        int32 ImageIndex = 0;
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "name", &ValueString);
        if (RetVal == TRUE)
            RetVal = SteamUserStats_GetAchievementIcon(ValueString, &ImageIndex);
        IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", ImageIndex, &ItemHandle);
    } else if (String_Compare(Method, "getAchievementDisplayAttribute") == TRUE) {
        char *KeyString = NULL;
        char *AttributeValue = "";
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "name", &ValueString);
        if (RetVal == TRUE) {
            RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "key", &KeyString);
            if (RetVal == TRUE)
                RetVal = SteamUserStats_GetAchievementDisplayAttributePtr(ValueString, KeyString, &AttributeValue);
        }
        IDictionary_AddString(ReturnDictionaryHandle, "returnValue", AttributeValue, &ItemHandle);
    } else if (String_Compare(Method, "setAchievement") == TRUE) {
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "name", &ValueString);
        if (RetVal == TRUE)
            ReturnValue = SteamUserStats_SetAchievement(ValueString);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "clearAchievement") == TRUE) {
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "name", &ValueString);
        if (RetVal == TRUE)
            ReturnValue = SteamUserStats_ClearAchievement(ValueString);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "getUserAchievement") == TRUE) {
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "steamId", &ValueString);
        if (RetVal == TRUE) {
            Value64 = String_AtoI64(ValueString);
            RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "name", &ValueString);
            if (RetVal == TRUE)
                RetVal = SteamUserStats_GetUserAchievement(Value64, ValueString, &Value32);
        }
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    } else if (String_Compare(Method, "getAchievementAchievedPercent") == TRUE) {
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "name", &ValueString);
        if (RetVal == TRUE)
            RetVal = SteamUserStats_GetAchievementAchievedPercent(ValueString, &ValueFloat32);
        IDictionary_AddFloat64(ReturnDictionaryHandle, "returnValue", ValueFloat32, &ItemHandle);
    } else if (String_Compare(Method, "requestUserStats") == TRUE) {
        int32 ImageIndex = 0;
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "steamId", &ValueString);
        if (RetVal == TRUE) {
            Value64 = String_AtoI64(ValueString);
            RetVal = SteamUserStats_RequestUserStats(Value64);
        }
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", TRUE, &ItemHandle);
    } else if (String_Compare(Method, "requestGlobalAchievementPercentages") == TRUE) {
        RetVal = SteamUserStats_RequestGlobalAchievementPercentages();
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", TRUE, &ItemHandle);
    } else if (String_Compare(Method, "resetAllStats") == TRUE) {
        RetVal = IDictionary_GetBooleanByKey(MethodDictionaryHandle, "achievementsToo", &Value32);
        if (RetVal == TRUE)
            ReturnValue = SteamUserStats_ResetAllStats(Value32);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }

    return RetVal;
}

/*********************************************************************/
// Global initialization functions

int32 SteamUserStats_Init(void) {
    SteamUserStatsStruct *UserStats = NULL;

    UserStats = (SteamUserStatsStruct *)malloc(sizeof(SteamUserStatsStruct));
    memset(UserStats, 0, sizeof(SteamUserStatsStruct));
    Interop_GenerateInstanceId(UserStats->Class.InstanceId, 40);

    UserStats->Class.RefCount = 1;

    GlobalSteamUserStats = UserStats;
    return TRUE;
}

int32 SteamUserStats_Remove(void) {
    SteamUserStatsStruct *UserStats = (SteamUserStatsStruct *)GlobalSteamUserStats;

    if (--UserStats->Class.RefCount == 0) {
        free(UserStats);
    }

    GlobalSteamUserStats = NULL;
    return TRUE;
}

/*********************************************************************/
