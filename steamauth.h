#pragma once

#ifndef _STEAMAUTH_H_
#define _STEAMAUTH_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

int32 SteamAuth_IsLoggedOn(void *SteamAuthContext, int32 *Result);
int32 SteamAuth_GetSteamId(void *SteamAuthContext, uint64 *Result);
int32 SteamAuth_GetSessionTicket(void *SteamAuthContext, char *HexTicket, int32 HexTicketLength);

/*********************************************************************/

int32 SteamAuth_GetInstanceId(void *SteamAuthContext, char *String, int32 MaxString);
int32 SteamAuth_SetInteropExecuteCallback(void *SteamAuthContext, void *UserPtr, Interop_ExecuteCallback Execute);

int32 SteamAuth_Process(void *SteamAuthContext);
int32 SteamAuth_Invoke(void *SteamAuthContext, char *Method, char *ResultString, int32 ResultStringLength);

/*********************************************************************/

int32 SteamAuth_Create(void **SteamAuthContext);
void* SteamAuth_AddRef(void *SteamAuthContext);
int32 SteamAuth_Release(void **SteamAuthContext);

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif