#pragma once

#ifndef _STEAMAPPS_H_
#define _STEAMAPPS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

bool SteamApps_Invoke(void *handle, echandle method_dictionary_handle, echandle return_dictionary_handle);

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif