function smartDownload() {
	var e = document.getElementById('download');
	var agent = navigator.appVersion;
	if (agent.indexOf('Linux') != -1) return;
	if (agent.indexOf('Macintosh') != -1) {
		e.href = 'http://sourceforge.net/projects/performous/files/performous/0.5.0/Performous-0.5.0.dmg/download';
		e.title = 'Performous 0.5.0 (OS X 10.5)';
		return;
	}
	if (agent.indexOf('Windows') != -1) {
		e.href = 'http://sourceforge.net/projects/performous/files/performous/0.6.1/Performous-0.6.1.exe/download';
		e.title = 'Performous 0.6.1 (Windows)';
		return;
	}
}
smartDownload();

