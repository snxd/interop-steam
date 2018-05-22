## Steam Interop Example for DIRECT 5.1.9

The steam interop example shows how to get information from the Steamworks API. It retrieves information such as whether or not the user is logged on, what their steam ID is, and what their session token is. The Javascript classes provided mirror the classes that the Steamworks API provides.

### Contents

* bin - Steam dlls
* lib - Steam libraries
* steam - Steam headers
* js - Javascript classes
* interop - DIRECT interop headers
* windows - Windows project files

### Requirements

* Visual Studio 2013
* DIRECT 5.1.9+
* Steam (must be running)

### Setup Instructions

1. Compile the solution
2. Copy the dynamic library from the target directory to the host.exe directory
3. Edit workflow.json and add the following task to be run in the load entry point: ```
    "loadSteam": {
        "type": "interopLoad",
        "name": "steam",
        "path": "{ModuleDirectory}{LibraryPrefix}steam.{LibraryExtension}"
    },```
4. Edit workflow.json and add the following task to be run in the unload entry point: ```
    "unloadSteam": {
        "type": "interopUnload",
        "name": "steam",
        "path": "{ModuleDirectory}{LibraryPrefix}steam.{LibraryExtension}"
    },```
5. Copy the files from the js directory to the skin directory
6. Open main.html and insert the following scripts after main.js: ```
    <script src="steamapi.js" type="text/javascript"></script>
    <script src="steamapps.js" type="text/javascript"></script>
    <script src="steamfriends.js" type="text/javascript"></script>
    <script src="steamuser.js" type="text/javascript"></script>
    <script src="steamuserstats.js" type="text/javascript"></script>
    <script src="steamutils.js" type="text/javascript"></script>
    <script src="steamtest.js" type="text/javascript"></script>```
7. Modify app.json to set cefEnabled to true
8. Modify main.json to set enableDeveloperTools to true for cef
9. Run host.exe with --disablesecurity as the first argument (during production if you sign the dll you won't need this).

### Screenshots

In the dev tools you can see the information being retrieved from the Steam client using the Steamworks API. The source code that prints all the details to the console can be found in _steamtest.js_.

![image](https://gitlab.com/snxd/interop/steam/uploads/08224347b79a0a6f5d5e109ce2b102e1/image.png)
