/** @file steamuserstats.js
 *  @class Steam user stats
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

function SteamUserStats(instanceId) {
  this.instanceId = instanceId;
}

SteamUserStats.prototype = Object.create(EventEmitter.prototype, {
  constructor: {value: SteamUserStats, enumerable: false, writable: true, configurable: true}
});

SteamUserStats.prototype.release = function() {
  this.emit("finalize");
  this.releaseInstance();
};
SteamUserStats.prototype.invoke = function(methodBinding) {
  return interop.invoke(this.instanceId, methodBinding);
};
SteamUserStats.prototype.releaseInstance = function() {
  interop.releaseInstance(this.instanceId);
};

/**
 * Get the number of players for the current app
 * @returns bool
 */
SteamUserStats.prototype.getNumberOfCurrentPlayers = function () {
  return this.invoke({
    "method": "getNumberOfCurrentPlayers"
  });
};
/**
 * Get the number of achievements for the current app
 * @returns bool
 */
SteamUserStats.prototype.getNumberOfAchievements = function () {
  return this.invoke({
    "method": "getNumberOfAchievements"
  });
};
/**
 * Get an achievement by name to see if it was achieved
 * @returns bool
 */
SteamUserStats.prototype.getAchievement = function (name) {
  return this.invoke({
    "method": "getAchievement",
    "name": name
  });
};
/**
 * Get an achievement name by index
 * @returns string
 */
SteamUserStats.prototype.getAchievementName = function (index) {
  return this.invoke({
    "method": "getAchievementName",
    "index": index
  });
};
/**
 * Get an achievement image icon index by name
 * @returns int
 */
SteamUserStats.prototype.getAchievementIcon = function (name) {
  return this.invoke({
    "method": "getAchievementIcon",
    "name": name
  });
};
/**
 * Get an achievement's attribute (name, desc, or hidden)
 * @returns int
 */
SteamUserStats.prototype.getAchievementDisplayAttribute = function (name, key) {
  return this.invoke({
    "method": "getAchievementDisplayAttribute",
    "name": name,
    "key": key
  });
};
/**
 * Set an achievement by name as achieved
 * @returns bool
 */
SteamUserStats.prototype.setAchievement = function (name) {
  return this.invoke({
    "method": "setAchievement",
    "name": name
  });
};
/**
 * Clears an achievement by name that was previously achieved
 * @returns bool
 */
SteamUserStats.prototype.clearAchievement = function (name) {
  return this.invoke({
    "method": "clearAchievement",
    "name": name
  });
};
/**
 * Get an achievement by user and name to see if it was achieved
 * @returns bool
 */
SteamUserStats.prototype.getUserAchievement = function (steamId, name) {
  return this.invoke({
    "method": "getUserAchievement",
    "steamId": steamId,
    "name": name
  });
};
/**
 * Get percentage of users who have completed achievement
 * @returns bool
 */
SteamUserStats.prototype.getAchievementAchievedPercent = function (name) {
  return this.invoke({
    "method": "getAchievementAchievedPercent",
    "name": name
  });
};
/**
 * Requests a user's stats
 * @returns bool
 */
SteamUserStats.prototype.requestUserStats = function (steamId) {
  return this.invoke({
    "method": "requestUserStats",
    "steamId": steamId
  });
};
/**
 * Requests global achievement percentages
 * @returns bool
 */
SteamUserStats.prototype.requestGlobalAchievementPercentages = function () {
  return this.invoke({
    "method": "requestGlobalAchievementPercentages"
  });
};
/**
 * Resets all stats
 * @returns bool
 */
SteamUserStats.prototype.resetAllStats = function (achievementsToo) {
  return this.invoke({
    "method": "resetAllStats",
    "achievementsToo": achievementsToo
  });
};

var createSteamUserStats = function(instanceId) {
  return interop.createInstance("Steam.UserStats", SteamUserStats, instanceId);
};

/** Global instance of SteamUserStats
 *  @type SteamUserStats
 */
var steamUserStats;
interop.on("load", function(info) {
  if (info.name === "steam") {
    steamUserStats = createSteamUserStats();
  }
});
interop.on("unload", function(info) {
  if (info.name === "steam") {
    steamUserStats.release();
    steamUserStats = null;
  }
});

export {createSteamUserStats, steamUserStats};