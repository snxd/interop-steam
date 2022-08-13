#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "interoplib.h"
#include "interopstub.h"
#include "dictionaryi.h"

#include "steam_api.h"

#include "steamapi.h"
#include "steamfriends.h"

/*********************************************************************/
// Concrete functions

static int32 SteamFriends_GetFriendCount(int32 *FriendCount) {
    *FriendCount = SteamFriends()->GetFriendCount(k_EFriendFlagAll);  // All
    return TRUE;
}

static int32 SteamFriends_GetFriendByIndex(int32 Index, uint64 *FriendId) {
    *FriendId = SteamFriends()->GetFriendByIndex(Index, k_EFriendFlagAll).ConvertToUint64();  // All
    return TRUE;
}

static int32 SteamFriends_GetFriendPersonaName(uint64 FriendId, char *PersonaName, int32 MaxPersonaName) {
    String_CopyLength(PersonaName, (char *)SteamFriends()->GetFriendPersonaName(CSteamID(FriendId)), MaxPersonaName);
    return TRUE;
}

static int32 SteamFriends_GetFriendPersonaState(uint64 FriendId, int32 *PersonaState) {
    *PersonaState = SteamFriends()->GetFriendPersonaState(CSteamID(FriendId));
    return TRUE;
}

static int32 SteamFriends_GetFriendRelationship(uint64 FriendId, int32 *Relationship) {
    *Relationship = SteamFriends()->GetFriendRelationship(CSteamID(FriendId));
    return TRUE;
}

static int32 SteamFriends_GetSmallFriendAvatar(uint64 FriendId, int32 *SmallFriendAvatar) {
    *SmallFriendAvatar = SteamFriends()->GetSmallFriendAvatar(CSteamID(FriendId));
    return TRUE;
}

static int32 SteamFriends_GetMediumFriendAvatar(uint64 FriendId, int32 *MediumFriendAvatar) {
    *MediumFriendAvatar = SteamFriends()->GetMediumFriendAvatar(CSteamID(FriendId));
    return TRUE;
}

static int32 SteamFriends_GetLargeFriendAvatar(uint64 FriendId, int32 *LargeFriendAvatar) {
    *LargeFriendAvatar = SteamFriends()->GetLargeFriendAvatar(CSteamID(FriendId));
    return TRUE;
}

static int32 SteamFriends_ActivateGameOverlayToWebPage(char *URL) {
    SteamFriends()->ActivateGameOverlayToWebPage(URL);
    return TRUE;
}

/*********************************************************************/
// Interop functions

int32 SteamFriends_Process(void *SteamFriendsContext) {
    // This function is called once per tick and can be used to process simple operations and
    // thread synchronization.
    return TRUE;
}

int32 SteamFriends_Invoke(void *SteamFriendsContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle) {
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    echandle ItemHandle = NULL;
    int64 Value64 = 0;
    int32 RetVal = FALSE;
    int32 ReturnValue = FALSE;
    int32 Value32 = 0;
    char *Method = NULL;
    char *ValueString = NULL;
    char Value64String[120] = {0};

    if (SteamAPI_IsInitialized() == FALSE)
        return FALSE;
    if (IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "method", &Method) == FALSE)
        return FALSE;

    if (String_Compare(Method, "getFriendCount") == TRUE) {
        RetVal = SteamFriends_GetFriendCount(&Value32);
        IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    } else if (String_Compare(Method, "getFriendByIndex") == TRUE) {
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "index", &Value32);
        if (RetVal == TRUE)
            SteamFriends_GetFriendByIndex(Value32, (uint64 *)&Value64);
        String_Print(Value64String, Element_Count(Value64String), "%lld", Value64);
        IDictionary_AddString(ReturnDictionaryHandle, "returnValue", Value64String, &ItemHandle);
    } else if (String_Compare(Method, "getFriendPersonaName") == TRUE) {
        char PersonaName[320] = {0};
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "steamId", &ValueString);
        if (RetVal == TRUE) {
            Value64 = String_AtoI64(ValueString);
            SteamFriends_GetFriendPersonaName(Value64, PersonaName, Element_Count(PersonaName));
        }
        IDictionary_AddString(ReturnDictionaryHandle, "returnValue", PersonaName, &ItemHandle);
    } else if (String_Compare(Method, "getFriendPersonaState") == TRUE) {
        int32 PersonaState = 0;
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "steamId", &ValueString);
        if (RetVal == TRUE) {
            Value64 = String_AtoI64(ValueString);
            SteamFriends_GetFriendPersonaState(Value64, &PersonaState);
        }
        IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", PersonaState, &ItemHandle);
    } else if (String_Compare(Method, "getFriendRelationship") == TRUE) {
        int32 Relationship = 0;
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "steamId", &ValueString);
        if (RetVal == TRUE) {
            Value64 = String_AtoI64(ValueString);
            SteamFriends_GetFriendRelationship(Value64, &Relationship);
        }
        IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", Relationship, &ItemHandle);
    } else if (String_Compare(Method, "getSmallFriendAvatar") == TRUE) {
        int32 FriendAvatar = 0;
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "steamId", &ValueString);
        if (RetVal == TRUE) {
            Value64 = String_AtoI64(ValueString);
            RetVal = SteamFriends_GetSmallFriendAvatar(Value64, &FriendAvatar);
        }
        IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", FriendAvatar, &ItemHandle);
    } else if (String_Compare(Method, "getMediumFriendAvatar") == TRUE) {
        int32 FriendAvatar = 0;
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "steamId", &ValueString);
        if (RetVal == TRUE) {
            Value64 = String_AtoI64(ValueString);
            RetVal = SteamFriends_GetMediumFriendAvatar(Value64, &FriendAvatar);
        }
        IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", FriendAvatar, &ItemHandle);
    } else if (String_Compare(Method, "getLargeFriendAvatar") == TRUE) {
        int32 FriendAvatar = 0;
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "steamId", &ValueString);
        if (RetVal == TRUE) {
            Value64 = String_AtoI64(ValueString);
            RetVal = SteamFriends_GetLargeFriendAvatar(Value64, &FriendAvatar);
        }
        IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", FriendAvatar, &ItemHandle);
    } else if (String_Compare(Method, "activateGameOverlayToWebPage") == TRUE) {
        char PersonaName[320] = {0};
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "url", &ValueString);
        if (RetVal == TRUE)
            ReturnValue = SteamFriends_ActivateGameOverlayToWebPage(ValueString);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }

    return RetVal;
}

/*********************************************************************/
