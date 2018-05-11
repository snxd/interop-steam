#pragma once

#ifndef _STEAMUSER_H_
#define _STEAMUSER_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

int32 SteamUser_IsLoggedOn(void *SteamUserContext);
int32 SteamUser_IsBehindNAT(void *SteamUserContext);
int32 SteamUser_IsPhoneVerified(void *SteamUserContext);
int32 SteamUser_IsTwoFactorEnabled(void *SteamUserContext);
int32 SteamUser_IsPhoneIdentifying(void *SteamUserContext);
int32 SteamUser_IsPhoneRequiringVerification(void *SteamUserContext);

int32 SteamUser_GetSteamId(void *SteamUserContext, uint64 *Result);
int32 SteamUser_GetAuthSessionTicket(void *SteamUserContext, int32 *Ticket);
int32 SteamUser_GetPlayerSteamLevel(void *SteamUserContext, int32 *PlayerSteamLevel);

int32 SteamUser_CancelAuthTicket(void *SteamUserContext);
int32 SteamUser_StartVoiceRecording(void *SteamUserContext);
int32 SteamUser_StopVoiceRecording(void *SteamUserContext);

/*********************************************************************/

int32 SteamUser_GetInstanceId(void *SteamUserContext, char *String, int32 MaxString);

int32 SteamUser_Process(void *SteamUserContext);
int32 SteamUser_Invoke(void *SteamUserContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle);

/*********************************************************************/

int32 SteamUser_Create(void **SteamUserContext);
void* SteamUser_AddRef(void *SteamUserContext);
int32 SteamUser_Release(void **SteamUserContext);

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif