window.onload = function () {
	$( "img" ).bind( "mouseover", function() {
		$(this).parent().parent().find(".author, .title").hide();
		this.style.width = 200;
		this.style.height = 200;
		$(this).parent().parent().css('zIndex','100');
	});
	$( "img" ).bind( "mouseout", function() {
		this.style.width = 80;
		this.style.height = 80;
		$(this).parent().parent().css('zIndex',100);
		$(this).parent().parent().find(".author, .title").show();
	});
}
