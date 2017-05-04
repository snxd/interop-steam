/** @file steamuserstats.js
 *  @class Steam user stats
 *  @brief 
 */
(function (root) {
    function SteamUserStats(instanceId) {
        this.instanceId = instanceId;
    }
    
    SteamUserStats.prototype = new EventEmitter();
    
    SteamUserStats.prototype.release = function() {
        this.emit("finalize");
        this.releaseInstance();
    };
    SteamUserStats.prototype.invoke = function(methodBinding) {
        return interop.invoke(this.instanceId, methodBinding);
    };
    SteamUserStats.prototype.releaseInstance = function() {
        interop.releaseInstance(this.instanceId);
    };

    /**
     * Checks if the client is logged into steam
     * @returns bool
     */
    SteamUserStats.prototype.getNumberOfCurrentPlayers = function () {
        return this.invoke({
            "method": "getNumberOfCurrentPlayers"
        });
    };

    root.createSteamUserStats = function(instanceId) {
        return interop.createInstance("Steam.UserStats", SteamUserStats, instanceId);
    };
    
}(_global()));