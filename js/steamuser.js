/** @file steamuser.js
 *  @class Steam user
 *  @brief
 */
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
 * Checks if the client is logged into steam
 * @returns bool
 */
SteamUser.prototype.isLoggedOn = function () {
    return this.invoke({
        "method": "isLoggedOn"
    });
};
/**
 * Checks if the client is behind a NAT
 * @returns bool
 */
SteamUser.prototype.isBehindNAT = function () {
    return this.invoke({
        "method": "isBehindNAT"
    });
};
/**
 * Checks if the user is phone verified
 * @returns bool
 */
SteamUser.prototype.isPhoneVerified = function () {
    return this.invoke({
        "method": "isPhoneVerified"
    });
};
/**
 * Checks if the user has two factor authentication enabled
 * @returns bool
 */
SteamUser.prototype.isTwoFactorEnabled = function () {
    return this.invoke({
        "method": "isTwoFactorEnabled"
    });
};
/**
 * Checks if the user's phone is identifying
 * @returns bool
 */
SteamUser.prototype.isPhoneIdentifying = function () {
    return this.invoke({
        "method": "isPhoneIdentifying"
    });
};
/**
 * Checks if the user's phone requires verification
 * @returns bool
 */
SteamUser.prototype.isPhoneRequiringVerification = function () {
    return this.invoke({
        "method": "isPhoneRequiringVerification"
    });
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
/**
 * Gets the user's player steam level
 * @returns bool
 */
SteamUser.prototype.getPlayerSteamLevel = function () {
    return this.invoke({
        "method": "getPlayerSteamLevel"
    });
};
/**
 * Starts voice recording in steam
 * @returns bool
 */
SteamUser.prototype.startVoiceRecording = function () {
    return this.invoke({
        "method": "startVoiceRecording"
    });
};
/**
 * Stops voice recording in steam
 * @returns bool
 */
SteamUser.prototype.stopVoiceRecording = function () {
    return this.invoke({
        "method": "stopVoiceRecording"
    });
};

createSteamUser = function(instanceId) {
    return interop.createInstance("Steam.User", SteamUser, instanceId);
};

/** Global instance of SteamUser
 *  @type SteamUser
 */

interop.on("load", function(info) {
    if (info.name.toLowerCase() == "steam") {
        steamUser = createSteamUser();
    }
});
interop.on("unload", function(info) {
    if (info.name.toLowerCase() == "steam") {
        steamUser.release();
        steamUser = null;
    }
});
