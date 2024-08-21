#pragma once

#ifndef _STEAMUSER_H_
#define _STEAMUSER_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

bool SteamUser_GetInstanceId(char *string, int32_t max_string);

bool SteamUser_Invoke(void *handle, echandle method_dictionary_handle, echandle return_dictionary_handle);

/*********************************************************************/

bool SteamUser_Init(void);
bool SteamUser_Remove(void);

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif