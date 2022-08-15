## Steam Interop Example for DIRECT 7.0

The steam interop example shows how to get information from the Steamworks API. It retrieves information such as whether or not the user is logged on, what their steam ID is, and what their session token is. The Javascript classes provided mirror the classes that the Steamworks API provides.

### Contents

* steamworks - Steamworks libraries and headers
* js - Javascript classes
* interop - DIRECT interop headers
* mac - macOS specific files
* linux - Linux specific files
* windows - Windows specific files

### Requirements

* CMake 3.0

### Build Instructions

Fetch submodules using Git:

```bash
git submodule update --init
```

CMake is a makefile generator that produces solutions and project files for various compiler toolkits. To generate the solution files for your environment and to build the project:

```bash
cmake -S . -B build
cmake --build build --config Debug
```
### Setup Instructions

First copy the interop the SteamWorks shared libraries to the directory where you intended to load it from. For our example, we use the same directory as the client's executable: `{moduleAppDirectory}`

### JavaScript Integration

If you intend to use the interop with-in JavaScript use the following steps:

1. Copy all of the javascript source files from the `js` directory to a directory in your skin called `src/interop`.
2. Load the example code and interop using `app.loadInterop`:
    ```js
    require("./interop/SteamExample");

    const steamPath = app.expandString("{moduleAppDirectory}{libraryPrefix}steam.{libraryExtension}");
    app.loadInterop("steam", steamPath);
    ```
3. Modify app.yaml and set `cef.enabled: true`
4. Add the following html element which will display the friends list:
    ```html
    <canvas width="300" height="600" id="friendsList"/>
    ```
5. Run `host` application with `--devtools --disablesecurity` (during production if you sign the dll you won't need this).
6. If it integrated properly, you will see the following lines in console window. <br/><img src="screenshots/console-ok.png"/><br/>And you will see the Steam friends list populated. <br/><img src="screenshots/friendslist2.png"/> <br/>If you do not have steam running you will see a warning in the console window. <br/><img src="screenshots/console-fail.png"/>
