// Remember to add steam.js to your HTML scripts
(function () {
    var steamUser = null;
    var steamUserStats = null;
    var observer = null;
    
    function interopLoaded() {

        //notificationCenter.verbose = true;
        //interop.verbose = true;
        
        steamApp.setId("570"); // Dota2 app id for testing
        
        steamUser = createSteamUser();
        observer = notificationCenter.addInstanceObserver("SteamUser", "AuthSessionTicketResponse", steamUser, function (sender, info) {
            console.log("SteamUser - AuthSessionTicketResponse - {0}".format(JSON.stringify(info)));
        });
        
        console.log("SteamUser - GetSteamId - {0}".format(steamUser.getSteamId()));
        console.log("SteamUser - IsLoggedOn - {0}".format(steamUser.isLoggedOn()));
        console.log("SteamUser - AuthSessionTicket - {0}".format(steamUser.getAuthSessionTicket()));
        
        steamUserStats = createSteamUserStats();
        observer = notificationCenter.addInstanceObserver("SteamUserStats", "NumberOfCurrentPlayersResponse", steamUserStats, function (sender, info) {
            console.log("SteamUserStats - NumberOfCurrentPlayersResponse - {0}".format(JSON.stringify(info)));
        });
        console.log("SteamUserStats - NumberOfCurrentPlayers - {0}".format(steamUserStats.getNumberOfCurrentPlayers()));
        
        steamFriends = createSteamFriends();
        var friendCount = steamFriends.getFriendCount();
        console.log("SteamFriends - GetFriendCount - {0}".format(friendCount));
        for (var i = 0; i < friendCount; i += 1)
        {
            var friendSteamId = steamFriends.getFriendByIndex(i);
            console.log("SteamFriends - GetFriendByIndex - {0}".format(friendSteamId));
            var friendPersonaName = steamFriends.getFriendPersonaName(friendSteamId);
            console.log("SteamFriends - GetFriendPersonaName - {0}".format(friendPersonaName));
        }
        steamFriends.activateGameOverlayToWebPage("http://google.com/");
    };
    
    function interopUnloaded() {
        // Release our notification center instance observer
        if (!isNull(observer)) {
            observer.release();
        }
        // Release our objects
        if (!isNull(steamUser)) {
            steamUser.release();
        }
        if (!isNull(steamUserStats)) {
            steamUserStats.release();
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