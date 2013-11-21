The repository is organized as follows:

-Build: contains build files such as makefiles and Visual Studio projects for FEZLynx and an installation script for FEZMedusa along with 3rd party headers and drivers that are needed, like FTDI for FEZLynx. The folder also contains an Output directory for FEZLynx that contains release and debug and x86 and x64 flavors of the pre-built static library for FEZLynx.

-Core: contains the core Gadgeteering framework files such as mainboard and digital inputs.

-Examples: contains ready to build projects that demosrate how to use and the functionality of the mainboards and modules.

-Mainboards: contains the actual definitions for each mainboard we provide.

-Modules: contains the actual definitions for each module we provide.

-Tests: contains test programs that test and verify the functionality of the modules and mainboards.

-Gadgeteering.h: a helper file to be included that automatically references all needed core files.