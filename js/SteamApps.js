/** @file steamapps.js
 *  @class SteamApps
 *  @brief
 */
import {EventEmitter} from "events";

import {interop} from "../direct/Host";

class SteamApps extends EventEmitter {
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
   * Gets whether or not an app is installed by its id
   */
  isAppInstalled(id) {
    return this.invoke("isAppInstalled", {id});
  }
  /**
   * Gets an app's install directory
   */
  getAppInstallDir(id) {
    return this.invoke("getAppInstallDir", {id});
  }
  /**
   * Gets the current app's build id
   */
  getAppBuildId() {
    return this.invoke("getAppBuildId");
  }
  /**
   * Gets whether or not a dlc is installed by its app id
   */
  isDLCInstalled(id) {
    return this.invoke("isDLCInstalled", {id});
  }
  /**
   * Gets number of dlcs for the current app
   */
  getDLCCount() {
    return this.invoke("getDLCCount");
  }
  /**
   * Installs a dlc by its app id
   */
  installDLC(id) {
    return this.invoke("installDLC", {id});
  }
  /**
   * Uninstalls a dlc by its app id
   */
  uninstallDLC(id) {
    return this.invoke("uninstallDLC", {id});
  }
  /**
   * Gets dlc data by index
   */
  getDLCData(index) {
    return this.invoke("getDLCData", {index});
  }
  /**
   * Checks if the current user is banned
   */
  isVACBanned() {
    return this.invoke("isVACBanned");
  }
  /**
   * Checks if the current user is subscribed
   */
  isSubscribed() {
    return this.invoke("isSubscribed");
  }
  /**
   * Checks if the current user has low violence setting enabled
   */
  isLowViolence() {
    return this.invoke("isLowViolence");
  }
  /**
   * Checks if the current user has cyber cafe setting enabled
   */
  isCybercafe() {
    return this.invoke("isCybercafe");
  }
  /**
   * Checks if the current user is subscribed from weekend
   */
  isSubscribedFromWeekend() {
    return this.invoke("isSubscribedFromWeekend");
  }
  /**
   * Gets the current branch for the app
   */
  getCurrentBetaName() {
    return this.invoke("getCurrentBetaName");
  }
  /**
   * Marks content as corrupt
   */
  markContentCorrupt(missingFilesOnly) {
    return this.invoke("markContentCorrupt", {missingFilesOnly});
  }
}

export const createSteamApps = (instanceId) =>
  interop.createInstance("Steam.Apps", SteamApps, instanceId);

/** Global instance of SteamApps
 *  @type SteamApps
 */
var steamApps;
interop.on("load", (info) => {
  if (info.name === "steam") {
    steamApps = createSteamApps();
  }
});
interop.on("unload", (info) => {
  if (info.name === "steam") {
    steamApps.release();
    steamApps = null;
  }
});

export {steamApps};
