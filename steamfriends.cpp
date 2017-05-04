#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "interoplib.h"
#include "interopstub.h"

#include "jansson.h"
#include "jansson_private.h"

#include "steam_api.h"

#include "steamapp.h"
#include "steamfriends.h"

/*********************************************************************/

typedef struct SteamFriendsStruct
{
    ClassStruct             Class;
} SteamFriendsStruct;

/********************************************************************/
// Concrete Functions

int32 SteamFriends_GetFriendCount(void *SteamFriendsContext, int32 *FriendCount)
{
    SteamFriendsStruct *Friends = (SteamFriendsStruct*)SteamFriendsContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    *FriendCount = SteamFriends()->GetFriendCount(0xFFFF); // All
    return TRUE;
}

int32 SteamFriends_GetFriendByIndex(void *SteamFriendsContext, int32 Index, uint64 *FriendId)
{
    SteamFriendsStruct *Friends = (SteamFriendsStruct*)SteamFriendsContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    *FriendId = SteamFriends()->GetFriendByIndex(Index, 0xFFFF).ConvertToUint64(); // All
    return TRUE;
}

int32 SteamFriends_GetFriendPersonaName(void *SteamFriendsContext, uint64 FriendId, char *PersonaName, int32 MaxPersonaName)
{
    SteamFriendsStruct *Friends = (SteamFriendsStruct*)SteamFriendsContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    String_CopyLength(PersonaName, (char *)SteamFriends()->GetFriendPersonaName(CSteamID(FriendId)), MaxPersonaName);
    return TRUE;
}

int32 SteamFriends_ActivateGameOverlayToWebPage(void *SteamFriendsContext, char *URL)
{
    SteamFriendsStruct *Friends = (SteamFriendsStruct*)SteamFriendsContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    SteamFriends()->ActivateGameOverlayToWebPage(URL);
    return TRUE;
}

/*********************************************************************/
// Interop Functions

int32 SteamFriends_GetInstanceId(void *SteamFriendsContext, char *String, int32 MaxString)
{
    SteamFriendsStruct *Friends = (SteamFriendsStruct *)SteamFriendsContext;
    String_CopyLength(String, Class_InstanceId(Friends), MaxString);
    return TRUE;
}

int32 SteamFriends_Process(void *SteamFriendsContext)
{
    // This function is called once per tick and can be used to process simple operations and
    // thread synchronization.
    return TRUE;
}

int32 SteamFriends_Invoke(void *SteamFriendsContext, char *Method, char *ResultString, int32 ResultStringLength)
{
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    SteamFriendsStruct *Friends = (SteamFriendsStruct*)SteamFriendsContext;
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

    if (RetVal == TRUE && String_Compare(MethodName, "getFriendCount") == TRUE)
    {
        SteamFriends_GetFriendCount(Friends, &MethodResultInt);
        RetVal = (JSONReturn = json_integer(MethodResultInt)) != NULL;
    }
    else if (RetVal == TRUE && String_Compare(MethodName, "getFriendByIndex") == TRUE)
    {
        RetVal = ((Parameter[0] = json_object_get(JSON, "index")) != NULL);
        if (RetVal == TRUE)
            RetVal = json_is_integer(Parameter[0]);
        if (RetVal == TRUE)
            SteamFriends_GetFriendByIndex(Friends, json_integer_value(Parameter[0]), &MethodResultULong);
        RetVal = (JSONReturn = json_integer(MethodResultULong)) != NULL;
    }
    else if (RetVal == TRUE && String_Compare(MethodName, "getFriendPersonaName") == TRUE)
    {
        RetVal = ((Parameter[0] = json_object_get(JSON, "steamId")) != NULL);
        if (RetVal == TRUE)
            RetVal = json_is_integer(Parameter[0]);
        if (RetVal == TRUE)
            SteamFriends_GetFriendPersonaName(Friends, json_integer_value(Parameter[0]), MethodResultString, Element_Count(MethodResultString));
        RetVal = (JSONReturn = json_string(MethodResultString)) != NULL;
    }
    else if (RetVal == TRUE && String_Compare(MethodName, "activateGameOverlayToWebPage") == TRUE)
    {
        RetVal = ((Parameter[0] = json_object_get(JSON, "url")) != NULL);
        if (RetVal == TRUE)
            RetVal = json_is_string(Parameter[0]);
        if (RetVal == TRUE)
            SteamFriends_ActivateGameOverlayToWebPage(Friends, (char *)json_string_value(Parameter[0]));
        RetVal = (JSONReturn = json_true()) != NULL;
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

int32 SteamFriends_Create(void **SteamFriendsContext)
{
    SteamFriendsStruct *Friends = NULL;

    Friends = (SteamFriendsStruct*)malloc(sizeof(SteamFriendsStruct));
    memset(Friends, 0, sizeof(SteamFriendsStruct));
    Interop_GenerateInstanceId(Friends->Class.InstanceId, 40);

    Friends->Class.RefCount = 1;

    *SteamFriendsContext = Friends;
    return TRUE;
}

void *SteamFriends_AddRef(void *SteamFriendsContext)
{
    SteamFriendsStruct *Friends = (SteamFriendsStruct *)SteamFriendsContext;
    Friends->Class.RefCount += 1;
    return Friends;
}

int32 SteamFriends_Release(void **SteamFriendsContext)
{
    SteamFriendsStruct *Friends = (SteamFriendsStruct *)*SteamFriendsContext;

    if (--Friends->Class.RefCount == 0)
    {
        free(Friends);
    }

    *SteamFriendsContext = NULL;
    return TRUE;
}

/*********************************************************************/
