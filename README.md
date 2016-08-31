# Vita HomeBrew Browser

![Alt text](http://www.playlight.com.au/vita/vhbb/info/icon.png "")

## Prerequisites



## Building

To build VHBB, navigate to the directory containing `Makefile` and use the `make` command to build.

## Running

To run VHBB:
- send the generated `.vpk` to your vita:
	- start an FTP server on your vita.
	- upload the vpk to the vita using a FTP client.
- install the .vpk on your vita using a vpk installer (eg. VitaShell)
- a new folder will be created under `ux0:/app/` named `/VHBB00001/`.

## Notes
- icon0.png, startup.png and bg.png must be using indexed palettes.
- PNG files created in some image editors will cause the .vpk intallation to crash.
	You may use PNGO to optimise addition PNG files (included under `/tools/`),
	or [pngquant](https://pngquant.org/).

## License

- Code and scripts inside the folder `src\vpk_install` are licensed under the terms of GNU GPLv3. Check the file `src/vpk_install/license.txt` for the complete conditions.
- The rest of the files (unless specified otherwise) are licensed under the terms of [CC by-nc-sa 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/). Check the file `license.txt` for the complete conditions.
