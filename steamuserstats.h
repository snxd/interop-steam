#pragma once

#ifndef _STEAMUSERSTATS_H_
#define _STEAMUSERSTATS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

int32 SteamUserStats_GetNumberOfCurrentPlayers(void *SteamUserStatsContext);
int32 SteamUserStats_GetNumberOfAchievements(void *SteamUserStatsContext, int32 *Achievements);
int32 SteamUserStats_GetAchievement(void *SteamUserStatsContext, char *Name, int32 *Achieved);
int32 SteamUserStats_GetAchievementNamePtr(void *SteamUserStatsContext, int32 Index, char **Name);
int32 SteamUserStats_GetAchievementIcon(void *SteamUserStatsContext, char *Name, int32 *ImageIndex);
int32 SteamUserStats_GetAchievementDisplayAttributePtr(void *SteamUserStatsContext, char *Name, char *Key, char **AttributeValue);
int32 SteamUserStats_SetAchievement(void *SteamUserStatsContext, char *Name);
int32 SteamUserStats_ClearAchievement(void *SteamUserStatsContext, char *Name);
int32 SteamUserStats_GetUserAchievement(void *SteamUserStatsContext, uint64 SteamId, char *Name, int32 *Achieved);
int32 SteamUserStats_GetAchievementAchievedPercent(void *SteamUserStatsContext, char *Name, float32 *PercentAchieved);

int32 SteamUserStats_RequestUserStats(void *SteamUserStatsContext, uint64 FriendId);
int32 SteamUserStats_RequestGlobalAchievementPercentages(void *SteamUserStatsContext);
int32 SteamUserStats_ResetAllStats(void *SteamUserStatsContext, int32 AchievementsToo);

/*********************************************************************/

int32 SteamUserStats_GetInstanceId(void *SteamUserStatsContext, char *String, int32 MaxString);

int32 SteamUserStats_Process(void *SteamUserStatsContext);
int32 SteamUserStats_Invoke(void *SteamUserStatsContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle);

/*********************************************************************/

int32 SteamUserStats_Create(void **SteamUserStatsContext);
void* SteamUserStats_AddRef(void *SteamUserStatsContext);
int32 SteamUserStats_Release(void **SteamUserStatsContext);

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif