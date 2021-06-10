## Using this application

The application can be accessed from <https://au.gmented.com/app/OGCT17>. Alternately, checkout the code, serve it from your webserver of choice, and load `index.html` in your browser.

The HTML on the page is very simple, with just a status prompt and button to enter the immersive environment, plus an indication of whether native WebXR is in use or the so-called "polyfill" (which emulates WebXR on WebVR-only or non-XR browsers), and the last modified date of the HTMLfile.

### The immersive app.

When complete, the application will present an interactive geo-data exploration environment.

At present however, the app just loads a static terrain via a DEM (digital elevation model) stored statically in a GeoTIFF file. This is specified by a local configuration file stored as a .json file located in the `data` subdirectory.

Once the HTML page has loaded, the WebAssembly module will be loaded, and provided the user has WebXR available, the button will display either "Enter AR" or "Enter VR", depending on device capabilities. Clicking the button will enter immersive mode.

In immersive mode, the geo-data will be visible in front of the user. If using a head-mounted device with a controller, pointing the controller will display a ray which acts as a pointer for selection and action. On handheld devices, touching the screen will have the same effect. You should be able to walk up to and around the geo-data model to inspect it.

To exit and return to the web page, use the normal control on your device. E.g. controller button, home button, or similar.

## How it works

The `index.html` file contains the page layout, as well as application settings, the WebAssembly loader, WebXR setup, and the main run loop. It is well commented, so looking through it should be instructive.

This is only one possible configuration; equally, the main run loop could have been in the WebAssembly module (as if calling `main()` in a C program). The choice to keep the main loop in Javascript in this application was made to assist understanding of the interaction between WebXR and OpenSceneGraph.

### Native code

Native code used in the project is contained in a set of C/C++ files and libraries. These are compiled ahead-of-time to a WebAssembly module via the [`build.sh`](build.sh) build script.

OpenSceneGraph (OSG) provides the core model loading and rendering in the application, including the construction of a terrain model from DEM data, plus the laser pointers, hit-testing, and rendering of label text. We use a version of OpenSceneGraph which has been pre-compiled to WebAssembly.

To connect the Javascript portion of the app to the compiled OSG WebAssembly module, we borrowed and extended a library named AROSG from the [artoolkitX project](https://github.com/artoolkitx/artoolkitx). This library provides a set of C++ functions with a C wrapper to perform tasks like loading models and drawing the rays of the laser pointers. Full documentation for these interfaces can be seen in the `arosg.h` header file.

* [`arosg.h`](arosg.h) - the main interfaces to the native code
* [`arosg.cpp`](arosg.cpp) - implements a set of functions that provide us with abstractions to OpenSceneGraph functionality
* [`ar_compat.h`](ar_compat.h) - a handful of macros normally found in artoolkitX
* [`ar_compat.c`](ar_compat.c) - a handful of macros normally found in artoolkitX
* [`mtx.h`](mtx.h) - utility functions to perform OpenGL-style matrix math
* [`mtx.c`](mtx.c) - utility functions to perform OpenGL-style matrix math
* [`osgPlugins.h`](osgPlugins.h) - macros to define which OpenSceneGraph plugins are to be linked statically into our application
* [`shaders.h`](shaders.h) - defines the shaders used by our OpenSceneGraph app.

The `build.sh` build script compiles and links these (and from that point onwards, these native files are not required to be present on the web server). The result is contained in these files:

* `arosg.wasm` - The compiled WebAssembly module
* [`arosg.js`](arosg.js) - The script file loaded by the HTML page that is responsible for loading the WebAssembly module, as well as interop between Javascript and WebAssembly.
* [`arosg.worker.js`](arosg.worker.js) - A script file loaded into worker threads in a multi-threaded WebAssembly environment.
* `arosg.data` - Contains the virtual filesystem for the WebAssembly.

### Models and other data used by native code

WebAssembly provides a variety of different means for the native code to access files at runtime. In this application, we pre-pack files needed by the native code into a virtual file system. This step is done at compile time by passing the flag `--preload-file` to the wasm compiler.

The files preloaded in this way include:

- [`models/`](models/) - The 3D models used in the app, in a supported format which includes OSG native, but also GeoTIFF (.tif) and glTF binary format (.glb).
- [`fonts/`](fonts/) - The font(s) used by OSG to render in-scene.
- [`shaders/`](shaders/) - Shaders required by OSG itself (i.e. in addition to the shaders we use in our app are directly are compiled into `shaders.h`). At present, just shaders required for osgText's text rendering.

AROSG allows for the use of a text-based `.dat` file to specify some global transformations and settings to be applied to each model. This allows for easy scaling and positioning of models with different scales and/or origins or axes.
 
## Licenses

[The license file](License) sets out the full license text.

* The HTML+JS code is licensed under the MPL2.
* The AROSG portions under the LGPLv3, and OpenSceneGraph under the OSGPL.

All of these licenses are compatible with use in a proprietary and/or commercial application.

## Building from source

### Prerequisites

What | Minimum version | Where to download 
---- | --------------- | ------------
Emscripten | | <https://emscripten.org/docs/getting_started/downloads.html> |
OpenSceneGraph for WebAssembly | 3.7.0 | <https://github.com/EtharInc/OpenSceneGraph/releases/download/OpenSceneGraph-3.7.0-wasm%2Bethar/openscenegraph-3.7.0-wasm.zip>
bash shell | | On macOS/Linux use Terminal<br> On Windows, use [Windows Subsystem for Linux](https://docs.microsoft.com/en-us/windows/wsl/install-win10), [Cygwin](https://cygwin.com/install.html), or [git-bash](https://gitforwindows.org/).

### Installing OpenSceneGraph for WebAssembly

These commands will download and unpack the compiled OpenSceneGraph for WebAssembly package and place it in the `dependencies` folder inside the repository:

```bash
cd dependencies
curl -LO https://github.com/EtharInc/OpenSceneGraph/releases/download/OpenSceneGraph-3.7.0-wasm%2Bethar/openscenegraph-3.7.0-wasm.zip
unzip openscenegraph-3.7.0-wasm.zip
```

### Compiling AROSG.

A convenience script `build.sh` allows for easy one-line compilation of the C/C++ sources and linking with OSG to form the WebAssembly module and loader code.

`bash ./build.sh`

To see verbose buid output, add the parameter `--verbose`. To do a debug build (with extra console logging and function names in debug stack backtraces) add `--debug`. E.g.:

`bash ./build.sh --verbose --debug`

## Modifying the application

To change the rendering itself, edit arosg.h/.cpp and recompile via build.sh.

See below for areas for extension and addition.

## WebAssembly requirements

While WebAssembly itself is supported on all WebXR-capable browsers, OpenSceneGraph assumes availability of threading and so must (at present) be built with WebAssembly's threads support enabled. WebAssembly threads depends in turn on Javascript's `SharedArrayBuffer` type, which now has tighter [security requirements](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/SharedArrayBuffer#security_requirements). You must ensure "cross origin isolation" is enabled for your site. (See below for details on how to enable "cross origin isolation".)

Supported browsers:

* At the date of writing, **Firefox** 79 and later (Windows, macOS, Linux).
    
  *Earlier versions of Firefox require a config flag to re-enable SharedArrayBuffer. In Firefox, type `about:config` in the URL bar, accept the warning, and then in the config search box type `javascript.options.shared_memory`. Click the ⇌ arrows symbol so that the value changes to `true` and close the config.*
  
* **Oculus Browser** (Oculus Quest 1 and 2)
* **Chrome for Windows and macOS** has SharedArrayBuffer enabled by default.
* **Chrome for Android** requires a config flag to enable use of SharedArrayBuffer with WebAssembly threads. In Chrome, type `chrome://flags` in the URL bar, and then in the flags search box type `WebAssembly threads`. Tap the pop-up and change the value from "Default" to "Enabled".
* **Microsoft Edge for Hololens 2** has SharedArrayBuffer enabled by default.

Unsupported browsers:

* **Magic Leap Helio** (Magic Leap) browser does not yet support multithreaded WebAssembly (as of Leap OS 0.98.20) although Magic Leap have tentatively committed to re-enabling support in a future release.
* **Safari for macOS/iOS** does not yet support SharedArrayBuffer.
* **Firefox Reality** (Oculus, HTC Vive, Pico VR) has SharedArrayBuffer disabled by default. Unfortunately, there is no user-accessible means to change the configuration. (Previous versions of Firefox Reality had SharedArrayBuffer enabled by defaul).
* **[Mozilla WebXR Viewer for iOS](https://apps.apple.com/app/webxr-viewer/id1295998056)** Since this is based on WebKit, like Safari for iOS, it does not support SharedArrayBuffer.

![1](doc/enable-js-sharedmem-ff.png)
![2](doc/enable-multithreaded-wasm-chrome-android.png)

### Enabling "cross origin isolation"

"Cross-origin isolation" refers to server-side config that enables protection features in the browser that make re-enablement of SharedArrayBuffer possible.

If your site is served by Apache, the simplest method is to create a `.htaccess` file in the top-level directory of your application with these lines included:

```
Header set Cross-Origin-Opener-Policy: same-origin
Header set Cross-Origin-Embedder-Policy: require-corp
```
To serve these, you will need to ensure that your apache config also loads the "headers" module. For Apache2, check the config file (`/etc/apache2/httpd.conf` or similar) has the option `LoadModule headers_module libexec/apache2/mod_headers.so` uncommented and enabled.

## Extensions and contributions welcomed:


### Controller models

The WebXR implementation does not at present render models for the device's hand controllers. The WebXR input profiles repository includes modules to allow selection and display of the appropriate 3D model for the user's controller(s). https://github.com/immersive-web/webxr-input-profiles/