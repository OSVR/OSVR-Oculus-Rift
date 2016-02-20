# OSVR Oculus Plugin
> Maintained at <https://github.com/OSVR/OSVR-Oculus-Rift>
>
> For details, see <http://osvr.github.io>
>
> For support, see <http://support.osvr.com>

This is a plugin for OSVR that provides access to Oculus Rift trackers from OSVR applications. It makes use of the [Oculus VR SDK][ovr-sdk]. This plugin currently supports Oculus VR SDK versions 0.4.4-beta through 0.8.0.0-beta.

You need the Oculus Runtime installed for this plugin to work. Then, just put the `.dll` or `.so` file in the same directory of your OSVR server as the other plugins - usually something like `osvr-plugins-0` - and use the config file provided here as an example.

[ovr-sdk]: https://developer.oculus.com/

## Licenses

This project: Licensed under the Apache License, Version 2.0.

Note that since this builds on libOVR code, you are responsible for complying with the Oculus SDK's license as well.

