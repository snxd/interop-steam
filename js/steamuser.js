/** @file steamuser.js
 *  @class Steam user
 *  @brief 
 */
(function (root) {
    function SteamUser(instanceId) {
        this.instanceId = instanceId;
    }
    
    SteamUser.prototype = new EventEmitter();
    
    SteamUser.prototype.release = function() {
        this.emit("finalize");
        this.releaseInstance();
    };
    SteamUser.prototype.invoke = function(methodBinding) {
        return interop.invoke(this.instanceId, methodBinding);
    };
    SteamUser.prototype.releaseInstance = function() {
        interop.releaseInstance(this.instanceId);
    };
    
    /**
     * Gets the SteamID for the current player
     * @returns integer 
     */
    SteamUser.prototype.getSteamId = function () {
        return this.invoke({
            "method": "getSteamId"
        });
    };
    /**
     * Checks if the client is logged into steam
     * @returns bool
     */
    SteamUser.prototype.isLoggedOn = function () {
        return this.invoke({
            "method": "isLoggedOn"
        });
    };
    /**
     * Gets a session ticket for the current player
     * @returns integer 
     */
    SteamUser.prototype.getAuthSessionTicket = function () {
        return this.invoke({
            "method": "getAuthSessionTicket"
        });
    };
    /**
     * Cancels a session ticket for the current player
     * @returns bool
     */
    SteamUser.prototype.cancelSessionTicket = function () {
        return this.invoke({
            "method": "cancelAuthTicket"
        });
    };

    root.createSteamUser = function(instanceId) {
        return interop.createInstance("Steam.User", SteamUser, instanceId);
    };
    
}(_global()));