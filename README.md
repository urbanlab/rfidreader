rfidreader
==========

This program creates a small agent working under Windows, Mac OSX and Linux that interfaces with a USB RFID reader to make HTTP requests when specific tags are detected. The goal is to control a media display by manipulating physical objects above a reader.

Compilation instructions
------------------------

This program uses Qt and uses .pro files for controlling compilation. This can be used by qmake alone but has been only tested so far through Qt Creator IDE.

Dependencies:

- Qt 4.8+ (may work with earlier version, but untested) 
- libnfc (a pre-compiled version for each OS is provided in lib/ More on that below)

On all OSes, the .pro project should complile just fine. To deploy a final application, make sure that the applicable dynamic librairies (.dll under windows, .dylib under Mac OS X and .so under linux) are correctly accessible. 

In order to make the application avaible in French, you should also copy the rfidreader-fr.qm file into the same directory as the executable.

LibNFC
------

Compiling LibNFC correctly is a bit of a challenge under Mac OS X. Here are a few tips for anyone who tries it:
- you need to install libusb-legacy, NOT libusb or libusb-compat
- you need glib2-devel

Configure the project with ccmake and turn ON the following options: ACR122_USB, ACR122_PCSC and PN53X_USB

Change the file cmake/modules/FindLIBUSB.cmake to make it look for the library usb-legacy instead of usb

Add #include <stdint.h> in every file that indicates an unrecognized symbol like u_int32_t or uint8_t

Bundling under Mac OSX
----------------------

An incomplete bundling script is present in osx/bundler 
It helps packaging the application but still has some shortcomings. Especially, it won't correct the path where to look for the libnf.0.dylib file. To correct this step, go to the directory where your executable is located. Run:
 otool -L rfidreader
It will show you all the dependencies that rfidreader uses. One of the first one should be libnfc, indicating an absolute path, probably dependent on your installation. Replace it with a local path by running
 install_name -change <absolute path to libnfc.0.dylib> @executable_path/../Frameworks/libnfc.0.dylib

Then generate a dmg file using the floowing command:
 hdiutil create -format UDBZ -quiet -srcfolder <path to rfidreader.app> rfidreader.dmg
