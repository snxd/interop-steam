#pragma once

#ifndef _STEAMUSER_H_
#define _STEAMUSER_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

int32 SteamUser_GetInstanceId(char *String, int32 MaxString);

int32 SteamUser_Process(void *SteamUserContext);
int32 SteamUser_Invoke(void *SteamUserContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle);

/*********************************************************************/

int32 SteamUser_Init(void);
int32 SteamUser_Remove(void);

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif