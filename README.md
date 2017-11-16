## Steam Interop Example for DIRECT 5.1.9

This is an example interop that integrates with the Steamworks API.

### Contents 

* bin - Steam dlls
* lib - Steam libraries
* steam - Steam headers
* js - Javascript classes
* interop - DIRECT interop headers
* windows - Windows project files

### Requirements

* Visual Studio 2013
* DIRECT 5.1.9

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
5. Copy simpleobject.js and simpleobjecttest.js from the js directory to the skin directory
6. Open main.html and insert the following scripts after main.js: ```
    <script src="steamapp.js" type="text/javascript"></script>
    <script src="steamfriends.js" type="text/javascript"></script>
    <script src="steamuser.js" type="text/javascript"></script>
    <script src="steamuserstats.js" type="text/javascript"></script>
    <script src="steamtest.js" type="text/javascript"></script>```
7. Run host.exe with --disablesecurity as the first argument (during production if you sign the dll you won't need this).
