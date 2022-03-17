# ZLS OBS plugin

OBS plugin to add ZLS input source.

## Build

You need a working development environment for OBS Studio–see
[from source build instructions](https://obsproject.com/wiki/install-instructions).
It should be nothing more than cloning recursively the OBS repository and
building it using cmake then make. You don't need to `make install` it
(especially if you already have a working OBS Studio installation—don't
mess with it!).

Then run `build.sh` with the path to the `libobs` source. As example, if
you cloned the `obs-studio` repository  alongside this repository, run:

```bash
$ ./build.sh ../obs-studio/libobs
```

You'll get the plugin files in the `build/zls-plugin` folder. You can symlink
this folder to your OBS plugins folder for easier development:

```bash
$ ln -s ~/path/to/zls-obs-plugin/build/zls-plugin ~/.config/obs-studio/plugins/zls-plugin
```

## Licence

Copyright (C) 2022 Amaury Carrade and Enzo-Luciano Calamia

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
