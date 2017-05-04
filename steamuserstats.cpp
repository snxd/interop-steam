#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "interoplib.h"
#include "interopstub.h"

#include "jansson.h"
#include "jansson_private.h"

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

int32 SteamUserStats_Invoke(void *SteamUserStatsContext, char *Method, char *ResultString, int32 ResultStringLength)
{
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    SteamUserStatsStruct *UserStats = (SteamUserStatsStruct*)SteamUserStatsContext;
    char *MethodName = NULL;
    char MethodResultString[INTEROP_MAXSTRING] = { 0 };
    int32 MethodResultInt = 0;
    int64 MethodResultLong = 0;
    uint64 MethodResultULong = 0;
    json_t *Parameter[1];
    char *JSONDumpString = NULL;
    json_t *JSON = NULL;
    json_t *JSONReturnRoot = NULL;
    json_t *JSONReturn = NULL;
    json_t *JSONMethod = NULL;
    json_error_t JSONError;
    int32 RetVal = FALSE;


    JSON = json_loads(Method, INTEROP_MAXSTRING, &JSONError);
    if (JSON == FALSE)
        return FALSE;

    RetVal = (JSONMethod = json_object_get(JSON, "method")) != NULL;

    if (RetVal == TRUE)
        RetVal = json_is_string(JSONMethod);

    if (RetVal == TRUE)
    {
        MethodName = (char*)json_string_value(JSONMethod);
        RetVal = MethodName != NULL;
    }

    if (RetVal == TRUE && String_Compare(MethodName, "getNumberOfCurrentPlayers") == TRUE)
    {
        MethodResultInt = SteamUserStats_GetNumberOfCurrentPlayers(UserStats);
        RetVal = (JSONReturn = json_integer(MethodResultInt)) != NULL;
    }

    // Set json return value
    if (RetVal == TRUE)
        RetVal = (JSONReturnRoot = json_object()) != NULL;
    if (RetVal == TRUE)
        RetVal = (json_object_set_new(JSONReturnRoot, "returnValue", JSONReturn) == 0);
    if (RetVal == TRUE)
        RetVal = (JSONDumpString = json_dumps(JSONReturnRoot, 0)) != NULL;
    if (RetVal == TRUE)
        RetVal = ((signed)String_Length(JSONDumpString) < ResultStringLength);
    if (RetVal == TRUE)
        String_CopyLength(ResultString, JSONDumpString, ResultStringLength);

    if (JSONDumpString != NULL)
        free(JSONDumpString);
    if (JSONReturnRoot != NULL)
        json_decref(JSONReturnRoot);
    if (JSON != NULL)
        json_decref(JSON);

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
