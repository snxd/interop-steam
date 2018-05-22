(function (root) {
    /** @file steamapi.js
     *  @class Steam API
     *  @brief
     */
    function SteamAPI(instanceId) {
        this.instanceId = instanceId;
    }

    SteamAPI.prototype = new EventEmitter();

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

    /** Global instance of SteamAPI
     *  @type SteamAPI
     */
    root.createSteamAPI = function() {
        return interop.createInstance("Steam.API", SteamAPI);
    };

    /** Global instance of SteamAPI created when interop loaded
     *  @type SteamAPI
     */
    interop.on("libraryLoad", function(info) {
        if (info.name.toLowerCase() == "steam") {
            root.steamAPI = root.createSteamAPI();
        }
    });
    interop.on("libraryUnload", function(info) {
        if (info.name.toLowerCase() == "steam") {
            root.steamAPI.release();
            root.steamAPI = null;
        }
    });

}(_global()));
