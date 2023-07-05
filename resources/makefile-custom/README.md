# Makefile

These are some versions of a default makefile file I use for my C / C++ projects. It works on Windows and Linux (tested on Ubuntu, Pop!OS and Arch so far) with no necessary changes.

## How It Works

For this makefile to work, header and source files must be organized into a folder named "source", then the makefile will create another folder named "objects" and will link header and source files generating .o files into it, automatically, and then it will compile the program.

* For changing the project name, just change 'PROJ_NAME' variable value.

* Else, for switching C / C++, just change the following variable values:

  * **CC**: represents the compiler you are going to use, you can change it into 'gcc' (for C projects) or 'g++' (for C++ projects).
  * **H_EXT**: represents your header files extension, you can use '.h' (for C projects) or '.hpp' (for C++ projects), but '.h' works for both.
  * **S_EXT**: represents your source files extension, you can use '.c' (for C projects) or '.cpp' (for C++ projects).

## Commands

1) **Compiling the project:**

``` sh
make
```

- _Using "make all" does the same thing._

2) **Compiling (if needed) and running the project:**

``` sh
make run
```

3) **Cleaning up the project folder (after compiled):**

```
make clean
```

- _Using this command will delete the executable file and the "objects" folder and all the files in it._

#
* This repo also includes a specif Makefile for compiling Allegro projects with all flags required for it, check "allegro" folder.
  * You can use the default Makefile file on "allegro" folder, it works for Windows and Linux OS. 
  * The other ones separated into specific folders are deprecated.
