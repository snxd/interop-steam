/** @file steamuserstats.js
 *  @class Steam user stats
 *  @brief
 */
import {EventEmitter} from "events";

import {interop} from "../direct/Host";

class SteamUserStats extends EventEmitter {
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
   * Get the number of players for the current app
   * @returns bool
   */
  getNumberOfCurrentPlayers() {
    return this.invoke("getNumberOfCurrentPlayers");
  }
  /**
   * Get the number of achievements for the current app
   * @returns bool
   */
  getNumberOfAchievements() {
    return this.invoke("getNumberOfAchievements");
  }
  /**
   * Get an achievement by name to see if it was achieved
   * @returns bool
   */
  getAchievement(name) {
    return this.invoke("getAchievement", {name});
  }
  /**
   * Get an achievement name by index
   * @returns string
   */
  getAchievementName(index) {
    return this.invoke("getAchievementName", {index});
  }
  /**
   * Get an achievement image icon index by name
   * @returns int
   */
  getAchievementIcon(name) {
    return this.invoke("getAchievementIcon", {name});
  }
  /**
   * Get an achievement's attribute (name, desc, or hidden)
   * @returns int
   */
  getAchievementDisplayAttribute(name, key) {
    return this.invoke("getAchievementDisplayAttribute", {name, key});
  }
  /**
   * Set an achievement by name as achieved
   * @returns bool
   */
  setAchievement(name) {
    return this.invoke("setAchievement", {name});
  }
  /**
   * Clears an achievement by name that was previously achieved
   * @returns bool
   */
  clearAchievement(name) {
    return this.invoke("clearAchievement", {name});
  }
  /**
   * Get an achievement by user and name to see if it was achieved
   * @returns bool
   */
  getUserAchievement(steamId, name) {
    return this.invoke("getUserAchievement", {steamId, name});
  }
  /**
   * Get percentage of users who have completed achievement
   * @returns bool
   */
  getAchievementAchievedPercent(name) {
    return this.invoke("getAchievementAchievedPercent", {name});
  }
  /**
   * Requests a user's stats
   * @returns bool
   */
  requestUserStats(steamId) {
    return this.invoke("requestUserStats", {steamId});
  }
  /**
   * Requests global achievement percentages
   * @returns bool
   */
  requestGlobalAchievementPercentages() {
    return this.invoke("requestGlobalAchievementPercentages");
  }
  /**
   * Resets all stats
   * @returns bool
   */
  resetAllStats(achievementsToo) {
    return this.invoke("resetAllStats", {achievementsToo});
  }
}

export const createSteamUserStats = (instanceId) =>
  interop.createInstance("Steam.UserStats", SteamUserStats, instanceId);

/** Global instance of SteamUserStats
 *  @type SteamUserStats
 */
var steamUserStats;
interop.on("load", (info) => {
  if (info.name === "steam") {
    steamUserStats = createSteamUserStats();
  }
});
interop.on("unload", (info) => {
  if (info.name === "steam") {
    steamUserStats.release();
    steamUserStats = null;
  }
});

export {steamUserStats};
