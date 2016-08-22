# Vita HomeBrew Browser

## Prerequisites



## Building

To build VHBB, place yourself in the directory containing `Makefile` and use the `make` command to build it.

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
	You may use PNGO to optimise addition PNG files (included under `/tools/`).
- For some reasons, some PNG files created by GIMP makes the .vpk installation crash.

## License

