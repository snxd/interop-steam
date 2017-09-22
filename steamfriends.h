#pragma once

#ifndef _STEAMFRIENDS_H_
#define _STEAMFRIENDS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

int32 SteamFriends_GetFriendCount(void *SteamFriendsContext, int32 *FriendCount);
int32 SteamFriends_GetFriendByIndex(void *SteamFriendsContext, int32 Index, uint64 *FriendId);
int32 SteamFriends_GetFriendPersonaName(void *SteamFriendsContext, uint64 FriendId, char *PersonaName, int32 MaxPersonaName);

/*********************************************************************/

int32 SteamFriends_GetInstanceId(void *SteamFriendsContext, char *String, int32 MaxString);

int32 SteamFriends_Process(void *SteamFriendsContext);
int32 SteamFriends_Invoke(void *SteamFriendsContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle);

/*********************************************************************/

int32 SteamFriends_Create(void **SteamFriendsContext);
void* SteamFriends_AddRef(void *SteamFriendsContext);
int32 SteamFriends_Release(void **SteamFriendsContext);

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif