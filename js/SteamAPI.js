/** @file steamapi.js
 *  @class Steam API
 *  @brief
 */
import {EventEmitter} from "events";

import {interop} from "../direct/Host";

class SteamAPI extends EventEmitter {
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
   * Initialize steam api
   */
  initialize() {
    return this.invoke("initialize");
  }
  /**
   * Gets whether or not steam is running
   */
  isSteamRunning() {
    return this.invoke("isSteamRunning");
  }
  /**
   * Gets whether or not steam api is initialized
   */
  isInitialized() {
    return this.invoke("isInitialized");
  }
  /**
   * Sets the current app id
   */
  setAppId(id) {
    return this.invoke("setAppId", {id});
  }
  /**
   * Restarts the app if not started from steam
   */
  restartAppIfNecessary(id) {
    return this.invoke("restartAppIfNecessary", {id});
  }
}

/** Global instance of SteamAPI
 *  @type SteamAPI
 */
export const createSteamAPI = (instanceId) =>
  interop.createInstance("Steam.API", SteamAPI, instanceId);

/** Global instance of SteamAPI created when interop loaded
 *  @type SteamAPI
 */
var steamAPI;
interop.on("load", (info) => {
  if (info.name === "steam") {
    steamAPI = createSteamAPI();
  }
});
interop.on("unload", (info) => {
  if (info.name === "steam") {
    steamAPI.release();
    steamAPI = null;
  }
});

export {steamAPI};
