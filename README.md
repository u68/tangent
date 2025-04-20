# Tangent

This is an operating system / kernel for *nx u8-100* devices ||Casio cwii calcs 🤫||

Currently, it does not work or I have not been bothered to test it, BUT I am working on it little by little, I would say it will be done in about a week and a bit.

## Usage

Because it is not finished, you can't really use it, but I plan on making it so it can be budled with programs after building and then be ran on an emulator or real hardware, I see no real use case for this project, it is really just a fun little thing that I find pretty neat.

Have fun with it!

## Building

You might think that this is some nice and easy cpp that you can throw at your computor, but you can't its for your calculator, to build it, either go through tht painful build settings of LAPIS Lexide Omega IDE, which I strongly do __NOT__ recommend, so here are all the commands:

```
rasu8 -CD -ABW -SD -DF -ML -O"./" -WRPEAST -PR"main.prn" -L -NS -NR -NPL -NPW "startup.asm"
```

This first command I am not so sure about, because it did not show in the command output bar, and I haven't tested it out yet :p

```
ccu8 -SD -ABW -TML620909 -ML -far -Fa"./" -Ot -Oa -W1 "../main.c"
rasu8 -CD -ABW -SD -DF -ML -O"./" -WRPEAST -PR"main.prn" -L -NS -NR -NPL -NPW "main.asm"
rlu8 -D -SD -NS -CC -ABW  "./main.obj" "./startup.obj" ,"Calcpaint.abs",,"LU8100LW.LIB";
CnvOHU8 -H  Calcpaint.abs  "Calcpaint.HEX";
Ohu8.exe "/H" "Calcpaint.abs" "Calcpaint.HEX;"
```
