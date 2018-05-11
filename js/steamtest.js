// Remember to add steam.js to your HTML scripts
(function () {
    var steamUser = null;
    var steamUserStats = null;
    var steamUtils = null;
    var steamFriends = null;
    var observer = null;

    function interopLoaded() {

        //notificationCenter.verbose = true;
        //interop.verbose = true;

        steamApp.setId("570"); // Dota2 app id for testing

        steamUtils = createSteamUtils();
        steamUser = createSteamUser();
        observer = notificationCenter.addInstanceObserver("SteamUser", "AuthSessionTicketResponse", steamUser, function (sender, info) {
            console.log("SteamUser - AuthSessionTicketResponse - {0}".format(JSON.stringify(info)));
        });

        var mySteamId = steamUser.getSteamId();

        console.log("SteamUtils - IsOverlayEnabled - {0}".format(steamUtils.isOverlayEnabled()));
        console.log("SteamUtils - IsSteamRunningInVR - {0}".format(steamUtils.isSteamRunningInVR()));
        console.log("SteamUtils - IsSteamInBigPictureMode - {0}".format(steamUtils.isSteamInBigPictureMode()));
        console.log("SteamUtils - IsVRHeadsetStreamingEnabled - {0}".format(steamUtils.isVRHeadsetStreamingEnabled()));
        console.log("SteamUtils - ServerRealTime - {0}".format(steamUtils.getServerRealTime()));
        console.log("SteamUtils - IPCountry - {0}".format(steamUtils.getIPCountry()));
        console.log("SteamUtils - CurrentBatteryPower - {0}".format(steamUtils.getCurrentBatteryPower()));
        console.log("SteamUtils - AppID - {0}".format(steamUtils.getAppID()));

        console.log("SteamUser - GetSteamId - {0}".format(mySteamId));
        console.log("SteamUser - IsLoggedOn - {0}".format(steamUser.isLoggedOn()));
        console.log("SteamUser - IsBehindNAT - {0}".format(steamUser.isBehindNAT()));
        console.log("SteamUser - IsPhoneVerified - {0}".format(steamUser.isPhoneVerified()));
        console.log("SteamUser - IsTwoFactorEnabled - {0}".format(steamUser.isTwoFactorEnabled()));
        console.log("SteamUser - IsPhoneIdentifying - {0}".format(steamUser.isPhoneIdentifying()));
        console.log("SteamUser - IsPhoneRequiringVerification - {0}".format(steamUser.isPhoneRequiringVerification()));
        console.log("SteamUser - GetPlayerSteamLevel - {0}".format(steamUser.getPlayerSteamLevel()));
        console.log("SteamUser - AuthSessionTicket - {0}".format(steamUser.getAuthSessionTicket()));

        steamUserStats = createSteamUserStats();
        observer = notificationCenter.addInstanceObserver("SteamUserStats", "NumberOfCurrentPlayersResponse", steamUserStats, function (sender, info) {
            console.log("SteamUserStats - NumberOfCurrentPlayersResponse - {0}".format(JSON.stringify(info)));
        });
        console.log("SteamUserStats - NumberOfCurrentPlayers - {0}".format(steamUserStats.getNumberOfCurrentPlayers()));
        console.log("SteamUserStats - NumberOfAchievements - {0}".format(steamUserStats.getNumberOfAchievements()));

        steamFriends = createSteamFriends();

        var myPersonaName = steamFriends.getFriendPersonaName(mySteamId);
        var myPersonaState = steamFriends.getFriendPersonaState(mySteamId);
        var myPersonaStateString = steamFriendsPersonaState.nameFromId(myPersonaState);

        console.log("SteamFriends - Current User - {0} ({1})".format(myPersonaName, myPersonaStateString));

        var friendCount = steamFriends.getFriendCount();
        console.log("SteamFriends - GetFriendCount - {0}".format(friendCount));
        for (var i = 0; i < friendCount; i += 1)
        {
            var friendSteamId = steamFriends.getFriendByIndex(i);
            var friendPersonaName = steamFriends.getFriendPersonaName(friendSteamId);
            var friendPersonaState = steamFriends.getFriendPersonaState(friendSteamId);
            var friendPersonaStateString = steamFriendsPersonaState.nameFromId(friendPersonaState);
            var friendRelationship = steamFriends.getFriendRelationship(friendSteamId);
            var friendRelationshipString = steamFriendsRelationship.nameFromId(friendRelationship);

            console.log("SteamFriends - GetFriend {0} - {1} - {2} ({3}) - {4}".
                format(i, friendSteamId, friendPersonaName, friendPersonaStateString, friendRelationshipString));
        }
        steamFriends.activateGameOverlayToWebPage("http://google.com/");
    };

    function interopUnloaded() {
        // Release our notification center instance observer
        if (!isNull(observer)) {
            observer.release();
        }
        // Release our objects
        if (!isNull(steamUtils)) {
            steamUtils.release();
        }
        if (!isNull(steamUser)) {
            steamUser.release();
        }
        if (!isNull(steamUserStats)) {
            steamUserStats.release();
        }
        if (!isNull(steamFriends)) {
            steamFriends.release();
        }
    };

    interop.on("libraryLoad", function(info) {
        if (info.name.toLowerCase() == "steam") {
            interopLoaded();
        }
    });

    interop.on("libraryUnload", function(info) {
        if (info.name.toLowerCase() == "steam") {
            interopUnloaded();
        }
    });
}());
