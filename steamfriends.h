#pragma once

#ifndef _STEAMFRIENDS_H_
#define _STEAMFRIENDS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

int32 SteamFriends_Process(void *SteamFriendsContext);
int32 SteamFriends_Invoke(void *SteamFriendsContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle);

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif