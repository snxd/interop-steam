(function () {
    var workflowLoaded = notificationCenter.addObserver("Workflow", "DidLoad", OnWorkflowLoaded);
    var workflowUnLoaded = notificationCenter.addObserver("Workflow", "WillUnload", OnWorkflowUnLoaded);

    function OnWorkflowLoaded() {
        window.steam = window.interop.createInstance("Steam.Auth", SteamController);

        window.configData = $.parseJSON(window.steam.getDebugData());

        if (window.configData != false) {
            window.authentication.initAccount(window.configData.user, window.configData.password, null);
        }

        workflowLoaded.release();
        workflowLoaded = null;
    }

    function OnWorkflowUnLoaded() {
        window.steam.release();

        workflowUnLoaded.release();
        workflowUnLoaded = null;
    }
}());