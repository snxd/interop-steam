#pragma once

#ifndef _STEAMAPPS_H_
#define _STEAMAPPS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

bool SteamApps_Process(void *SteamAppsContext);
bool SteamApps_Invoke(void *SteamAppsContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle);

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif