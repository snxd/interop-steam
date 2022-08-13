#pragma once

#ifndef _STEAMAPI_H_
#define _STEAMAPI_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

bool SteamAPI_IsInitialized(void);

bool SteamAPI_Process(void *SteamAPIContext);
bool SteamAPI_Invoke(void *SteamAPIContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle);

bool SteamAPI_InitLib(void);
bool SteamAPI_RemoveLib(void);

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif