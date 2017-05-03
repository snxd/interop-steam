### Steam Interop Example

This is an example interop that integrates with the Steamworks API.

These instructions will show you how to implement the Steam Auth Interop. 
A test harness javascript file (steamtest.js) is included. 
This file will simply alert out the variables that are produced by the interop. 
Using this example, you should be able to integrate these variables into your authentication service calls as needed.

### Contents 

* bin - Steam dlls
* lib - Steam libraries
* js/steam.js - Javascript class
* js/steamtest.js - Javascript test
* interop - DIRECT interop headers
* jansson - Jansson javascript parser
* windows - Windows project files

### Requirements

* Visual Studio 2013
* DIRECT 5

### Setup Instructions

1. Compile the solution
2. Copy the dynamic library from the target directory to the host.exe directory
3. Edit workflow.json and add the following task to be run in the load entry point: ```
    "loadSimple": {
        "type": "interopLoad",
        "name": "steam",
        "path": "{ModuleDirectory}{LibraryPrefix}steam.{LibraryExtension}"
    },```
4. Edit workflow.json and add the following task to be run in the unload entry point: ```
    "unloadSimple": {
        "type": "interopUnload",
        "name": "steam",
        "path": "{ModuleDirectory}{LibraryPrefix}steam.{LibraryExtension}"
    },```
5. Copy simpleobject.js and simpleobjecttest.js from the js directory to the skin directory
6. Open main.html and insert the following scripts after main.js: ```
    <script src="steam.js" type="text/javascript"></script>
    <script src="steamtest.js" type="text/javascript"></script>```
7. Run host.exe with --disablesecurity as the first argument (during production if you sign the dll you won't need this).
