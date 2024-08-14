/** @file steamutils.js
 *  @class Steam utils
 *  @brief
 */
function SteamUtils(instanceId) {
    this.instanceId = instanceId;
}

SteamUtils.prototype = new EventEmitter();

SteamUtils.prototype.release = function() {
    this.emit("finalize");
    this.releaseInstance();
};
SteamUtils.prototype.invoke = function(methodBinding) {
    return interop.invoke(this.instanceId, methodBinding);
};
SteamUtils.prototype.releaseInstance = function() {
    interop.releaseInstance(this.instanceId);
};

/**
 * Gets if steam overlay is enabled
 * @returns bool
 */
SteamUtils.prototype.isOverlayEnabled = function () {
    return this.invoke({
        "method": "isOverlayEnabled"
    });
};
/**
 * Gets if steam is running in VR mode
 * @returns bool
 */
SteamUtils.prototype.isSteamRunningInVR = function () {
    return this.invoke({
        "method": "isSteamRunningInVR"
    });
};
/**
 * Gets if steam is running in big picture mode
 * @returns bool
 */
SteamUtils.prototype.isSteamInBigPictureMode = function () {
    return this.invoke({
        "method": "isSteamInBigPictureMode"
    });
};
/**
 * Gets if steam VR headset has streaming enabled
 * @returns bool
 */
SteamUtils.prototype.isVRHeadsetStreamingEnabled = function () {
    return this.invoke({
        "method": "isVRHeadsetStreamingEnabled"
    });
};
/**
 * Gets the steam server unix time
 * @returns int
 */
SteamUtils.prototype.getServerRealTime = function () {
    return this.invoke({
        "method": "getServerRealTime"
    });
};
/**
 * Gets the IP country for the current user
 * @returns string
 */
SteamUtils.prototype.getIPCountry = function () {
    return this.invoke({
        "method": "getIPCountry"
    });
};
/**
 * Gets the current battery power level (0-255)
 * @returns int
 */
SteamUtils.prototype.getCurrentBatteryPower = function () {
    return this.invoke({
        "method": "getCurrentBatteryPower"
    });
};
/**
 * Gets the app id for the current process
 * @returns int
 */
SteamUtils.prototype.getAppID = function () {
    return this.invoke({
        "method": "getAppID"
    });
};
/**
 * Gets the image's width
 * @returns int
 */
SteamUtils.prototype.getImageWidth = function (index) {
    return this.invoke({
        "method": "getImageWidth",
        "index": index
    });
};
/**
 * Gets the image's height
 * @returns int
 */
SteamUtils.prototype.getImageHeight = function (index) {
    return this.invoke({
        "method": "getImageHeight",
        "index": index
    });
};
/**
 * Gets the base64 rgba buffer
 * @returns string
 */
SteamUtils.prototype.getImageRGBA = function (index) {
    return this.invoke({
        "method": "getImageRGBA",
        "index": index
    });
};
/**
 * Starts the VR dashboard for steam
 * @returns int
 */
SteamUtils.prototype.startVRDashboard = function (index) {
    return this.invoke({
        "method": "startVRDashboard"
    });
};

createSteamUtils = function(instanceId) {
    return interop.createInstance("Steam.Utils", SteamUtils, instanceId);
};

/** Global instance of SteamUtils
     *  @type SteamUtils
     */

interop.on("load", function(info) {
    if (info.name.toLowerCase() == "steam") {
        steamUtils = createSteamUtils();
    }
});
interop.on("unload", function(info) {
    if (info.name.toLowerCase() == "steam") {
        steamUtils.release();
        steamUtils = null;
    }
});
