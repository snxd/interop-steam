(function (root) {
    /** @file steamapp.js
     *  @class Steam app
     *  @brief 
     */
    function SteamApp(instanceId) {
        this.instanceId = instanceId;
    }

    SteamApp.prototype = new EventEmitter();
    
    SteamApp.prototype.release = function() {
        this.emit("finalize");
        this.releaseInstance();
    };
    SteamApp.prototype.invoke = function(methodBinding) {
        return interop.invoke(this.instanceId, methodBinding);
    };
    SteamApp.prototype.releaseInstance = function() {
        interop.releaseInstance(this.instanceId);
    };

    /**
     * Sets the current app id
     */
    SteamApp.prototype.setId = function (id) {
        return this.invoke({
            "method": "setId",
            "value": id
        });
    };
    
    /** Global instance of SteamApp
     *  @type SteamApp
     */
    root.createSteamApp = function() {
        return interop.createInstance("Steam.App", SteamApp);
    };
    
    /** Global instance of SteamApp created when interop loaded
     *  @type SteamApp
     */
    interop.on("libraryLoad", function(info) {
        if (info.name.toLowerCase() == "steam") {
            root.steamApp = createSteamApp();
        }
    });
    interop.on("libraryUnload", function(info) {
        if (info.name.toLowerCase() == "steam") {
            root.steamApp.release();
            root.steamApp = null;
        }
    });
    
}(_global()));