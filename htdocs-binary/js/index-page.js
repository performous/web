var img = Math.floor(Math.random() * 20);
var anim = -1;
var frames_anim = 120;
var frames_img = 400;

function showcaseScroll() {
	++anim;
	if (anim == frames_img) { anim = 0; ++img; }
	var a = Math.min(anim / frames_anim, 1.0);
	var pos = 5 * 128 * (img + 0.5 * (1 - Math.cos(Math.PI * a)));
	$('#showcase').css('backgroundPosition', Math.round(-pos) + 'px 0');
}

function smartDownload() {
	var e = document.getElementById('download');
	if (!e) return;
	if (navigator.platform.indexOf("Win32") != -1 || navigator.platform.indexOf("Win64") != -1) {
		// Windows
		e.href = 'https://github.com/performous/performous/releases/download/1.0/Performous-1.0.exe';
		e.title = 'Performous 1.0 (Windows)';
		return;
	} else if (navigator.platform.indexOf("Linux") != -1) {
		// Linux
		return;
	} else if (navigator.userAgent.indexOf("Mac OS X") != -1 || navigator.userAgent.indexOf("MSIE 5.2") != -1 || navigator.platform.indexOf("Mac") != -1) {
		// Mac/Mac OS X
		e.href = 'https://github.com/performous/performous/releases/download/1.0/Performous-1.0.dmg';
		e.title = 'Performous 1.0 (OS X 10.6 or newer)';
		return;
	} else {
		// Other
		return;
	}
}

$('document').ready(function() {
	setInterval(smartDownload, 50);
	setInterval(showcaseScroll, 16.66);
});



