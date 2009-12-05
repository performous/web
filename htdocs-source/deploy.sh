#/bin/sh

if test $# -ne 1 ; then
	echo "-!- Please choose a profile"
	exit
fi

case $1 in
	tronic)
		GITPATH=~/src/performous-web
		SITEPATH=~/public_html/performous
		RSH=sh
		;;
	yoda)
		GITPATH=~/git/web
		SITEPATH=root@www:/pub/www/performous/
		RSH=ssh
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
rsync --rsh ${RSH} --archive --verbose ${TEMPPATH}/* ${SITEPATH} &&
rsync --rsh ${RSH} --archive --verbose ${GITPATH}/htdocs-binary/* ${SITEPATH} &&
echo ">>> INSTALLATION SUCCESSFUL" ||
echo "-!- INSTALLATION FAILED"

echo ">>> Please run 'rm -rf \"${TEMPPATH}\"' (script will never do an rm -rf)"

