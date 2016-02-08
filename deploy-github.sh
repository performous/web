#!/bin/sh

set -ue

cd "$(dirname "$0")"

[ -d ../pages ] && rm -Rfv ../pages
git clone -b gh-pages . ../pages

webconv/webconv htdocs-source ../pages

rsync -r --del --verbose htdocs-binary/* ../pages

cd ../pages

git config user.email 'nobody@performous.org'
git config user.name 'Performous Website Bot'
git config push.default simple
git remote set-url origin 'git@github.com:performous/web.git'

git add -A .
git commit -a -m "Update website from commit $TRAVIS_COMMIT"
git push origin HEAD:gh-pages
