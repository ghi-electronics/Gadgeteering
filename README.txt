# Gadgeteering

This is the repository for our Gadgeteering drivers. While we are keeping it up, it is no longer being actively maintained.

The repository is organized as follows:

-Core: contains the core Gadgeteering framework files such as mainboard and digital inputs.

-Dist: contains build files such as makefiles and Visual Studio projects for FEZLynx, a Windows Installer, and 3rd party headers and drivers that are needed, like FTDI for FEZLynx. The folder also contains an Output directory for FEZLynx that contains release and debug and x86 and x64 flavors of the pre-built static library.

-Examples: contains ready to build projects that demosrate how to use and the functionality of the mainboards and modules.

-Mainboards: contains the actual definitions for each mainboard we provide.

-Modules: contains the actual definitions for each module we provide.

-Tests: contains test programs that test and verify the functionality of the modules and mainboards.

-Gadgeteering.h: a helper file to be included that automatically references all needed core files.
