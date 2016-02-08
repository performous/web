#!/bin/sh

set -ue

cd "$(dirname "$0")"
P=../pages

cp .travis/deploy_key ~/.ssh/id_rsa

[ -d $P ] && rm -Rfv $P
git clone --depth=50 --branch=gh-pages git@github.com:performous/web.git $P

webconv/webconv htdocs-source $P

rsync -r --del --verbose htdocs-binary/* $P

cd $P

git config user.email 'nobody@performous.org'
git config user.name 'Performous Website Bot'
git config push.default simple

git add -A .
git commit -a -m "Update website from commit $TRAVIS_COMMIT"
git push origin HEAD:gh-pages
