# TRK_Hollywood_Revolution.a

This repository covers the decompiled source code to the debugger NUB ("Kernel")
of "MetroTRK for Revolution", whereby there exist two different versions of this
particular library: v0.1 and v0.4

The latter has been fully decompiled to a "one-on-one" hex binary match, compared
between the object file(s) of the original library and the fresh compiled object
file(s).

Currently, the source code within this repository enables v0.4 operation.
The previous release (v0.1) and it's object files of the original library, once
extracted, cover dates from the years between 1999 and 2006. The older library
code was harder to decompile due to the fact that (most likely) multiple compiler
releases were used to compile different parts of the library code but at the same
time, most of the older library code is included here as well but disabled. At
thesame time it is very unlikely that the v0.1 source code would ever run with
success.

## Build / Clean-up instructions

To compile the required object files and create the actual library, it requires
the command-line compiler and linker of "CodeWarrior for Wii". The exact compiler
version is "v4.2 build 141" ("Runtime Built July, 31st 2008"). Same goes for the
assembler and linker. They also require the FLEX license manager DLL coming with
them and they need to be put into the empty "bin" folder.

From the command-line, run file "build.bat" or "clean.bat" to either compile the
code or clean things up. The final library will be located within folder "lib".

## Origin / Background

"MetroTRK" / "CodeWarriorTRK" / "AppTRK" are the common names used across
multiple CodeWarrior IDE's for either "mobileGT" / "68K" / "Power Architecture".
They all cover different versions of the source code whereby two versions are of
use within every single repository: "Kernel version" vs. "Protocol version".

"Kernel version" most likely represents the modified code of the original source
"Protocol version" represents the source code of the API back in the day before
changes were applied to make things run on different hardware / MCU's.

For this particular library right here, "Protocol version" was v1.10 which
during development turned into "Kernel version" v0.4 in the end.

For the decompilation, "Protocol version" v3.2 was used as it's origin before
things turned out to be much older. So lots of changes where done to recover the
source code to the original binaries.

## Christmas gift: GCC / GNU additions

The intention to decompile this library is to port it over to GCC for use with
libOGC (devkitPPC). Most of the required modifications are already included and
visible as "#if" definitions. Aside from that, libOGC itself needs modifications
and it will also require a modified linker script, GNU plugins for CodeWarrior
and specific CodeWarrior project data.

# Test results

Tests were successful to the point, that the code within here works the way it
should when compiling and linking it using the command-line tools of CodeWarrior
for Wii. For GCC v4.8.2 as of devkitPro r27, once all the requirements are set
and an "App" is run, it will work for everything but SDL / big projects so far.



Merry christmas 2023
