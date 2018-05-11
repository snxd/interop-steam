#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "interoplib.h"
#include "interopstub.h"
#include "dictionaryi.h"

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
    *FriendCount = SteamFriends()->GetFriendCount(k_EFriendFlagAll); // All
    return TRUE;
}

int32 SteamFriends_GetFriendByIndex(void *SteamFriendsContext, int32 Index, uint64 *FriendId)
{
    SteamFriendsStruct *Friends = (SteamFriendsStruct*)SteamFriendsContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    *FriendId = SteamFriends()->GetFriendByIndex(Index, k_EFriendFlagAll).ConvertToUint64(); // All
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

int32 SteamFriends_GetFriendPersonaState(void *SteamFriendsContext, uint64 FriendId, int32 *PersonaState)
{
    SteamFriendsStruct *Friends = (SteamFriendsStruct*)SteamFriendsContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    *PersonaState = SteamFriends()->GetFriendPersonaState(CSteamID(FriendId));
    return TRUE;
}

int32 SteamFriends_GetFriendRelationship(void *SteamFriendsContext, uint64 FriendId, int32 *Relationship)
{
    SteamFriendsStruct *Friends = (SteamFriendsStruct*)SteamFriendsContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    *Relationship = SteamFriends()->GetFriendRelationship(CSteamID(FriendId));
    return TRUE;
}

int32 SteamFriends_GetSmallFriendAvatar(void *SteamFriendsContext, uint64 FriendId, int32 *SmallFriendAvatar)
{
    SteamFriendsStruct *Friends = (SteamFriendsStruct*)SteamFriendsContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    *SmallFriendAvatar = SteamFriends()->GetSmallFriendAvatar(CSteamID(FriendId));
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

int32 SteamFriends_Invoke(void *SteamFriendsContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle)
{
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    SteamFriendsStruct *Friends = (SteamFriendsStruct*)SteamFriendsContext;
    echandle ItemHandle = NULL;
    int64 Value64 = 0;
    int32 RetVal = FALSE;
    int32 ReturnValue = FALSE;
    int32 Value32 = 0;
    char *Method = NULL;
    char *ValueString = NULL;
    char Value64String[120] = { 0 };


    if (IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "method", &Method) == FALSE)
        return FALSE;

    if (String_Compare(Method, "getFriendCount") == TRUE)
    {
        RetVal = SteamFriends_GetFriendCount(Friends, &Value32);
        IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    }
    else if (String_Compare(Method, "getFriendByIndex") == TRUE)
    {
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "index", &Value32);
        if (RetVal == TRUE)
            SteamFriends_GetFriendByIndex(Friends, Value32, (uint64 *)&Value64);
        String_Print(Value64String, Element_Count(Value64String), "%lld", Value64);
        IDictionary_AddString(ReturnDictionaryHandle, "returnValue", Value64String, &ItemHandle);
    }
    else if (String_Compare(Method, "getFriendPersonaName") == TRUE)
    {
        char PersonaName[320] = { 0 };
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "steamId", &ValueString);
        if (RetVal == TRUE)
        {
            Value64 = String_AtoI64(ValueString);
            SteamFriends_GetFriendPersonaName(Friends, Value64, PersonaName, Element_Count(PersonaName));
        }
        IDictionary_AddString(ReturnDictionaryHandle, "returnValue", PersonaName, &ItemHandle);
    }
    else if (String_Compare(Method, "getFriendPersonaState") == TRUE)
    {
        int32 PersonaState = 0;
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "steamId", &ValueString);
        if (RetVal == TRUE)
        {
            Value64 = String_AtoI64(ValueString);
            SteamFriends_GetFriendPersonaState(Friends, Value64, &PersonaState);
        }
        IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", PersonaState, &ItemHandle);
    }
    else if (String_Compare(Method, "getFriendRelationship") == TRUE)
    {
        int32 Relationship = 0;
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "steamId", &ValueString);
        if (RetVal == TRUE)
        {
            Value64 = String_AtoI64(ValueString);
            SteamFriends_GetFriendRelationship(Friends, Value64, &Relationship);
        }
        IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", Relationship, &ItemHandle);
    }
    else if (String_Compare(Method, "activateGameOverlayToWebPage") == TRUE)
    {
        char PersonaName[320] = { 0 };
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "url", &ValueString);
        if (RetVal == TRUE)
            ReturnValue = SteamFriends_ActivateGameOverlayToWebPage(Friends, ValueString);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }
    else if (String_Compare(Method, "getSmallFriendAvatar") == TRUE)
    {
        int32 SmallFriendAvatar = 0;
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "steamId", &ValueString);
        if (RetVal == TRUE)
        {
            Value64 = String_AtoI64(ValueString);
            SteamFriends_GetSmallFriendAvatar(Friends, Value64, &SmallFriendAvatar);
        }
        IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", SmallFriendAvatar, &ItemHandle);
    }
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
