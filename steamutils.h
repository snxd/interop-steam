#pragma once

#ifndef _STEAMUTILS_H_
#define _STEAMUTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

int32 SteamUtils_IsOverlayEnabled(void *SteamUtilsContext);
int32 SteamUtils_IsSteamRunningInVR(void *SteamUtilsContext);
int32 SteamUtils_IsSteamInBigPictureMode(void *SteamUtilsContext);
int32 SteamUtils_IsVRHeadsetStreamingEnabled(void *SteamUtilsContext);

int32 SteamUtils_GetServerRealTime(void *SteamUtilsContext, int64 *ServerRealTime);
int32 SteamUtils_GetIPCountry(void *SteamUtilsContext, char *IPCountry, int32 MaxIPCountry);
int32 SteamUtils_GetCurrentBatteryPower(void *SteamUtilsContext, int32 *CurrentBatteryPower);
int32 SteamUtils_GetAppID(void *SteamUtilsContext, int32 *AppID);
int32 SteamUtils_GetImageWidth(void *SteamUtilsContext, int32 Index, int32 *Width);
int32 SteamUtils_GetImageHeight(void *SteamUtilsContext, int32 Index, int32 *Height);

int32 SteamUtils_StartVRDashboard(void *SteamUtilsContext);

/*********************************************************************/

int32 SteamUtils_GetInstanceId(void *SteamUtilsContext, char *String, int32 MaxString);

int32 SteamUtils_Process(void *SteamUtilsContext);
int32 SteamUtils_Invoke(void *SteamUtilsContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle);

/*********************************************************************/

int32 SteamUtils_Create(void **SteamUtilsContext);
void* SteamUtils_AddRef(void *SteamUtilsContext);
int32 SteamUtils_Release(void **SteamUtilsContext);

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif