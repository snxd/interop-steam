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
}

/********************************************************************/
// Concrete Functions

int32 SteamUserStats_GetNumberOfCurrentPlayers(void *SteamUserStatsContext)
{
    SteamUserStatsStruct *UserStats = (SteamUserStatsStruct*)SteamUserStatsContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;

    UserStats->Results->Call = SteamUserStats()->GetNumberOfCurrentPlayers();
    UserStats->Results->NumberOfCurrentPlayers.Set(UserStats->Results->Call, UserStats->Results, &UserStatsResults::OnNumberOfCurrentPlayers );
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

    UserStats->Results = new UserStatsResults();
    UserStats->Results->UserStats = UserStats;

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
        delete UserStats->Results;
        free(UserStats);
    }

    *SteamUserStatsContext = NULL;
    return TRUE;
}

/*********************************************************************/
