1. compile:
g++ webconv.cpp -o webconv

2. edit source files, in ./source/ (optional)

3. convert (from ./source/* to ./html/*)
./webconv source html

4. examine results (mozilla html/index.html)

5. repeat from step 2 as required

This also works in non-UNIX systems (such as one quite popular one from
Redmond). Just get a C++ compiler and adapt this guide a bit.

The files must be in system's native format. If there are DOS line feeds and
the program was compiled on UNIX, it won't work. Convert your files with
dos2unix and unix2dos apps as required.

The default backbone.htm requires that you use UTF-8 in the source files.
Notepad and all other decent editors can do this. Just make sure you select
UTF-8 as encoding when saving. Of course, just edit the document to get other
charset, if you wish.

This tool has never been formally released, but consider it to be licensed
GNU GPLv3.

