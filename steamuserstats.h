#pragma once

#ifndef _STEAMUSERSTATS_H_
#define _STEAMUSERSTATS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

int32 SteamUserStats_GetNumberOfCurrentPlayers(void *SteamUserStatsContext);

/*********************************************************************/

int32 SteamUserStats_GetInstanceId(void *SteamUserStatsContext, char *String, int32 MaxString);

int32 SteamUserStats_Process(void *SteamUserStatsContext);
int32 SteamUserStats_Invoke(void *SteamUserStatsContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle);

/*********************************************************************/

int32 SteamUserStats_Create(void **SteamUserStatsContext);
void* SteamUserStats_AddRef(void *SteamUserStatsContext);
int32 SteamUserStats_Release(void **SteamUserStatsContext);

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif