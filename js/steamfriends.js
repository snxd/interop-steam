/** @file steamfriends.js
 *  @class Steam friends
 *  @brief
 */
(function (root) {
    function SteamFriends(instanceId) {
        this.instanceId = instanceId;
    }

    SteamFriends.prototype = new EventEmitter();

    SteamFriends.prototype.release = function() {
        this.emit("finalize");
        this.releaseInstance();
    };
    SteamFriends.prototype.invoke = function(methodBinding) {
        return interop.invoke(this.instanceId, methodBinding);
    };
    SteamFriends.prototype.releaseInstance = function() {
        interop.releaseInstance(this.instanceId);
    };

    /**
     * Gets the number of friends
     * @returns integer
     */
    SteamFriends.prototype.getFriendCount = function () {
        return this.invoke({
            "method": "getFriendCount"
        });
    };
    /**
     * Gets the steam id of a friend by index
     * @returns integer
     */
    SteamFriends.prototype.getFriendByIndex = function (index) {
        return this.invoke({
            "method": "getFriendByIndex",
            "index": index
        });
    };
    /**
     * Gets the persona name of a friend by steam id
     * @returns integer
     */
    SteamFriends.prototype.getFriendPersonaName = function (steamId) {
        return this.invoke({
            "method": "getFriendPersonaName",
            "steamId": steamId
        });
    };
    /**
     * Gets the persona state of a friend by steam id
     * @returns string
     */
    SteamFriends.prototype.getFriendPersonaState = function (steamId) {
        return this.invoke({
            "method": "getFriendPersonaState",
            "steamId": steamId
        });
    };
    /**
     * Gets the relationship of a friend by steam id
     * @returns string
     */
    SteamFriends.prototype.getFriendRelationship = function (steamId) {
        return this.invoke({
            "method": "getFriendRelationship",
            "steamId": steamId
        });
    };

    SteamFriends.prototype.activateGameOverlayToWebPage = function (url) {
        return this.invoke({
            "method": "activateGameOverlayToWebPage",
            "url": url
        });
    };

    root.createSteamFriends = function(instanceId) {
        return interop.createInstance("Steam.Friends", SteamFriends, instanceId);
    };

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
    SteamFriendsPersonaState.prototype.nameFromId = function(id, separator) {
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
    }

    root.steamFriendsPersonaState = new SteamFriendsPersonaState();

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
    SteamFriendsRelationship.prototype.nameFromId = function(id, separator) {
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
    }

    root.steamFriendsRelationship = new SteamFriendsRelationship();

}(_global()));
