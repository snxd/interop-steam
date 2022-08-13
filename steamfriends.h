#pragma once

#ifndef _STEAMFRIENDS_H_
#define _STEAMFRIENDS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

bool SteamFriends_Process(void *SteamFriendsContext);
bool SteamFriends_Invoke(void *SteamFriendsContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle);

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif