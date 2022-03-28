#!/bin/sh

WEBCONV=./webconv/webconv
if test -f "$WEBCONV"; then
    echo "$WEBCONV exists."
else
    echo "$WEBCONV doesn't exist let's build it."
    g++ ./webconv/webconv.cpp -o ./webconv/webconv
fi
[ -d ./web ] && rm -rf ./web
git clone --depth=50 --branch=gh-pages git@github.com:performous/web.git
./webconv/webconv ./htdocs-source ./web
cp -rp ./htdocs-binary/* ./web
echo "Performous Web has been built. Check ./web/index.html to load the website locally."