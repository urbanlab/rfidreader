rfidreader
==========

This program creates a small agent working under Windows, Mac OSX and Linux that interfaces with a USB RFID reader to make HTTP requests when specific tags are detected. The goal is to control a media display by manipulating physical objects above a reader.

Compilation instructions
------------------------

This program uses Qt and uses .pro files for controlling compilation. This can be used by qmake alone but has been only tested so far through Qt Creator IDE.

Dependencies:

- Qt 4.8+ (may work with earlier version, but untested) 
- libnfc (a pre-compiled version for each OS is provided in lib/ More on that below)

* mention translation files for deployement
