var currentPage = window.location.href;

$('document').ready(function() {
	$('#sitebox').videoBG({
		position:"fixed",
		zIndex:-1,
		fullscreen:true,
		mp4:'/bgs/GoldenDust.mp4',
		ogv:'/bgs/GoldenDust.ogv',
		webm:'/bgs/GoldenDust.webm',
		poster:'/bgs/GoldenDust.jpg'
	});
	$('a').live('click', function(ev) {
		// We don't handle external links...
		if (ev.target.hostname != window.location.hostname) return;
		// Instead of following link, manipulate history (location) and call switchPage to do the loading
		history.pushState(null, "xhr", ev.target.href);
		if (switchPage()) ev.preventDefault();
	});
	function switchPage() {
		var href = window.location.href;
		if (href == currentPage) return false;  // Nothing to be done
		currentPage = href;
		$.ajax({
			type: 'GET',
			url: href,
			success: function(data) {
				// Update page content with data
				$('#page').html($('#page', data).html());
			},
			error: function() {
				window.location.href = href;  // Fallback to old-fashion way
			}
		});
		return true;
	}
	$(window).on("popstate", switchPage);
});


