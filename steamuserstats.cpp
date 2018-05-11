#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "interoplib.h"
#include "interopstub.h"
#include "dictionaryi.h"

#include "steam_api.h"

#include "steamapp.h"
#include "steamuserstats.h"

/*********************************************************************/

class UserStatsResults
{
public:
    CCallResult<UserStatsResults, NumberOfCurrentPlayers_t> NumberOfCurrentPlayers;
    void OnNumberOfCurrentPlayers(NumberOfCurrentPlayers_t *Result, bool bIOFailure);
    CCallResult<UserStatsResults, UserStatsReceived_t> UserStatsReceived;
    void OnUserStatsReceived(UserStatsReceived_t *Result, bool bIOFailure);
    CCallResult<UserStatsResults, GlobalAchievementPercentagesReady_t> GlobalAchievementPercentages;
    void OnGlobalAchievementPercentages(GlobalAchievementPercentagesReady_t *Result, bool bIOFailure);

    void                    *UserStats;
    SteamAPICall_t          Call;
};

typedef struct SteamUserStatsStruct
{
    ClassStruct             Class;
    UserStatsResults        *Results;
} SteamUserStatsStruct;

/********************************************************************/
// Callback Functions

void UserStatsResults::OnNumberOfCurrentPlayers(NumberOfCurrentPlayers_t *Result, bool bIOFailure)
{
    NotificationCenter_FireAfterDelayWithJSON("SteamUserStats", "NumberOfCurrentPlayersResponse", this->UserStats, 0,
        "{ \"successful\": %s,  \"playerCount\": %d }",
        Result->m_bSuccess ? "true" : "false", Result->m_cPlayers);
    delete this;
}

void UserStatsResults::OnUserStatsReceived(UserStatsReceived_t *Result, bool bIOFailure)
{
    char SteamIdString[120] = { 0 };
    String_Print(SteamIdString, Element_Count(SteamIdString), "%lld", Result->m_steamIDUser.ConvertToUint64());
    NotificationCenter_FireAfterDelayWithJSON("SteamUserStats", "UserStatsReceivedResponse", this->UserStats, 0,
        "{ \"successful\": %s, \"result\": %d, \"appID\": %lld,  \"steamId\": \"%s\"}",
        (Result->m_eResult == k_EResultOK) ? "true" : "false", Result->m_eResult, Result->m_nGameID, SteamIdString);
    delete this;
}

void UserStatsResults::OnGlobalAchievementPercentages(GlobalAchievementPercentagesReady_t *Result, bool bIOFailure)
{
    NotificationCenter_FireAfterDelayWithJSON("SteamUserStats", "GlobalAchievementPercentagesResponse", this->UserStats, 0,
        "{ \"successful\": %s, \"result\": %d, \"appID\": %lld }",
        (Result->m_eResult == k_EResultOK) ? "true" : "false", Result->m_eResult, Result->m_nGameID);
    delete this;
}

/********************************************************************/
// Concrete Functions

int32 SteamUserStats_GetNumberOfCurrentPlayers(void *SteamUserStatsContext)
{
    SteamUserStatsStruct *UserStats = (SteamUserStatsStruct*)SteamUserStatsContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;

    UserStatsResults *ApiCall = new UserStatsResults();

    ApiCall->UserStats = UserStats;
    ApiCall->Call = SteamUserStats()->GetNumberOfCurrentPlayers();
    ApiCall->NumberOfCurrentPlayers.Set(ApiCall->Call, ApiCall, &UserStatsResults::OnNumberOfCurrentPlayers);

    return 0;
}

int32 SteamUserStats_GetNumberOfAchievements(void *SteamUserStatsContext, int32 *Achievements)
{
    SteamUserStatsStruct *UserStats = (SteamUserStatsStruct*)SteamUserStatsContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    *Achievements = SteamUserStats()->GetNumAchievements();
    return TRUE;
}

int32 SteamUserStats_GetAchievement(void *SteamUserStatsContext, char *Name, int32 *Achieved)
{
    SteamUserStatsStruct *UserStats = (SteamUserStatsStruct*)SteamUserStatsContext;
    bool IsSet = false;
    bool Result = false;
    *Achieved = FALSE;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    Result = SteamUserStats()->GetAchievement(Name, &IsSet);
    if (Result && IsSet)
        *Achieved = TRUE;
    return Result != 0;
}

int32 SteamUserStats_GetAchievementNamePtr(void *SteamUserStatsContext, int32 Index, char **Name)
{
    SteamUserStatsStruct *UserStats = (SteamUserStatsStruct*)SteamUserStatsContext;
    *Name = NULL;
    if (SteamApp_IsInitialized() == TRUE)
        *Name = (char *)SteamUserStats()->GetAchievementName(Index);
    if (*Name == NULL)
    {
        *Name = "";
        return FALSE;
    }
    return TRUE;
}

int32 SteamUserStats_GetAchievementIcon(void *SteamUserStatsContext, char *Name, int32 *ImageIndex)
{
    SteamUserStatsStruct *UserStats = (SteamUserStatsStruct*)SteamUserStatsContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    *ImageIndex = SteamUserStats()->GetAchievementIcon(Name);
    return TRUE;
}

int32 SteamUserStats_GetAchievementDisplayAttributePtr(void *SteamUserStatsContext, char *Name, char *Key, char **AttributeValue)
{
    SteamUserStatsStruct *UserStats = (SteamUserStatsStruct*)SteamUserStatsContext;
    *AttributeValue = NULL;
    if (SteamApp_IsInitialized() == TRUE)
        *AttributeValue = (char *)SteamUserStats()->GetAchievementDisplayAttribute(Name, Key);
    if (*AttributeValue == NULL)
    {
        *AttributeValue = "";
        return FALSE;
    }
    return TRUE;
}

int32 SteamUserStats_SetAchievement(void *SteamUserStatsContext, char *Name)
{
    SteamUserStatsStruct *UserStats = (SteamUserStatsStruct*)SteamUserStatsContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    return SteamUserStats()->SetAchievement(Name) != 0;
}

int32 SteamUserStats_ClearAchievement(void *SteamUserStatsContext, char *Name)
{
    SteamUserStatsStruct *UserStats = (SteamUserStatsStruct*)SteamUserStatsContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    return SteamUserStats()->ClearAchievement(Name) != 0;
}

int32 SteamUserStats_GetUserAchievement(void *SteamUserStatsContext, uint64 SteamId, char *Name, int32 *Achieved)
{
    SteamUserStatsStruct *UserStats = (SteamUserStatsStruct*)SteamUserStatsContext;
    bool IsSet = false;
    bool Result = false;
    *Achieved = FALSE;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    Result = SteamUserStats()->GetUserAchievement(CSteamID(SteamId), Name, &IsSet);
    if (Result && IsSet)
        *Achieved = TRUE;
    return Result != 0;
}

int32 SteamUserStats_GetAchievementAchievedPercent(void *SteamUserStatsContext, char *Name, float32 *PercentAchieved)
{
    SteamUserStatsStruct *UserStats = (SteamUserStatsStruct*)SteamUserStatsContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    SteamUserStats()->GetAchievementAchievedPercent(Name, PercentAchieved);
    return TRUE;
}

int32 SteamUserStats_RequestUserStats(void *SteamUserStatsContext, uint64 FriendId)
{
    SteamUserStatsStruct *UserStats = (SteamUserStatsStruct*)SteamUserStatsContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;

    UserStatsResults *ApiCall = new UserStatsResults();

    ApiCall->UserStats = UserStats;
    ApiCall->Call = SteamUserStats()->RequestUserStats(CSteamID(FriendId));
    ApiCall->UserStatsReceived.Set(ApiCall->Call, ApiCall, &UserStatsResults::OnUserStatsReceived);
    return TRUE;
}

int32 SteamUserStats_RequestGlobalAchievementPercentages(void *SteamUserStatsContext)
{
    SteamUserStatsStruct *UserStats = (SteamUserStatsStruct*)SteamUserStatsContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;

    UserStatsResults *ApiCall = new UserStatsResults();

    ApiCall->UserStats = UserStats;
    ApiCall->Call = SteamUserStats()->RequestGlobalAchievementPercentages();
    ApiCall->GlobalAchievementPercentages.Set(ApiCall->Call, ApiCall, &UserStatsResults::OnGlobalAchievementPercentages);
    return TRUE;
}

int32 SteamUserStats_ResetAllStats(void *SteamUserStatsContext, int32 AchievementsToo)
{
    SteamUserStatsStruct *UserStats = (SteamUserStatsStruct*)SteamUserStatsContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    SteamUserStats()->ResetAllStats(AchievementsToo);
    return TRUE;
}

/*********************************************************************/
// Interop Functions

int32 SteamUserStats_GetInstanceId(void *SteamUserStatsContext, char *String, int32 MaxString)
{
    SteamUserStatsStruct *UserStats = (SteamUserStatsStruct *)SteamUserStatsContext;
    String_CopyLength(String, Class_InstanceId(UserStats), MaxString);
    return TRUE;
}

int32 SteamUserStats_Process(void *SteamUserStatsContext)
{
    // This function is called once per tick and can be used to process simple operations and
    // thread synchronization.
    return TRUE;
}

int32 SteamUserStats_Invoke(void *SteamUserStatsContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle)
{
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    SteamUserStatsStruct *UserStats = (SteamUserStatsStruct*)SteamUserStatsContext;
    echandle ItemHandle = NULL;
    float32 ValueFloat32 = 0;
    int64 Value64 = 0;
    int32 RetVal = FALSE;
    int32 ReturnValue = FALSE;
    int32 Value32 = 0;
    char *Method = NULL;
    char *ValueString = NULL;


    if (IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "method", &Method) == FALSE)
        return FALSE;

    if (String_Compare(Method, "getNumberOfCurrentPlayers") == TRUE)
    {
        Value64 = SteamUserStats_GetNumberOfCurrentPlayers(UserStats);
        RetVal = IDictionary_AddInt64(ReturnDictionaryHandle, "returnValue", Value64, &ItemHandle);
    }
    else if (String_Compare(Method, "getNumberOfAchievements") == TRUE)
    {
        RetVal = SteamUserStats_GetNumberOfAchievements(UserStats, &Value32);
        IDictionary_AddInt64(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    }
    else if (String_Compare(Method, "getAchievement") == TRUE)
    {
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "name", &ValueString);
        if (RetVal == TRUE)
            RetVal = SteamUserStats_GetAchievement(UserStats, ValueString, &Value32);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    }
    else if (String_Compare(Method, "getAchievementName") == TRUE)
    {
        char *AchievementName = "";
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "index", &Value32);
        if (RetVal == TRUE)
            RetVal = SteamUserStats_GetAchievementNamePtr(UserStats, Value32, &AchievementName);
        IDictionary_AddString(ReturnDictionaryHandle, "returnValue", AchievementName, &ItemHandle);
    }
    else if (String_Compare(Method, "getAchievementIcon") == TRUE)
    {
        int32 ImageIndex = 0;
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "name", &ValueString);
        if (RetVal == TRUE)
            RetVal = SteamUserStats_GetAchievementIcon(UserStats, ValueString, &ImageIndex);
        IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", ImageIndex, &ItemHandle);
    }
    else if (String_Compare(Method, "getAchievementDisplayAttribute") == TRUE)
    {
        char *KeyString = NULL;
        char *AttributeValue = "";
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "name", &ValueString);
        if (RetVal == TRUE)
        {
            RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "key", &KeyString);
            if (RetVal == TRUE)
                RetVal = SteamUserStats_GetAchievementDisplayAttributePtr(UserStats, ValueString, KeyString, &AttributeValue);
        }
        IDictionary_AddString(ReturnDictionaryHandle, "returnValue", AttributeValue, &ItemHandle);
    }
    else if (String_Compare(Method, "setAchievement") == TRUE)
    {
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "name", &ValueString);
        if (RetVal == TRUE)
            ReturnValue = SteamUserStats_SetAchievement(UserStats, ValueString);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }
    else if (String_Compare(Method, "clearAchievement") == TRUE)
    {
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "name", &ValueString);
        if (RetVal == TRUE)
            ReturnValue = SteamUserStats_ClearAchievement(UserStats, ValueString);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }
    else if (String_Compare(Method, "getUserAchievement") == TRUE)
    {
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "steamId", &ValueString);
        if (RetVal == TRUE)
        {
            Value64 = String_AtoI64(ValueString);
            RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "name", &ValueString);
            if (RetVal == TRUE)
                RetVal = SteamUserStats_GetUserAchievement(UserStats, Value64, ValueString, &Value32);
        }
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    }
    else if (String_Compare(Method, "getAchievementAchievedPercent") == TRUE)
    {
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "name", &ValueString);
        if (RetVal == TRUE)
            RetVal = SteamUserStats_GetAchievementAchievedPercent(UserStats, ValueString, &ValueFloat32);
        IDictionary_AddFloat64(ReturnDictionaryHandle, "returnValue", ValueFloat32, &ItemHandle);
    }
    else if (String_Compare(Method, "requestUserStats") == TRUE)
    {
        int32 ImageIndex = 0;
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "steamId", &ValueString);
        if (RetVal == TRUE)
        {
            Value64 = String_AtoI64(ValueString);
            RetVal = SteamUserStats_RequestUserStats(UserStats, Value64);
        }
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", TRUE, &ItemHandle);
    }
    else if (String_Compare(Method, "requestGlobalAchievementPercentages") == TRUE)
    {
        RetVal = SteamUserStats_RequestGlobalAchievementPercentages(UserStats);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", TRUE, &ItemHandle);
    }
    else if (String_Compare(Method, "resetAllStats") == TRUE)
    {
        RetVal = IDictionary_GetBooleanByKey(MethodDictionaryHandle, "achievementsToo", &Value32);
        if (RetVal == TRUE)
            ReturnValue = SteamUserStats_ResetAllStats(UserStats, Value32);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }

    return RetVal;
}

/*********************************************************************/
// Creation/Deletion Functions

int32 SteamUserStats_Create(void **SteamUserStatsContext)
{
    SteamUserStatsStruct *UserStats = NULL;

    UserStats = (SteamUserStatsStruct*)malloc(sizeof(SteamUserStatsStruct));
    memset(UserStats, 0, sizeof(SteamUserStatsStruct));
    Interop_GenerateInstanceId(UserStats->Class.InstanceId, 40);
    
    UserStats->Class.RefCount = 1;

    *SteamUserStatsContext = UserStats;
    return TRUE;
}

void *SteamUserStats_AddRef(void *SteamUserStatsContext)
{
    SteamUserStatsStruct *UserStats = (SteamUserStatsStruct *)SteamUserStatsContext;
    UserStats->Class.RefCount += 1;
    return UserStats;
}

int32 SteamUserStats_Release(void **SteamUserStatsContext)
{
    SteamUserStatsStruct *UserStats = (SteamUserStatsStruct *)*SteamUserStatsContext;

    if (--UserStats->Class.RefCount == 0)
    {
        free(UserStats);
    }

    *SteamUserStatsContext = NULL;
    return TRUE;
}

/*********************************************************************/
