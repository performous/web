var img = Math.floor(Math.random() * 20);
var anim = 0;
function showcaseScroll() {
	var timeout = 3500;
	if (anim > 0) timeout = 16;
	if (anim == 30) { anim = -1; ++img; }
	setTimeout('showcaseScroll()', timeout);
	++anim;
	var e = document.getElementById('showcase');
	var pos = 128 * (img + 0.5 * (1 - Math.cos(Math.PI * anim / 30)));
	e.style.backgroundPosition = Math.round(-pos) + "px 0";
}
showcaseScroll();

