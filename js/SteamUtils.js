/** @file steamutils.js
 *  @class Steam utils
 *  @brief
 */
import {EventEmitter} from "events";

import {interop} from "../direct/Host";

class SteamUtils extends EventEmitter {
  constructor(instanceId) {
    super();
    this.instanceId = instanceId;
    this.refCount = 1;
  }

  addRef() {
    this.refCount++;
  }
  release() {
    if (--this.refCount === 0) {
      this.emit("release");

      return interop.releaseInstance(this.instanceId);
    }
  }
  invoke(method, methodArgs) {
    return interop.invoke(this.instanceId, method, methodArgs);
  }
  /**
   * Gets if steam overlay is enabled
   * @returns bool
   */
  isOverlayEnabled() {
    return this.invoke("isOverlayEnabled");
  }
  /**
   * Gets if steam is running in VR mode
   * @returns bool
   */
  isSteamRunningInVR() {
    return this.invoke("isSteamRunningInVR");
  }
  /**
   * Gets if steam is running in big picture mode
   * @returns bool
   */
  isSteamInBigPictureMode() {
    return this.invoke("isSteamInBigPictureMode");
  }
  /**
   * Gets if steam VR headset has streaming enabled
   * @returns bool
   */
  isVRHeadsetStreamingEnabled() {
    return this.invoke("isVRHeadsetStreamingEnabled");
  }
  /**
   * Gets the steam server unix time
   * @returns int
   */
  getServerRealTime() {
    return this.invoke("getServerRealTime");
  }
  /**
   * Gets the IP country for the current user
   * @returns string
   */
  getIPCountry() {
    return this.invoke("getIPCountry");
  }
  /**
   * Gets the current battery power level (0-255)
   * @returns int
   */
  getCurrentBatteryPower() {
    return this.invoke("getCurrentBatteryPower");
  }
  /**
   * Gets the app id for the current process
   * @returns int
   */
  getAppID() {
    return this.invoke("getAppID");
  }
  /**
   * Gets the image's width
   * @returns int
   */
  getImageWidth(index) {
    return this.invoke("getImageWidth", {index});
  }
  /**
   * Gets the image's height
   * @returns int
   */
  getImageHeight(index) {
    return this.invoke("getImageHeight", {index});
  }
  /**
   * Gets the base64 rgba buffer
   * @returns string
   */
  getImageRGBA(index) {
    return this.invoke("getImageRGBA", {index});
  }
  /**
   * Starts the VR dashboard for steam
   * @returns int
   */
  startVRDashboard() {
    return this.invoke("startVRDashboard");
  }
}

export const createSteamUtils = (instanceId) =>
  interop.createInstance("Steam.Utils", SteamUtils, instanceId);

/** Global instance of SteamUtils
 *  @type SteamUtils
 */
var steamUtils;
interop.on("load", (info) => {
  if (info.name === "steam") {
    steamUtils = createSteamUtils();
  }
});
interop.on("unload", (info) =>{
  if (info.name === "steam") {
    steamUtils.release();
    steamUtils = null;
  }
});

export {steamUtils};
