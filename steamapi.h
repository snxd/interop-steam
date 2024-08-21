#pragma once

#ifndef _STEAMAPI_H_
#define _STEAMAPI_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

bool SteamAPI_IsInitialized(void);

bool SteamAPI_Process(void *handle);
bool SteamAPI_Invoke(void *handle, echandle method_dictionary_handle, echandle return_dictionary_handle);

bool SteamAPI_InitLib(void);
bool SteamAPI_RemoveLib(void);

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif