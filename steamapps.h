#pragma once

#ifndef _STEAMAPPS_H_
#define _STEAMAPPS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

int32 SteamApps_Process(void *SteamAppsContext);
int32 SteamApps_Invoke(void *SteamAppsContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle);

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif