/** @file steamfriends.js
 *  @class Steam friends
 *  @brief
 */
import {EventEmitter} from "events";

import {interop} from "../direct/Host";

class SteamFriends extends EventEmitter {
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
   * Gets the number of friends
   * @returns integer
   */
  getFriendCount() {
    return this.invoke("getFriendCount");
  }
  /**
   * Gets the steam id of a friend by index
   * @returns integer
   */
  getFriendByIndex(index) {
    return this.invoke("getFriendByIndex", {index});
  }
  /**
   * Gets the persona name of a friend by steam id
   * @returns integer
   */
  getFriendPersonaName(steamId) {
    return this.invoke("getFriendPersonaName", {steamId});
  }
  /**
   * Gets the persona state of a friend by steam id
   * @returns string
   */
  getFriendPersonaState(steamId) {
    return this.invoke("getFriendPersonaState", {steamId});
  }
  /**
   * Gets the relationship of a friend by steam id
   * @returns string
   */
  getFriendRelationship(steamId) {
    return this.invoke("getFriendRelationship", {steamId});
  }
  /**
   * Gets the image index of the small friend avatar
   * @returns string
   */
  getSmallFriendAvatar(steamId) {
    return this.invoke("getSmallFriendAvatar", {steamId});
  }
  /**
   * Gets the image index of the medium friend avatar
   * @returns string
   */
  getMediumFriendAvatar(steamId) {
    return this.invoke("getMediumFriendAvatar", {steamId});
  }
  /**
   * Gets the image index of the large friend avatar
   * @returns string
   */
  getLargeFriendAvatar(steamId) {
    return this.invoke("getLargeFriendAvatar", {steamId});
  }

  activateGameOverlayToWebPage(url) {
    return this.invoke("activateGameOverlayToWebPage", {url});
  }
}

export const createSteamFriends = (instanceId) =>
  interop.createInstance("Steam.Friends", SteamFriends, instanceId);


/** Global instance of SteamFriends
 *  @type SteamFriends
 */
var steamFriends;
interop.on("load", (info) => {
  if (info.name === "steam") {
    steamFriends = createSteamFriends();
  }
});
interop.on("unload", (info) => {
  if (info.name === "steam") {
    steamFriends.release();
    steamFriends = null;
  }
});

/** @class SteamFriendsPersonaState
 *  @brief persona state
 */
function SteamFriendsPersonaState() {
  /**
   * Offline
   * @type int
   */
  this.OFFLINE = 0;
  /**
   * Online
   * @type int
   */
  this.ONLINE = 1;
  /**
   * Busy
   * @type int
   */
  this.BUSY = 2;
  /**
   * Away
   * @type int
   */
  this.AWAY = 3;
  /**
     * Snooze
     * @type int
     */
  this.SNOOZE = 4;
  /**
   * Looking To Trade
   * @type int
   */
  this.LOOKINGTOTRADE = 5;
  /**
   * Looking To Play
   * @type int
   */
  this.LOOKINGTOPLAY = 6;
}

/** Converts a value to a string
 *  @tparam int
 *  @type string
 *  @returns stringified name of persona state
 */
SteamFriendsPersonaState.prototype.nameFromId = (id) => {
  var nameMap = [
    "Offline",
    "Online",
    "Busy",
    "Away",
    "Snooze",
    "LookingToTrade",
    "LookingToPlay"
  ];
  if (nameMap.length > id) {
    return nameMap[id];
  }
  return id.toString();
};

var steamFriendsPersonaState = new SteamFriendsPersonaState();

/** @class SteamFriendsRelationship
 *  @brief relationships
 */
function SteamFriendsRelationship() {
  /**
   * None
   * @type int
   */
  this.NONE = 0;
  /**
   * Blocked
   * @type int
   */
  this.BLOCKED = 1;
  /**
   * Request Recipient
   * @type int
   */
  this.REQUESTRECIPIENT = 2;
  /**
   * Friend
   * @type int
   */
  this.FRIEND = 3;
  /**
   * Request Initiator
   * @type int
   */
  this.REQUESTINITIATOR = 4;
  /**
   * Ignored
   * @type int
   */
  this.IGNORED = 5;
  /**
   * Ignored Friend
   * @type int
   */
  this.IGNOREDFRIEND = 6;
  /**
   * Suggested
   * @type int
   */
  this.SUGGESTED = 7;
}

/** Converts a value to a string
 *  @tparam int
 *  @type string
 *  @returns stringified name of relationship
 */
SteamFriendsRelationship.prototype.nameFromId = (id) => {
  var nameMap = [
    "None",
    "Blocked",
    "RequestRecipient",
    "Friend",
    "RequestInitiator",
    "Ignored",
    "IgnoredFriend",
    "Suggested"
  ];
  if (nameMap.length > id) {
    return nameMap[id];
  }
  return id.toString();
};

var steamFriendsRelationship = new SteamFriendsRelationship();

export {steamFriends, steamFriendsRelationship, steamFriendsPersonaState};
