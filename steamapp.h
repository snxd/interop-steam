#pragma once

#ifndef _STEAMAPP_H_
#define _STEAMAPP_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

int32 SteamApp_IsInitialized(void);

int32 SteamApp_GetInstanceId(char *String, int32 MaxString);

int32 SteamApp_Process(void *SteamAppContext);
int32 SteamApp_Invoke(void *SteamAppContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle);

int32 SteamApp_Init(void);
int32 SteamApp_Remove(void);

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif