function smartDownload() {
	var e = document.getElementById('download');
	if (navigator.platform.indexOf("Win32") != -1 || navigator.platform.indexOf("Win64") != -1) {
		// Windows
		e.href = 'http://sourceforge.net/projects/performous/files/performous/0.6.1/Performous-0.6.1.exe/download';
		e.title = 'Performous 0.6.1 (Windows)';
		return;
	} else if (navigator.platform.indexOf("Linux") != -1) {
		// Linux
		return;
	} else if (navigator.userAgent.indexOf("Mac OS X") != -1 || navigator.userAgent.indexOf("MSIE 5.2") != -1 || navigator.platform.indexOf("Mac") != -1) {
		// Mac/Mac OS X
		e.href = 'http://sourceforge.net/projects/performous/files/performous/0.5.0/Performous-0.5.0.dmg/download';
		e.title = 'Performous 0.5.0 (OS X 10.5)';
		return;
	} else {
		// Other
		return;
	}
}
smartDownload();

