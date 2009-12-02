#/bin/sh

GITPATH=~/src/performous-web
SITEPATH=~/public_html/performous

echo ">>> Updating the GIT repository"
git pull ${GITPATH} master || exit 1

echo ">>> Rebuilding and installing the website"

TEMPPATH=`mktemp -d`
${GITPATH}/webconv/webconv-x86-linux.bin ${GITPATH}/htdocs-source ${TEMPPATH} &&
rm -rf ${SITEPATH} &&
mv ${TEMPPATH} ${SITEPATH} &&
echo ">>> INSTALLATION SUCCESSFUL" ||
echo "-!- INSTALLATION FAILED"

rm -rf ${TEMPPATH}

