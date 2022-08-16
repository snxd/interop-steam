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

function interopLoaded() {

  //notificationCenter.verbose = true;
  //interop.verbose = true;

  var isSteamRunning = steamAPI.isSteamRunning();
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

  var isLoggedOn = steamUser.isLoggedOn();
  console.log(`SteamUser - IsLoggedOn - ${isLoggedOn}`);
  if (!isLoggedOn) {
    return;
  }

  var appId = steamUtils.getAppID();

  console.log(`SteamUtils - AppID - ${appId}`);
  console.log(`SteamUtils - IsOverlayEnabled - ${steamUtils.isOverlayEnabled()}`);
  console.log(`SteamUtils - IsSteamRunningInVR - ${steamUtils.isSteamRunningInVR()}`);
  console.log(`SteamUtils - IsSteamInBigPictureMode - ${steamUtils.isSteamInBigPictureMode()}`);
  console.log(`SteamUtils - IsVRHeadsetStreamingEnabled - ${steamUtils.isVRHeadsetStreamingEnabled()}`);
  console.log(`SteamUtils - ServerRealTime - ${steamUtils.getServerRealTime()}`);
  console.log(`SteamUtils - IPCountry - ${steamUtils.getIPCountry()}`);
  console.log(`SteamUtils - CurrentBatteryPower - ${steamUtils.getCurrentBatteryPower()}`);

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

  for (var x = 0; x < steamApps.getDLCCount(); x += 1) {
    var dlcData = steamApps.getDLCData(x, appId);
    console.log(`SteamApps - DLC - ${x} - Data ${JSON.stringify(dlcData)}`);
    var isDLCInstalled = steamApps.isDLCInstalled(dlcData.id);
    console.log(`SteamApps - DLC - ${x} - IsInstalled ${isDLCInstalled}`);
  }

  var mySteamId = steamUser.getSteamId();

  console.log(`SteamUser - GetSteamId - ${mySteamId}`);
  console.log(`SteamUser - IsBehindNAT - ${steamUser.isBehindNAT()}`);
  console.log(`SteamUser - IsPhoneVerified - ${steamUser.isPhoneVerified()}`);
  console.log(`SteamUser - IsTwoFactorEnabled - ${steamUser.isTwoFactorEnabled()}`);
  console.log(`SteamUser - IsPhoneIdentifying - ${steamUser.isPhoneIdentifying()}`);
  console.log(`SteamUser - IsPhoneRequiringVerification - ${steamUser.isPhoneRequiringVerification()}`);
  console.log(`SteamUser - GetPlayerSteamLevel - ${steamUser.getPlayerSteamLevel()}`);

  var ob0 = notificationCenter.addInstanceObserver("SteamUser", "AuthSessionTicketResponse", steamUser, function (sender, info) {
    console.log(`SteamUser - AuthSessionTicketResponse - ${JSON.stringify(info)}`);
    ob0.release();
  });
  console.log(`SteamUser - AuthSessionTicket - ${steamUser.getAuthSessionTicket()}`);

  var myPersonaName = steamFriends.getFriendPersonaName(mySteamId);
  var myPersonaState = steamFriends.getFriendPersonaState(mySteamId);
  var myPersonaStateString = steamFriendsPersonaState.nameFromId(myPersonaState);

  console.log(`SteamFriends - Current User - ${myPersonaName} (${myPersonaStateString})`);

  function convertBase64ToBinary(base64) {
    var raw = window.atob(base64);
    var array = new Uint8ClampedArray(new ArrayBuffer(raw.length));
    for (var i = 0; i < raw.length; i++) {
      array[i] = raw.charCodeAt(i);
    }
    return array;
  }

  var canvas = document.getElementById("friendsList");
  var ctx = null;
  if (canvas) {
    canvas.height = 600;
    ctx = canvas.getContext("2d");
  }

  var friendCount = steamFriends.getFriendCount();
  console.log(`SteamFriends - FriendCount - ${friendCount}`);

  for (var i = 0, y = 0; i < friendCount; i += 1) {
    var friendSteamId = steamFriends.getFriendByIndex(i);
    var friendPersonaName = steamFriends.getFriendPersonaName(friendSteamId);
    var friendPersonaState = steamFriends.getFriendPersonaState(friendSteamId);
    var friendPersonaStateString = steamFriendsPersonaState.nameFromId(friendPersonaState);
    var friendRelationship = steamFriends.getFriendRelationship(friendSteamId);
    var friendRelationshipString = steamFriendsRelationship.nameFromId(friendRelationship);

    console.log(`SteamFriends - Friend ${(i + 1)} - ${friendSteamId} - ${friendPersonaName} (${friendPersonaStateString}) - ${friendRelationshipString}`);

    var friendAvatarIndex = steamFriends.getSmallFriendAvatar(friendSteamId);
    var friendAvatarWidth = steamUtils.getImageWidth(friendAvatarIndex);
    var friendAvatarHeight = steamUtils.getImageHeight(friendAvatarIndex);

    console.log(`SteamUtils - Image ${friendAvatarIndex} - ${friendAvatarWidth}x${friendAvatarHeight}`);

    if (canvas) {
      var friendAvatarRGBA = steamUtils.getImageRGBA(friendAvatarIndex);
      var friendAvatarImageArray = convertBase64ToBinary(friendAvatarRGBA);
      var friendAvatarImageData = new ImageData(friendAvatarImageArray, friendAvatarWidth, friendAvatarHeight);

      ctx.putImageData(friendAvatarImageData, 0, y);
      ctx.fillStyle = "white";
      ctx.fillText(`${friendPersonaName} (${friendPersonaState})`, friendAvatarWidth + 10, y + ((friendAvatarHeight / 2) + 10));

      y += friendAvatarHeight;
    }
  }

  var ob1 = notificationCenter.addInstanceObserver("SteamUserStats", "NumberOfCurrentPlayersResponse", steamUserStats, function (sender, info) {
    console.log(`SteamUserStats - NumberOfCurrentPlayersResponse - ${JSON.stringify(info)}`);
    ob1.release();
  });

  var achievementCount = steamUserStats.getNumberOfAchievements();
  var ob2 = notificationCenter.addInstanceObserver("SteamUserStats", "UserStatsReceivedResponse", steamUserStats, function (sender, info) {

    console.log(`SteamUserStats - UserStatsReceivedResponse - ${JSON.stringify(info)}`);
    steamUserStats.requestGlobalAchievementPercentages();

    var ob3 = notificationCenter.addInstanceObserver("SteamUserStats", "GlobalAchievementPercentagesResponse", steamUserStats, function (sender, info) {
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

      ob3.release();
    });

    ob2.release();
  });

  // Wait for results in notification callbacks
  steamUserStats.getNumberOfCurrentPlayers();
  steamUserStats.requestUserStats(mySteamId);

  steamFriends.activateGameOverlayToWebPage("http://google.com/");
}

function interopUnloaded() {
  // Release instances
}

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
