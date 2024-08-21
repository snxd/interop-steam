#pragma once

#ifndef _STEAMUTILS_H_
#define _STEAMUTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

bool SteamUtils_Invoke(void *handle, echandle method_dictionary_handle, echandle return_dictionary_handle);

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif