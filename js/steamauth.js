(function() {
    var loadObs, unloadObs;
    loadObs = notificationCenter.addObserver("Interop", "DidLoad", function(sender, info) {
        if (info.name !== "steam") 
        { 
    	    return; 
        }
        loadObs.release();

        function SteamClient(instanceId) {
            this.instanceId = instanceId;
        }

        SteamClient.prototype.release = function() {
            interop.releaseInstance(this.instanceId);
        }

        SteamClient.prototype.isLoggedOn = function() {
            var methodBinding = {
                "method": "isLoggedOn"
            }
            return interop.invoke(this.instanceId, methodBinding);
        }

        SteamClient.prototype.getSteamId = function() {
            var methodBinding = {
                "method": "getSteamId"
            }
            return interop.invoke(this.instanceId, methodBinding);
        }

        SteamClient.prototype.getSessionTicket = function() {
            var methodBinding = {
                "method": "getSessionTicket"
            }
            return interop.invoke(this.instanceId, methodBinding);
        }

        SteamClient.prototype.setSteamAppId = function(appId) {
            var methodBinding = {
                "method": "setAppId",
                "value" : String(appId)
            }
            return interop.invoke(this.instanceId, methodBinding);
        }

        function createSteamClient() {
            return interop.createInstance("Steam.Auth", SteamClient);
        }

        // Create steam singleton object
        window.steam = interop.createInstance("Steam.Auth", SteamClient);
        unloadObs = notificationCenter.addObserver("Interop", "WillUnload", function(sender, info) {
            if (info.name !== "steam") 
            { 
        	    return; 
            }
            unloadObs.release();
            window.steam.release();
            window.steam = null;
        });
    });
}());
