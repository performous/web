#/bin/sh

SVNPATH=~/ultrastar-ng
SITEPATH=/home/groups/u/ul/ultrastar-ng/htdocs

echo ">>> Updating the SVN repository"
svn up ${SVNPATH} || exit 1

echo ">>> Rebuilding and installing the website"

TEMPPATH=`mktemp -d`
svn export ${SVNPATH}/htdocs ${TEMPPATH}/htdocs &&
${SVNPATH}/webconv/webconv-x86-linux.bin ${SVNPATH}/htdocs-source ${TEMPPATH}/htdocs &&
rm -rf ${SITEPATH} &&
mv ${TEMPPATH}/htdocs ${SITEPATH} &&
echo ">>> INSTALLATION SUCCESSFUL" ||
echo "-!- INSTALLATION FAILED"

rm -rf ${TEMPPATH}

