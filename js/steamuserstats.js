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
            "method": "getAchievement",
            "name": name
        });
    };
    /**
     * Get an achievement name by index
     * @returns string
     */
    SteamUserStats.prototype.getAchievementName = function (index) {
        return this.invoke({
            "method": "getAchievementName",
            "index": index
        });
    };
    /**
     * Get an achievement image icon index by name
     * @returns int
     */
    SteamUserStats.prototype.getAchievementIcon = function (name) {
        return this.invoke({
            "method": "getAchievementIcon",
            "name": name
        });
    };
    /**
     * Get an achievement's attribute (name, desc, or hidden)
     * @returns int
     */
    SteamUserStats.prototype.getAchievementDisplayAttribute = function (name, key) {
        return this.invoke({
            "method": "getAchievementDisplayAttribute",
            "name": name,
            "key": key
        });
    };
    /**
     * Set an achievement by name as achieved
     * @returns bool
     */
    SteamUserStats.prototype.setAchievement = function (name) {
        return this.invoke({
            "method": "setAchievement",
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
    /**
     * Get an achievement by user and name to see if it was achieved
     * @returns bool
     */
    SteamUserStats.prototype.getUserAchievement = function (steamId, name) {
        return this.invoke({
            "method": "getUserAchievement",
            "steamId": steamId,
            "name": name
        });
    };
    /**
     * Get percentage of users who have completed achievement
     * @returns bool
     */
    SteamUserStats.prototype.getAchievementAchievedPercent = function (name) {
        return this.invoke({
            "method": "getAchievementAchievedPercent",
            "name": name
        });
    };
    /**
     * Requests a user's stats
     * @returns bool
     */
    SteamUserStats.prototype.requestUserStats = function (steamId) {
        return this.invoke({
            "method": "requestUserStats",
            "steamId": steamId
        });
    };
    /**
     * Requests global achievement percentages
     * @returns bool
     */
    SteamUserStats.prototype.requestGlobalAchievementPercentages = function () {
        return this.invoke({
            "method": "requestGlobalAchievementPercentages"
        });
    };
    /**
     * Resets all stats
     * @returns bool
     */
    SteamUserStats.prototype.resetAllStats = function (achievementsToo) {
        return this.invoke({
            "method": "resetAllStats",
            "achievementsToo": achievementsToo
        });
    };

    root.createSteamUserStats = function(instanceId) {
        return interop.createInstance("Steam.UserStats", SteamUserStats, instanceId);
    };

    /** Global instance of SteamUserStats
     *  @type SteamUserStats
     */

    interop.on("libraryLoad", function(info) {
        if (info.name.toLowerCase() == "steam") {
            root.steamUserStats = root.createSteamUserStats();
        }
    });
    interop.on("libraryUnload", function(info) {
        if (info.name.toLowerCase() == "steam") {
            root.steamUserStats.release();
            root.steamUserStats = null;
        }
    });

}(_global()));
