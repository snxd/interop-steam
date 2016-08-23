// Remember to add steamauth.js to your HTML scripts

notificationCenter.addObserver("Workflow", "DidLoad", function() {
	
	window.steam.setSteamAppId("440"); // do this ONCE per application run, and BEFORE any other calls

	var isLoggedIn=window.steam.isLoggedOn();
	var steamId=window.steam.getSteamId();
	var ticket=window.steam.getSessionTicket();
	var resp = "Logged In: "+isLoggedIn+", Steam Id: "+steamId+", Ticket #" + ticket;
	window.console.info(resp);
	alert(resp);
});
