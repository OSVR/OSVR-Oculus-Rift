# OSVR Oculus Plugin
> Maintained at <https://github.com/OSVR/OSVR-Oculus-Rift>
>
> For details, see <http://osvr.github.io>
>
> For support, see <http://support.osvr.com>

This is a plugin for OSVR that provides access to Oculus Rift trackers from OSVR applications. It makes use of the [Oculus VR SDK][ovr-sdk]. This plugin currently supports Oculus VR SDK versions 0.4.4-beta through 1.9.0-public.

[ovr-sdk]: https://developer.oculus.com/


## Building the Oculus Rift plugin

[Download][ovr-sdk] and install the both Oculus SDK and runtime. Run CMake and point the `OVR_ROOT_DIR` parameter at the location in which you installed the Oculus SDK. Set the `CMAKE_PREFIX_PATH` to include the path to OSVR and jsoncpp. Set the `CMAKE_INSTALL_PREFIX` parameter to the same location OSVR was installed to. When you build and install the plugin, the files will be copied to the proper location.

## Licenses

This project: Licensed under the Apache License, Version 2.0.

Note that since this uses the Oculus SDK, you are responsible for complying with the Oculus SDK's license as well.

