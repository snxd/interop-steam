#pragma once

#ifndef _STEAMUSER_H_
#define _STEAMUSER_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

int32 SteamUser_IsLoggedOn(void *SteamUserContext, int32 *Result);
int32 SteamUser_GetSteamId(void *SteamUserContext, uint64 *Result);
int32 SteamUser_GetSessionTicket(void *SteamUserContext, char *HexTicket, int32 HexTicketLength);

/*********************************************************************/

int32 SteamUser_GetInstanceId(void *SteamUserContext, char *String, int32 MaxString);

int32 SteamUser_Process(void *SteamUserContext);
int32 SteamUser_Invoke(void *SteamUserContext, char *Method, char *ResultString, int32 ResultStringLength);

/*********************************************************************/

int32 SteamUser_Create(void **SteamUserContext);
void* SteamUser_AddRef(void *SteamUserContext);
int32 SteamUser_Release(void **SteamUserContext);

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif