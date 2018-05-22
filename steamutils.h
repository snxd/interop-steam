#pragma once

#ifndef _STEAMUTILS_H_
#define _STEAMUTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

int32 SteamUtils_Process(void *SteamUtilsContext);
int32 SteamUtils_Invoke(void *SteamUtilsContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle);

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif