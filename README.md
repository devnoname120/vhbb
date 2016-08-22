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
- a new folder will be created in under `ux0:/app/` named `/VHBB00001/`

## Notes
- icon0.png, startup.png and bg.png must be using indexed palettes.
- PNG files created in some image editors will cause the .vpk intallation to crash.
	You may use PNGO to optimise addition PNG files (included under `/tools/`),
	or [pngquant](https://pngquant.org/).

## License

Code and scripts licensed under the terms of [CC0 1.0 Universal](https://creativecommons.org/publicdomain/zero/1.0/).