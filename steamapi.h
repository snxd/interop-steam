#pragma once

#ifndef _STEAMAPI_H_
#define _STEAMAPI_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

int32 SteamAPI_IsInitialized(void);

int32 SteamAPI_Process(void *SteamAPIContext);
int32 SteamAPI_Invoke(void *SteamAPIContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle);

int32 SteamAPI_InitLib(void);
int32 SteamAPI_RemoveLib(void);

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif