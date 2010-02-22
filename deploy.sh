#/bin/sh

if test $# -ne 1 ; then
	echo "-!- Please choose a profile"
	exit
fi

case $1 in
	tronic)
		# deploying on performous.org
		GITPATH=~/performous-web
		SITEPATH=performous.org:/wwwhome/performous/
		;;
	yoda)
		# deploying on yoda-jm.performous.org (backup)
		GITPATH=~/git/performous/web
		SITEPATH=domU-3:/pub/www/performous/
		;;
	yoda-jm)
		# deploying on performous.sf.net (backup)
		GITPATH=~/git/performous/web
		SITEPATH=yoda-jm,performous@web.sourceforge.net:htdocs/
		;;
	*)
		echo "-!- \"$1\" is not a valid profile"
		exit
		;;
esac

echo ">>> Updating the GIT repository"
git pull ${GITPATH} master || exit 1

echo ">>> Rebuilding and installing the website"

TEMPPATH=`mktemp -d`
${GITPATH}/webconv/webconv-x86-linux.bin ${GITPATH}/htdocs-source ${TEMPPATH} &&
rsync -r --verbose ${TEMPPATH}/* ${SITEPATH} &&
rsync -r --verbose ${GITPATH}/htdocs-binary/* ${SITEPATH} &&
echo ">>> INSTALLATION SUCCESSFUL" ||
echo "-!- INSTALLATION FAILED"

echo ">>> Please run 'rm -rf \"${TEMPPATH}\"' (script will never do an rm -rf)"

