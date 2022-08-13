#pragma once

#ifndef _STEAMUSER_H_
#define _STEAMUSER_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

bool SteamUser_GetInstanceId(char *String, int32_t MaxString);

bool SteamUser_Process(void *SteamUserContext);
bool SteamUser_Invoke(void *SteamUserContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle);

/*********************************************************************/

bool SteamUser_Init(void);
bool SteamUser_Remove(void);

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif