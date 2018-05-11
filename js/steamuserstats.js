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
     * Get the number of players for the current app
     * @returns bool
     */
    SteamUserStats.prototype.getNumberOfCurrentPlayers = function () {
        return this.invoke({
            "method": "getNumberOfCurrentPlayers"
        });
    };
    /**
     * Get the number of achievements for the current app
     * @returns bool
     */
    SteamUserStats.prototype.getNumberOfAchievements = function () {
        return this.invoke({
            "method": "getNumberOfAchievements"
        });
    };
    /**
     * Get an achievement by name to see if it was achieved
     * @returns bool
     */
    SteamUserStats.prototype.getAchievement = function (name) {
        return this.invoke({
            "method": "getArchievement",
            "name": name
        });
    };
    /**
     * Set an achievement by name as achieved
     * @returns bool
     */
    SteamUserStats.prototype.setAchievement = function (name) {
        return this.invoke({
            "method": "setArchievement",
            "name": name
        });
    };
    /**
     * Clears an achievement by name that was previously achieved
     * @returns bool
     */
    SteamUserStats.prototype.clearAchievement = function (name) {
        return this.invoke({
            "method": "clearAchievement",
            "name": name
        });
    };


    root.createSteamUserStats = function(instanceId) {
        return interop.createInstance("Steam.UserStats", SteamUserStats, instanceId);
    };

}(_global()));
