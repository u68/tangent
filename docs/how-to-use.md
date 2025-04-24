# How to use

This is a hot piece of garbage right now, and there are big flaws and things that don't really work quite right *yet*

You can allways support me by making a pull request :)

So, once you have compiled the source, you then need to create a program to run, currently the OS pulls 1KiB of code from 0x511A to run, unless you modified the source of course.

To generate the code, use the `assembler.py` in `/asm`, (`python assembler.py <in.asm> <out.bin>`).

I like to use HxD to paste the binary into the main binary file, but any other way that doesnt fuck up everything else is also fine.

This is a CWII rom, so you will need a CWII model.bin & interface.png to run on [CasioEmuMSVC](https://github.com/telecomadm1145/CasioEmuMsvc)

Then you can run it easily by booting the model on the CasioEmuMSVC main menu, or how you do it on any other emulator.