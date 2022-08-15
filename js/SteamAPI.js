/** @file steamapi.js
 *  @class Steam API
 *  @brief
 */

import {interop} from "../direct/Host";

// Node JS support
var EventEmitter;
if (typeof (require) !== "undefined") {
  if (typeof (EventEmitter) == "undefined") {
    EventEmitter = require("events");
  }
}

function SteamAPI(instanceId) {
  this.instanceId = instanceId;
}

SteamAPI.prototype = Object.create(EventEmitter.prototype, {
  constructor: {value: SteamAPI, enumerable: false, writable: true, configurable: true}
});

SteamAPI.prototype.release = function() {
  this.emit("finalize");
  this.releaseInstance();
};
SteamAPI.prototype.invoke = function(methodBinding) {
  return interop.invoke(this.instanceId, methodBinding);
};
SteamAPI.prototype.releaseInstance = function() {
  interop.releaseInstance(this.instanceId);
};

/**
 * Initialize steam api
 */
SteamAPI.prototype.initialize = function () {
  return this.invoke({
    "method": "initialize"
  });
};
/**
 * Gets whether or not steam is running
 */
SteamAPI.prototype.isSteamRunning = function () {
  return this.invoke({
    "method": "isSteamRunning"
  });
};
/**
 * Gets whether or not steam api is initialized
 */
SteamAPI.prototype.isInitialized = function () {
  return this.invoke({
    "method": "isInitialized"
  });
};
/**
 * Sets the current app id
 */
SteamAPI.prototype.setAppId = function (id) {
  return this.invoke({
    "method": "setAppId",
    "id": id
  });
};
/**
 * Restarts the app if not started from steam
 */
SteamAPI.prototype.restartAppIfNecessary = function (id) {
  return this.invoke({
    "method": "restartAppIfNecessary",
    "id": id
  });
};

/** Global instance of SteamAPI
 *  @type SteamAPI
 */
var createSteamAPI = function() {
  return interop.createInstance("Steam.API", SteamAPI);
};

/** Global instance of SteamAPI created when interop loaded
 *  @type SteamAPI
 */
var steamAPI;
interop.on("load", function(info) {
  if (info.name === "steam") {
    steamAPI = createSteamAPI();
  }
});
interop.on("unload", function(info) {
  if (info.name === "steam") {
    steamAPI.release();
    steamAPI = null;
  }
});

export{createSteamAPI, steamAPI};
