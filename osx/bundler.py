import sys
import os.path

def examine_dylib(path):
    (stin, stout) = os.popen2("otool -L "+apppath+path)#"/Contents/Frameworks/libnfc*dylib")
    s = stout.read()
    repmap=dict()
    for ss in s.split("\n")[2:]:
	ss = ss.lstrip()
        if ss.startswith("/opt/") or ss.startswith("/Users/"):
            print "WARNING: Please take care of this dependence in {0} : {1}".format(path,ss)        

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
        p2=p+"/"+fn
        if os.path.isdir(p2):
	    print p2
            for fn2 in os.listdir(p2):
                p3 = p2+"/"+fn2
		if os.path.isdir(p3):
		    for fn3 in os.listdir(p3):
			p4 = p3+"/"+fn3
			if os.path.isdir(p4):
			    for fn4 in os.listdir(p4):
				if fn4=="bin":
				    macdeploypath = p4 + "/" + fn4 + "/macdeployqt"
if os.path.exists(macdeploypath):
    print "Found macdeployqt : " + macdeploypath
else:
    print "Error: Could not find macdeployqt"
    exit()			    

# copy dylibs from libs/ to Frameworks/
os.system("cp "+srcpath+"/lib/*.dylib "+apppath+"/Contents/Frameworks/")
# start macdeployqt
os.system(macdeploypath + " " + apppath)
# get result from otool -L rfidreader
#(stin, stout) = os.popen2("otool -L "+apppath+"/Contents/MacOS/rfidreader")
examine_dylib("/Contents/Frameworks/libnfc.dylib")
examine_dylib("/Contents/Frameworks/libusb-legacy.dylib")
examine_dylib("/Contents/MacOS/rfidreader")

os.system("hdiutil create -format UDBZ -quiet -srcfolder "+apppath+" rfidreader.dmg")
