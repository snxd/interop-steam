/** @file SteamExample.js
 *  @brief example using the Steam classes
 */

import {interop, notificationCenter} from "../direct/Host";

import {steamAPI} from "./SteamAPI";
import {steamApps} from "./SteamApps";
import {steamFriends, steamFriendsPersonaState, steamFriendsRelationship} from "./SteamFriends";
import {steamUser} from "./SteamUser";
import {steamUserStats} from "./SteamUserStats";
import {steamUtils} from "./SteamUtils";

const printSteamUtils = (appId) => {
  console.log(`SteamUtils - AppID - ${appId}`);
  console.log(`SteamUtils - IsOverlayEnabled - ${steamUtils.isOverlayEnabled()}`);
  console.log(`SteamUtils - IsSteamRunningInVR - ${steamUtils.isSteamRunningInVR()}`);
  console.log(`SteamUtils - IsSteamInBigPictureMode - ${steamUtils.isSteamInBigPictureMode()}`);
  console.log(`SteamUtils - IsVRHeadsetStreamingEnabled - ${steamUtils.isVRHeadsetStreamingEnabled()}`);
  console.log(`SteamUtils - ServerRealTime - ${steamUtils.getServerRealTime()}`);
  console.log(`SteamUtils - IPCountry - ${steamUtils.getIPCountry()}`);
  console.log(`SteamUtils - CurrentBatteryPower - ${steamUtils.getCurrentBatteryPower()}`);
};

const printSteamApps = (appId) => {
  console.log(`SteamApps - IsAppInstalled - ${steamApps.isAppInstalled(appId)}`);
  console.log(`SteamApps - IsSubscribed - ${steamApps.isSubscribed()}`);
  console.log(`SteamApps - IsSubscribedFromWeekend - ${steamApps.isSubscribedFromWeekend()}`);
  console.log(`SteamApps - IsVACBanned - ${steamApps.isVACBanned()}`);
  console.log(`SteamApps - IsCybercafe - ${steamApps.isCybercafe()}`);
  console.log(`SteamApps - IsLowViolence - ${steamApps.isLowViolence()}`);
  console.log(`SteamApps - CurrentBetaName - ${steamApps.getCurrentBetaName()}`);
  console.log(`SteamApps - AppInstallDir - ${steamApps.getAppInstallDir(appId)}`);
  console.log(`SteamApps - AppBuildId - ${steamApps.getAppBuildId()}`);
  console.log(`SteamApps - GetDLCCount - ${steamApps.getDLCCount()}`);
};

const printSteamAppsDLCs = (appId) => {
  for (var x = 0; x < steamApps.getDLCCount(); x += 1) {
    var dlcData = steamApps.getDLCData(x, appId);
    console.log(`SteamApps - DLC - ${x} - Data ${JSON.stringify(dlcData)}`);
    var isDLCInstalled = steamApps.isDLCInstalled(dlcData.id);
    console.log(`SteamApps - DLC - ${x} - IsInstalled ${isDLCInstalled}`);
  }
};

const printSteamUser = (mySteamId) => {
  console.log(`SteamUser - GetSteamId - ${mySteamId}`);
  console.log(`SteamUser - IsBehindNAT - ${steamUser.isBehindNAT()}`);
  console.log(`SteamUser - IsPhoneVerified - ${steamUser.isPhoneVerified()}`);
  console.log(`SteamUser - IsTwoFactorEnabled - ${steamUser.isTwoFactorEnabled()}`);
  console.log(`SteamUser - IsPhoneIdentifying - ${steamUser.isPhoneIdentifying()}`);
  console.log(`SteamUser - IsPhoneRequiringVerification - ${steamUser.isPhoneRequiringVerification()}`);
  console.log(`SteamUser - GetPlayerSteamLevel - ${steamUser.getPlayerSteamLevel()}`);
};

const printSteamUserAuthSessionTicket = () => {
  var observer = notificationCenter.addInstanceObserver("SteamUser", "AuthSessionTicketResponse", steamUser, (sender, info) => {
    console.log(`SteamUser - AuthSessionTicketResponse - ${JSON.stringify(info)}`);
    observer.release();
  });
  // Wait for response from SteamUser.AuthSessionTicketResponse notification
  console.log(`SteamUser - AuthSessionTicket - ${steamUser.getAuthSessionTicket()}`);
};

const printSteamFriends = (mySteamId) => {
  var myPersonaName = steamFriends.getFriendPersonaName(mySteamId);
  var myPersonaState = steamFriends.getFriendPersonaState(mySteamId);
  var myPersonaStateString = steamFriendsPersonaState.nameFromId(myPersonaState);

  console.log(`SteamFriends - Current User - ${myPersonaName} (${myPersonaStateString})`);

  // Print friends list
  var friendCount = steamFriends.getFriendCount();
  console.log(`SteamFriends - FriendCount - ${friendCount}`);

  for (var i = 0; i < friendCount; i++) {
    var friendSteamId = steamFriends.getFriendByIndex(i);
    var friendPersonaName = steamFriends.getFriendPersonaName(friendSteamId);
    var friendPersonaState = steamFriends.getFriendPersonaState(friendSteamId);
    var friendPersonaStateString = steamFriendsPersonaState.nameFromId(friendPersonaState);
    var friendRelationship = steamFriends.getFriendRelationship(friendSteamId);
    var friendRelationshipString = steamFriendsRelationship.nameFromId(friendRelationship);

    console.log(`SteamFriends - Friend ${(i + 1)} - ${friendSteamId} - ${friendPersonaName} (${friendPersonaStateString}) - ${friendRelationshipString}`);
  }
};

const activateSteamFriendsOverlayWebPage = (url) => {
  steamFriends.activateGameOverlayToWebPage(url);
};

const printSteamUserStatsNumberOfPlayers = () => {
  var observer = notificationCenter.addInstanceObserver("SteamUserStats", "NumberOfCurrentPlayersResponse", steamUserStats, (sender, info) => {
    console.log(`SteamUserStats - NumberOfCurrentPlayersResponse - ${JSON.stringify(info)}`);
    observer.release();
  });

  // Wait for result in SteamUserStats.NumberOfCurrentPlayersResponse notification
  steamUserStats.getNumberOfCurrentPlayers();
};

const printSteamUserStatsAchievements = (mySteamId) => {
  var achievementCount = steamUserStats.getNumberOfAchievements();

  var observer2 = notificationCenter.addInstanceObserver("SteamUserStats", "UserStatsReceivedResponse", steamUserStats, (sender, info) => {
    console.log(`SteamUserStats - UserStatsReceivedResponse - ${JSON.stringify(info)}`);

    var observer3 = notificationCenter.addInstanceObserver("SteamUserStats", "GlobalAchievementPercentagesResponse", steamUserStats, () => {
      console.log(`SteamUserStats - NumberOfAchievements - ${achievementCount}`);

      for (var i = 0; i < achievementCount; i += 1) {
        var achievementName = steamUserStats.getAchievementName(i);
        var achievementFriendlyName = steamUserStats.getAchievementDisplayAttribute(achievementName, "name");
        var achievementDescription = steamUserStats.getAchievementDisplayAttribute(achievementName, "desc");
        var achievementHidden = steamUserStats.getAchievementDisplayAttribute(achievementName, "hidden");
        var achievementAchieved = steamUserStats.getAchievement(achievementName);
        var achievementAchievedPercent = steamUserStats.getAchievementAchievedPercent(achievementName);

        console.log(`SteamUserStats - Achievement ${(i + 1)} - ${achievementName} - "${achievementFriendlyName}" - ${achievementDescription} - ${(achievementHidden === "1") ? "Invisible " : ""}${(achievementAchieved) ? "Achieved ": ""}${achievementAchievedPercent}%`);
      }

      observer3.release();
    });

    // Wait for result in SteamUserStats.GlobalAchievementPercentagesResponse notification
    steamUserStats.requestGlobalAchievementPercentages();

    observer2.release();
  });

  // Wait for result in SteamUserStats.UserStatsReceivedResponse notification
  steamUserStats.requestUserStats(mySteamId);
};

const interopLoaded = () => {
  // Check if steam is running
  const isSteamRunning = steamAPI.isSteamRunning();
  console.log(`SteamAPI - IsSteamRunning - ${isSteamRunning}`);
  if (!isSteamRunning) {
    console.warn("Steam must be running");
    return;
  }

  steamAPI.setAppId(287450); // Rise of Nations app id for testing
  if (!steamAPI.initialize()) {
    console.warn("Cannot initialize steam for app");
    return;
  }

  // Check if user is logged in to steam
  const isLoggedOn = steamUser.isLoggedOn();
  console.log(`SteamUser - IsLoggedOn - ${isLoggedOn}`);
  if (!isLoggedOn) {
    return;
  }

  const appId = steamUtils.getAppID();

  printSteamUtils(appId);
  printSteamApps(appId);
  printSteamAppsDLCs(appId);

  const mySteamId = steamUser.getSteamId();
  printSteamUser(mySteamId);
  printSteamUserAuthSessionTicket();

  printSteamFriends(mySteamId);

  printSteamUserStatsNumberOfPlayers();
  printSteamUserStatsAchievements(mySteamId);

  activateSteamFriendsOverlayWebPage("http://google.com/");
};

const interopUnloaded = () => {
  // Release instances
};

interop.on("load", function(info) {
  if (info.name === "steam") {
    interopLoaded(info);
  }
});

interop.on("unload", function(info) {
  if (info.name === "steam") {
    interopUnloaded(info);
  }
});
