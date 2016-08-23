#pragma once

#ifndef _SIMPLEOBJECT_H_
#define _SIMPLEOBJECT_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef FALSE
#define FALSE   (0)
#endif

#ifndef TRUE
#define TRUE    (1)
#endif

/*********************************************************************/

void SteamAuth_SetInstanceId(void *SteamAuthContext, char *String);
void SteamAuth_SetInteropExecuteCallback(void *SteamAuthContext, void *UserPtr, Interop_ExecuteCallback Execute);

// Interop Callbacks
int32 SteamAuth_ProcessInstance(void *SteamAuthContext, char *InstanceId,  int32 *Continue);
int32 SteamAuth_InvokeInstance(void *SteamAuthContext, char *InstanceId, char *Method, char *ResultString, int32 ResultStringLength);
int32 SteamAuth_RemoveInstance(void *SteamAuthContext, char *InstanceId);

/*********************************************************************/

void SteamAuth_IsLoggedOn(void *SteamAuthContext, int32 *Result);

void SteamAuth_GetSteamId(void *SteamAuthContext, uint64 *Result);

void SteamAuth_GetSessionTicket(void *SteamAuthContext, char *HexTicket, int32 HexTicketLength);

/*********************************************************************/

void SteamAuth_Create(void **SteamAuthContext);
void SteamAuth_Delete(void **SteamAuthContext);

/*********************************************************************/

#endif