/** @file steamfriends.js
 *  @class Steam friends
 *  @brief 
 */
(function (root) {
    function SteamFriends(instanceId) {
        this.instanceId = instanceId;
    }
    
    SteamFriends.prototype = new EventEmitter();
    
    SteamFriends.prototype.release = function() {
        this.emit("finalize");
        this.releaseInstance();
    };
    SteamFriends.prototype.invoke = function(methodBinding) {
        return interop.invoke(this.instanceId, methodBinding);
    };
    SteamFriends.prototype.releaseInstance = function() {
        interop.releaseInstance(this.instanceId);
    };

    /**
     * Gets the number of friends
     * @returns integer
     */
    SteamFriends.prototype.getFriendCount = function () {
        return this.invoke({
            "method": "getFriendCount"
        });
    };
    /**
     * Gets the steam id of a friend by index
     * @returns integer
     */
    SteamFriends.prototype.getFriendByIndex = function (index) {
        return this.invoke({
            "method": "getFriendByIndex",
            "index": index
        });
    };
    /**
     * Gets the persona name of a friend by steam id 
     * @returns integer
     */
    SteamFriends.prototype.getFriendPersonaName = function (steamId) {
        return this.invoke({
            "method": "getFriendPersonaName",
            "steamId": steamId
        });
    };
    SteamFriends.prototype.activateGameOverlayToWebPage = function (url) {
        return this.invoke({
            "method": "activateGameOverlayToWebPage",
            "url": url
        });
    };
    
    root.createSteamFriends = function(instanceId) {
        return interop.createInstance("Steam.Friends", SteamFriends, instanceId);
    };
    
}(_global()));