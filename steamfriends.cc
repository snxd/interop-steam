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

static bool SteamFriends_GetFriendCount(int32_t *FriendCount) {
    *FriendCount = SteamFriends()->GetFriendCount(k_EFriendFlagAll);  // All
    return true;
}

static bool SteamFriends_GetFriendByIndex(int32_t Index, uint64_t *FriendId) {
    *FriendId = SteamFriends()->GetFriendByIndex(Index, k_EFriendFlagAll).ConvertToUint64();  // All
    return true;
}

static bool SteamFriends_GetFriendPersonaName(uint64_t FriendId, char *PersonaName, int32_t MaxPersonaName) {
    strncpy(PersonaName, (char *)SteamFriends()->GetFriendPersonaName(CSteamID((uint64)FriendId)), MaxPersonaName);
    PersonaName[MaxPersonaName - 1] = 0;
    return true;
}

static bool SteamFriends_GetFriendPersonaState(uint64_t FriendId, int32_t *PersonaState) {
    *PersonaState = SteamFriends()->GetFriendPersonaState(CSteamID((uint64)FriendId));
    return true;
}

static bool SteamFriends_GetFriendRelationship(uint64_t FriendId, int32_t *Relationship) {
    *Relationship = SteamFriends()->GetFriendRelationship(CSteamID((uint64)FriendId));
    return true;
}

static bool SteamFriends_GetSmallFriendAvatar(uint64_t FriendId, int32_t *SmallFriendAvatar) {
    *SmallFriendAvatar = SteamFriends()->GetSmallFriendAvatar(CSteamID((uint64)FriendId));
    return true;
}

static bool SteamFriends_GetMediumFriendAvatar(uint64_t FriendId, int32_t *MediumFriendAvatar) {
    *MediumFriendAvatar = SteamFriends()->GetMediumFriendAvatar(CSteamID((uint64)FriendId));
    return true;
}

static bool SteamFriends_GetLargeFriendAvatar(uint64_t FriendId, int32_t *LargeFriendAvatar) {
    *LargeFriendAvatar = SteamFriends()->GetLargeFriendAvatar(CSteamID((uint64)FriendId));
    return true;
}

static bool SteamFriends_ActivateGameOverlayToWebPage(const char *URL) {
    SteamFriends()->ActivateGameOverlayToWebPage(URL);
    return true;
}

/*********************************************************************/
// Interop functions

bool SteamFriends_Process(void *SteamFriendsContext) {
    // This function is called once per tick and can be used to process simple operations and
    // thread synchronization.
    return true;
}

bool SteamFriends_Invoke(void *SteamFriendsContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle) {
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    echandle ItemHandle = NULL;
    uint64_t Value64 = 0;
    bool RetVal = false;
    int32_t ReturnValue = false;
    int32_t Value32 = 0;
    const char *Method = NULL;
    const char *ValueString = NULL;
    char Value64String[120] = {0};

    if (SteamAPI_IsInitialized() == false)
        return false;
    if (IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "method", &Method) == false)
        return false;

    if (strcmp(Method, "getFriendCount") == 0) {
        RetVal = SteamFriends_GetFriendCount(&Value32);
        IDictionary_AddInt(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    } else if (strcmp(Method, "getFriendByIndex") == 0) {
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "index", &Value32);
        if (RetVal == true)
            SteamFriends_GetFriendByIndex(Value32, (uint64_t *)&Value64);
        snprintf(Value64String, sizeof(Value64String), "%" PRIu64, (uint64_t)Value64);
        IDictionary_AddString(ReturnDictionaryHandle, "returnValue", Value64String, &ItemHandle);
    } else if (strcmp(Method, "getFriendPersonaName") == 0) {
        char PersonaName[320] = {0};
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "steamId", &ValueString);
        if (RetVal == true) {
            Value64 = strtoull(ValueString, NULL, 10);
            SteamFriends_GetFriendPersonaName(Value64, PersonaName, sizeof(PersonaName));
        }
        IDictionary_AddString(ReturnDictionaryHandle, "returnValue", PersonaName, &ItemHandle);
    } else if (strcmp(Method, "getFriendPersonaState") == 0) {
        int32_t PersonaState = 0;
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "steamId", &ValueString);
        if (RetVal == true) {
            Value64 = strtoull(ValueString, NULL, 10);
            SteamFriends_GetFriendPersonaState(Value64, &PersonaState);
        }
        IDictionary_AddInt(ReturnDictionaryHandle, "returnValue", PersonaState, &ItemHandle);
    } else if (strcmp(Method, "getFriendRelationship") == 0) {
        int32_t Relationship = 0;
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "steamId", &ValueString);
        if (RetVal == true) {
            Value64 = strtoull(ValueString, NULL, 10);
            SteamFriends_GetFriendRelationship(Value64, &Relationship);
        }
        IDictionary_AddInt(ReturnDictionaryHandle, "returnValue", Relationship, &ItemHandle);
    } else if (strcmp(Method, "getSmallFriendAvatar") == 0) {
        int32_t FriendAvatar = 0;
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "steamId", &ValueString);
        if (RetVal == true) {
            Value64 = strtoull(ValueString, NULL, 10);
            RetVal = SteamFriends_GetSmallFriendAvatar(Value64, &FriendAvatar);
        }
        IDictionary_AddInt(ReturnDictionaryHandle, "returnValue", FriendAvatar, &ItemHandle);
    } else if (strcmp(Method, "getMediumFriendAvatar") == 0) {
        int32_t FriendAvatar = 0;
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "steamId", &ValueString);
        if (RetVal == true) {
            Value64 = strtoull(ValueString, NULL, 10);
            RetVal = SteamFriends_GetMediumFriendAvatar(Value64, &FriendAvatar);
        }
        IDictionary_AddInt(ReturnDictionaryHandle, "returnValue", FriendAvatar, &ItemHandle);
    } else if (strcmp(Method, "getLargeFriendAvatar") == 0) {
        int32_t FriendAvatar = 0;
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "steamId", &ValueString);
        if (RetVal == true) {
            Value64 = strtoull(ValueString, NULL, 10);
            RetVal = SteamFriends_GetLargeFriendAvatar(Value64, &FriendAvatar);
        }
        IDictionary_AddInt(ReturnDictionaryHandle, "returnValue", FriendAvatar, &ItemHandle);
    } else if (strcmp(Method, "activateGameOverlayToWebPage") == 0) {
        char PersonaName[320] = {0};
        RetVal = IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "url", &ValueString);
        if (RetVal == true)
            ReturnValue = SteamFriends_ActivateGameOverlayToWebPage(ValueString);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }

    return RetVal;
}

/*********************************************************************/
