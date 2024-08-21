#pragma once

#ifndef _STEAMFRIENDS_H_
#define _STEAMFRIENDS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

bool SteamFriends_Invoke(void *handle, echandle method_dictionary_handle, echandle return_dictionary_handle);

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif