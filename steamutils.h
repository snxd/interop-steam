#pragma once

#ifndef _STEAMUTILS_H_
#define _STEAMUTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

bool SteamUtils_Process(void *SteamUtilsContext);
bool SteamUtils_Invoke(void *SteamUtilsContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle);

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif