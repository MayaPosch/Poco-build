# POCO-build #

POCO-build is a replacement build system for the [POCO](http://pocoproject.org/) framework. It consists out a collection of Makefiles that allow for the (cross-)compiling of the POCO libraries.

POCO-build targets the current (currently 1.10.1) version of POCO, it adds a zero-configuration compilation feature, with easy retargeting for new platforms. Currently it focuses on GCC and Clang toolchains.

## Installing ##

The POCO-build project files are to be copied into the POCO source folder (v1.10.1) as-is, so that its Makefiles overwrite those in the POCO source folder and sub-folders. Confirm the replacing of any existing files.

## Using ##

After installing POCO-build, navigate to the root folder of the POCO source folder. The syntax for running POCO-build is:

	$ make [module] TARGET=[target]

Here `module` is optional while `target` is required.

**Targets:**

* Windows (MSYS2, MinGW).
* Android-aarch64.


**Modules:**

* foundation
* net
* util
* json

**Cleaning:**

All intermediate build objects can be cleaned with:

	$ make clean TARGET=[target]

This will clean the object files for the specific target. By specifying `clean-[module]`, a specific module can be cleaned for that target without affecting the others.

## Output binaries ##

POCO-build creates a `build/[target]/poco/lib/` path in the POCO source folder with the produced binaries. These include:

* libPoco[module].a
* libPoco[module].so.[version]

## Adding a target ##

To add a new target to POCO-build, a new Makefile.[target] file has to be created in the root folder, alongside the other target files. These define the toolchain name as well as C and C++ toolchain parameters that are to be passed to the toolchain.

At this point it's probably easiest to modify an existing target file for one's purposes to make sure that all of the variable names are present and correct.

Any potential target that supports either GCC or Clang with a C++ language level of C++14 or better qualifies.

## Issues ##

* Android-aarch64 target is limited to Windows hosts: needs to have the Linux & MacOS NDK paths added.
* Shared libraries produced for Windows targets are not valid DLLs.
* Adding of remaining POCO modules.
* Preprocessor issue in Net module (ICMPv4PacketImpl.h) hot-fixed with #undef of offending #define from wincrypt.h. (Fix provided in POCO-build)
* pocomsg.h missing by default. (Fix provided in POCO-build)
