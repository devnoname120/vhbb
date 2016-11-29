# Vita HomeBrew Browser

![Alt text](http://www.playlight.com.au/vita/vhbb/info/icon.png "")

## Prerequisites

Optional:

- [PSP2SHELL](https://github.com/Cpasjuste/PSP2SHELL#readme): automatically send and reload app after compilation, see [Running](#Running).
- debugnet: real-time logging that can be viewed from your computer, see [Logging](#Logging).

## Building

To build VHBB, navigate to the directory containing `Makefile` and use the `make` command to build.

## Running

To run VHBB:

- send the generated `.vpk` to your vita:
	- start an FTP server on your vita, for example [VitaShell](https://github.com/TheOfficialFloW/VitaShell/releases).
	- upload the vpk to the vita using a FTP client.
- install the .vpk on your vita using a vpk installer, for example [VitaShell](https://github.com/TheOfficialFloW/VitaShell/releases).
- a new folder will be created under `ux0:/app/` named `/VHBB00001/`.

You can also use [PSP2SHELL](https://github.com/Cpasjuste/PSP2SHELL#readme) in order to send and reload the application easily after compilation:

- put the IP of your PS Vita in a new file `psvitaip.txt`
- compile using `make psp2shell=1` and install the resulting VPK on your Vita like above
- start the application on your Vita
- every time you do modifications and want to test them, you can execute `make psp2shell psp2shell=1` which will automatically compile, send, and reload the application.

## Logging

Debug code prints text in order to help the debugging. Currently only debugnet is supported. We will probably add file logging in the future.

Instructions to use debugnet:

- install [debugnet](https://github.com/psxdev/debugnet#readme)
- set the local IP of your computer in `debugnetip.txt`
- compile VHBB using `make debugnet=1`
- on your computer, listen to the port 18194
  - with [netcat](http://nc110.sourceforge.net/): `nc -u -l -p 18194`
  - with [socat](http://www.dest-unreach.org/socat/): `socat udp-recv:18194 stdout`
- install VHBB on your PS Vita and start it. The logging should display in the terminal.

## Notes

- icon0.png, startup.png and bg.png must be using indexed palettes.
- PNG files created in some image editors will cause the .vpk intallation to crash.
	You may use PNGO to optimise addition PNG files (included under `/tools/`),
	or [pngquant](https://pngquant.org/).

## License

- Code and scripts inside the folder `src\vpk_install` are licensed under the terms of GNU GPLv3. Check the file `src/vpk_install/license.txt` for the complete conditions.
- The rest of the files (unless specified otherwise) are licensed under the terms of [CC by-nc-sa 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/). Check the file `license.txt` for the complete conditions.
