import sys
import os.path

# check argument
if len(sys.argv)!=3:
    print """Usage:
    ./bundler.py path_to_sources> <path_to_rfidreader.app>
Example:
    ./bundler /Users/Yves/rfidreader/ /Users/Yves/tmp/build-rfidreader-Desktop_Qt_5_2_0_clang_64bit-Release/rfidreader.app"""
    exit()

#check directories existence
for i in (1,2):
    if(not os.path.exists(sys.argv[i])):
        print("Error: Could not find "+argv[i])
        exit()

srcpath = sys.argv[1]
apppath = sys.argv[2]

# find macdeployqt
p = os.path.expanduser("~")
macdeploypath=""
for fn in os.listdir(p):
    if fn.startswith("Qt"):
        p2=p+"/"+fn+"/"
        print p2
        for fn2 in os.listdir(p2):
            #Look at the path structure
            ...

# copy dylibs from libs/ to Frameworks/
os.system("cp "+srcpath+"/lib/*.dylib "+apppath+"/Contents/Frameworks/")
# start macdeployqt
os.system(macdeploypath + " " + apppath)
# get result from otool -L rfidreader
(sin, sout) = os.popen2("otool -L "+apppath+"/Contents/MacOS/rfidreader")
s = sout.read().split("\n")
repmap=dict()
for ss in s:
    if ss.startswith("/opt/") or ss.startswith("/Users/"):
        

# find libs in /opt/ or /Users and replace them with @executable_path/../Frameworks/...
