// Remember to add steam.js to your HTML scripts
(function () {

    function interopLoaded() {

        //notificationCenter.verbose = true;
        //interop.verbose = true;

        var isSteamRunning = steamAPI.isSteamRunning();
        console.log("SteamAPI - IsSteamRunning - {0}".format(isSteamRunning));
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
        console.log("SteamUser - IsLoggedOn - {0}".format(isLoggedOn));
        if (!isLoggedOn) {
            return;
        }

        var appId = steamUtils.getAppID();

        console.log("SteamUtils - AppID - {0}".format(appId));
        console.log("SteamUtils - IsOverlayEnabled - {0}".format(steamUtils.isOverlayEnabled()));
        console.log("SteamUtils - IsSteamRunningInVR - {0}".format(steamUtils.isSteamRunningInVR()));
        console.log("SteamUtils - IsSteamInBigPictureMode - {0}".format(steamUtils.isSteamInBigPictureMode()));
        console.log("SteamUtils - IsVRHeadsetStreamingEnabled - {0}".format(steamUtils.isVRHeadsetStreamingEnabled()));
        console.log("SteamUtils - ServerRealTime - {0}".format(steamUtils.getServerRealTime()));
        console.log("SteamUtils - IPCountry - {0}".format(steamUtils.getIPCountry()));
        console.log("SteamUtils - CurrentBatteryPower - {0}".format(steamUtils.getCurrentBatteryPower()));

        console.log("SteamApps - IsAppInstalled - {0}".format(steamApps.isAppInstalled(appId)));
        console.log("SteamApps - IsSubscribed - {0}".format(steamApps.isSubscribed()));
        console.log("SteamApps - IsSubscribedFromWeekend - {0}".format(steamApps.isSubscribedFromWeekend()));
        console.log("SteamApps - IsVACBanned - {0}".format(steamApps.isVACBanned()));
        console.log("SteamApps - IsCybercafe - {0}".format(steamApps.isCybercafe()));
        console.log("SteamApps - IsLowViolence - {0}".format(steamApps.isLowViolence()));
        console.log("SteamApps - CurrentBetaName - {0}".format(steamApps.getCurrentBetaName()));
        console.log("SteamApps - AppInstallDir - {0}".format(steamApps.getAppInstallDir(appId)));
        console.log("SteamApps - AppBuildId - {0}".format(steamApps.getAppBuildId()));
        console.log("SteamApps - GetDLCCount - {0}".format(steamApps.getDLCCount()));

        for (var x = 0; x < steamApps.getDLCCount(); x += 1) {
            var dlcData = steamApps.getDLCData(x, appId);
            console.log("SteamApps - DLC - {0} - Data {1}".format(x, JSON.stringify(dlcData)));
            var isDLCInstalled = steamApps.isDLCInstalled(dlcData.id);
            console.log("SteamApps - DLC - {0} - IsInstalled {1}".format(x, isDLCInstalled));
        }

        var mySteamId = steamUser.getSteamId();

        console.log("SteamUser - GetSteamId - {0}".format(mySteamId));
        console.log("SteamUser - IsBehindNAT - {0}".format(steamUser.isBehindNAT()));
        console.log("SteamUser - IsPhoneVerified - {0}".format(steamUser.isPhoneVerified()));
        console.log("SteamUser - IsTwoFactorEnabled - {0}".format(steamUser.isTwoFactorEnabled()));
        console.log("SteamUser - IsPhoneIdentifying - {0}".format(steamUser.isPhoneIdentifying()));
        console.log("SteamUser - IsPhoneRequiringVerification - {0}".format(steamUser.isPhoneRequiringVerification()));
        console.log("SteamUser - GetPlayerSteamLevel - {0}".format(steamUser.getPlayerSteamLevel()));

        var ob0 = notificationCenter.addInstanceObserver("SteamUser", "AuthSessionTicketResponse", steamUser, function (sender, info) {
            console.log("SteamUser - AuthSessionTicketResponse - {0}".format(JSON.stringify(info)));
            ob0.release();
        });
        console.log("SteamUser - AuthSessionTicket - {0}".format(steamUser.getAuthSessionTicket()));

        var myPersonaName = steamFriends.getFriendPersonaName(mySteamId);
        var myPersonaState = steamFriends.getFriendPersonaState(mySteamId);
        var myPersonaStateString = steamFriendsPersonaState.nameFromId(myPersonaState);

        console.log("SteamFriends - Current User - {0} ({1})".format(myPersonaName, myPersonaStateString));

        function convertBase64ToBinary(base64) {
            var raw = window.atob(base64);
            var array = new Uint8ClampedArray(new ArrayBuffer(raw.length));
            for (var i = 0; i < raw.length; i++) {
                array[i] = raw.charCodeAt(i);
            }
            return array;
        };

        var canvas = document.getElementById("canvas");
        var ctx = null;
        if (canvas) {
            canvas.height = 600;
            ctx = canvas.getContext('2d');
        }

        var friendCount = steamFriends.getFriendCount();
        console.log("SteamFriends - FriendCount - {0}".format(friendCount));

        for (var i = 0, y = 0; i < friendCount; i += 1)
        {
            var friendSteamId = steamFriends.getFriendByIndex(i);
            var friendPersonaName = steamFriends.getFriendPersonaName(friendSteamId);
            var friendPersonaState = steamFriends.getFriendPersonaState(friendSteamId);
            var friendPersonaStateString = steamFriendsPersonaState.nameFromId(friendPersonaState);
            var friendRelationship = steamFriends.getFriendRelationship(friendSteamId);
            var friendRelationshipString = steamFriendsRelationship.nameFromId(friendRelationship);

            console.log("SteamFriends - Friend {0} - {1} - {2} ({3}) - {4}".
                format((i + 1), friendSteamId, friendPersonaName, friendPersonaStateString, friendRelationshipString));

            var friendAvatarIndex = steamFriends.getSmallFriendAvatar(friendSteamId);
            var friendAvatarWidth = steamUtils.getImageWidth(friendAvatarIndex);
            var friendAvatarHeight = steamUtils.getImageHeight(friendAvatarIndex);

            console.log("SteamUtils - Image {0} - {1}x{2}".format(friendAvatarIndex, friendAvatarWidth, friendAvatarHeight));

            if (canvas) {
                var friendAvatarRGBA = steamUtils.getImageRGBA(friendAvatarIndex);
                var friendAvatarImageArray = convertBase64ToBinary(friendAvatarRGBA);
                var friendAvatarImageData = new ImageData(friendAvatarImageArray, friendAvatarWidth, friendAvatarHeight);

                ctx.putImageData(friendAvatarImageData, 0, y);
                ctx.fillStyle = "white";
                ctx.fillText("{0} ({1})".format(friendPersonaName, friendPersonaState), friendAvatarWidth + 10, y + ((friendAvatarHeight / 2) + 10));

                y += friendAvatarHeight;
            }
        }

        var ob1 = notificationCenter.addInstanceObserver("SteamUserStats", "NumberOfCurrentPlayersResponse", steamUserStats, function (sender, info) {
            console.log("SteamUserStats - NumberOfCurrentPlayersResponse - {0}".format(JSON.stringify(info)));
            ob1.release();
        });

        var achievementCount = steamUserStats.getNumberOfAchievements();
        var ob2 = notificationCenter.addInstanceObserver("SteamUserStats", "UserStatsReceivedResponse", steamUserStats, function (sender, info) {

            console.log("SteamUserStats - UserStatsReceivedResponse - {0}".format(JSON.stringify(info)));
            steamUserStats.requestGlobalAchievementPercentages();

            var ob3 = notificationCenter.addInstanceObserver("SteamUserStats", "GlobalAchievementPercentagesResponse", steamUserStats, function (sender, info) {
                console.log("SteamUserStats - NumberOfAchievements - {0}".format(achievementCount));

                for (var i = 0; i < achievementCount; i += 1) {
                    var achievementName = steamUserStats.getAchievementName(i);
                    var achievementFriendlyName = steamUserStats.getAchievementDisplayAttribute(achievementName, "name");
                    var achievementDescription = steamUserStats.getAchievementDisplayAttribute(achievementName, "desc");
                    var achievementHidden = steamUserStats.getAchievementDisplayAttribute(achievementName, "hidden");
                    var achievementAchieved = steamUserStats.getAchievement(achievementName);
                    var achievementAchievedPercent = steamUserStats.getAchievementAchievedPercent(achievementName);

                    console.log("SteamUserStats - Achievement {0} - {1} - \"{2}\" - {3} - {4}{5}{6}%".
                        format((i + 1), achievementName, achievementFriendlyName, achievementDescription,
                            (achievementHidden == "1") ? "Invisible " : "", (achievementAchieved) ? "Achieved ": "",
                            achievementAchievedPercent));
                }

                ob3.release();
            });

            ob2.release();
        });

        // Wait for results in notification callbacks
        steamUserStats.getNumberOfCurrentPlayers();
        steamUserStats.requestUserStats(mySteamId);

        steamFriends.activateGameOverlayToWebPage("http://google.com/");
    };

    function interopUnloaded() {
        // Release our notification center instance observer
        if (!isNull(observer)) {
            observer.release();
        }
    };

    interop.on("load", function(info) {
        if (info.name.toLowerCase() == "steam") {
            interopLoaded(info);
        }
    });

    interop.on("unload", function(info) {
        if (info.name.toLowerCase() == "steam") {
            interopUnloaded(info);
        }
    });
}());
