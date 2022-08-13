#pragma once

#ifndef _STEAMUSERSTATS_H_
#define _STEAMUSERSTATS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

bool SteamUserStats_GetInstanceId(char *String, int32_t MaxString);

bool SteamUserStats_Process(void *SteamUserStatsContext);
bool SteamUserStats_Invoke(void *SteamUserStatsContext, echandle MethodDictionaryHandle,
                           echandle ReturnDictionaryHandle);

/*********************************************************************/

bool SteamUserStats_Init(void);
bool SteamUserStats_Remove(void);

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif