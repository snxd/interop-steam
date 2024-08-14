/** @file steamapps.js
 *  @class SteamApps
 *  @brief
 */
function SteamApps(instanceId) {
    this.instanceId = instanceId;
}

SteamApps.prototype = new EventEmitter();

SteamApps.prototype.release = function() {
    this.emit("finalize");
    this.releaseInstance();
};
SteamApps.prototype.invoke = function(methodBinding) {
    return interop.invoke(this.instanceId, methodBinding);
};
SteamApps.prototype.releaseInstance = function() {
    interop.releaseInstance(this.instanceId);
};

/**
 * Gets whether or not an app is installed by its id
 */
SteamApps.prototype.isAppInstalled = function (id) {
    return this.invoke({
        "method": "isAppInstalled",
        "id": id
    });
};
/**
 * Gets an app's install directory
 */
SteamApps.prototype.getAppInstallDir = function (id) {
    return this.invoke({
        "method": "getAppInstallDir",
        "id": id
    });
};
/**
 * Gets the current app's build id
 */
SteamApps.prototype.getAppBuildId = function () {
    return this.invoke({
        "method": "getAppBuildId"
    });
};
/**
 * Gets whether or not a dlc is installed by its app id
 */
SteamApps.prototype.isDLCInstalled = function (id) {
    return this.invoke({
        "method": "isDLCInstalled",
        "id": id
    });
};
/**
 * Gets number of dlcs for the current app
 */
SteamApps.prototype.getDLCCount = function () {
    return this.invoke({
        "method": "getDLCCount"
    });
};
/**
 * Installs a dlc by its app id
 */
SteamApps.prototype.installDLC = function (id) {
    return this.invoke({
        "method": "installDLC",
        "id": id
    });
};
/**
 * Uninstalls a dlc by its app id
 */
SteamApps.prototype.installDLC = function (id) {
    return this.invoke({
        "method": "uninstallDLC",
        "id": id
    });
};
/**
 * Gets dlc data by index
 */
SteamApps.prototype.getDLCData = function (index) {
    return this.invoke({
        "method": "getDLCData",
        "index": index
    });
};
/**
 * Checks if the current user is banned
 */
SteamApps.prototype.isVACBanned = function () {
    return this.invoke({
        "method": "isVACBanned"
    });
};
/**
 * Checks if the current user is subscribed
 */
SteamApps.prototype.isSubscribed = function () {
    return this.invoke({
        "method": "isSubscribed"
    });
};
/**
 * Checks if the current user has low violence setting enabled
 */
SteamApps.prototype.isLowViolence = function () {
    return this.invoke({
        "method": "isLowViolence"
    });
};
/**
 * Checks if the current user has cyber cafe setting enabled
 */
SteamApps.prototype.isCybercafe = function () {
    return this.invoke({
        "method": "isCybercafe"
    });
};
/**
 * Checks if the current user is subscribed from weekend
 */
SteamApps.prototype.isSubscribedFromWeekend = function () {
    return this.invoke({
        "method": "isSubscribedFromWeekend"
    });
};
/**
 * Gets the current branch for the app
 */
SteamApps.prototype.getCurrentBetaName = function () {
    return this.invoke({
        "method": "getCurrentBetaName"
    });
};
/**
 * Marks content as corrupt
 */
SteamApps.prototype.markContentCorrupt = function (missingFilesOnly) {
    return this.invoke({
        "method": "markContentCorrupt",
        "missingFilesOnly": missingFilesOnly
    });
};

createSteamApps = function() {
    return interop.createInstance("Steam.Apps", SteamApps);
};

/** Global instance of SteamApps
 *  @type SteamApps
 */

interop.on("load", function(info) {
    if (info.name.toLowerCase() == "steam") {
        steamApps = createSteamApps();
    }
});
interop.on("unload", function(info) {
    if (info.name.toLowerCase() == "steam") {
        steamApps.release();
        steamApps = null;
    }
});
