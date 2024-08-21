/** @file steamuser.js
 *  @class Steam user
 *  @brief
 */
import {EventEmitter} from "events";

import {interop} from "../direct/Host";

class SteamUser extends EventEmitter {
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
   * Checks if the client is logged into steam
   * @returns bool
   */
  isLoggedOn() {
    return this.invoke("isLoggedOn");
  }
  /**
   * Checks if the client is behind a NAT
   * @returns bool
   */
  isBehindNAT() {
    return this.invoke("isBehindNAT");
  }
  /**
   * Checks if the user is phone verified
   * @returns bool
   */
  isPhoneVerified() {
    return this.invoke("isPhoneVerified");
  }
  /**
   * Checks if the user has two factor authentication enabled
   * @returns bool
   */
  isTwoFactorEnabled() {
    return this.invoke("isTwoFactorEnabled");
  }
  /**
   * Checks if the user's phone is identifying
   * @returns bool
   */
  isPhoneIdentifying() {
    return this.invoke("isPhoneIdentifying");
  }
  /**
   * Checks if the user's phone requires verification
   * @returns bool
   */
  isPhoneRequiringVerification() {
    return this.invoke("isPhoneRequiringVerification");
  }
  /**
   * Gets the SteamID for the current player
   * @returns integer
   */
  getSteamId() {
    return this.invoke("getSteamId");
  }
  /**
   * Gets a session ticket for the current player
   * @returns integer
   */
  getAuthSessionTicket() {
    return this.invoke("getAuthSessionTicket");
  }
  /**
   * Cancels a session ticket for the current player
   * @returns bool
   */
  cancelSessionTicket() {
    return this.invoke("cancelAuthTicket");
  }
  /**
   * Gets the user's player steam level
   * @returns bool
   */
  getPlayerSteamLevel() {
    return this.invoke("getPlayerSteamLevel");
  }
  /**
   * Starts voice recording in steam
   * @returns bool
   */
  startVoiceRecording() {
    return this.invoke("startVoiceRecording");
  }
  /**
   * Stops voice recording in steam
   * @returns bool
   */
  stopVoiceRecording() {
    return this.invoke("stopVoiceRecording");
  }
}

export const createSteamUser = (instanceId) =>
  interop.createInstance("Steam.User", SteamUser, instanceId);

/** Global instance of SteamUser
 *  @type SteamUser
 */
var steamUser;
interop.on("load", (info) => {
  if (info.name === "steam") {
    steamUser = createSteamUser();
  }
});
interop.on("unload", (info) => {
  if (info.name === "steam") {
    steamUser.release();
    steamUser = null;
  }
});

export {steamUser};
