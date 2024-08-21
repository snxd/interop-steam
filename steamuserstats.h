#pragma once

#ifndef _STEAMUSERSTATS_H_
#define _STEAMUSERSTATS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

bool SteamUserStats_GetInstanceId(char *string, int32_t max_string);

bool SteamUserStats_Invoke(void *handle, echandle method_dictionary_handle, echandle return_dictionary_handle);

/*********************************************************************/

bool SteamUserStats_Init(void);
bool SteamUserStats_Remove(void);

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif