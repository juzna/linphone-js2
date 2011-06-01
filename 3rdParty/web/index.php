<?php
require_once 'config.php';
?>
<html>
<head>
<link rel="shortcut icon" href="images/favicon.ico" type="image/x-icon" /> 
<title><?php echo $_global_config['header']; ?></title>
<link type="text/css" rel="stylesheet" media="all" href="css/template.css" /> 
<link type="text/css" rel="stylesheet" media="all" href="css/colorbox.css" /> 
<link type="text/css" rel="stylesheet" media="all" href="css/jquery-ui-1.8.12.custom.css" /> 
<script type="text/javascript" src="js/jquery-1.4.2.min.js"></script> 
<script type="text/javascript" src="js/jquery.colorbox.js"></script> 
<script type="text/javascript" src="js/jquery-ui-1.8.12.custom.min.js"></script> 
<script type="text/javascript" src="js/jquery.text-overflow.js"></script> 
<style>
body {
	background-color: <?php echo $_global_config['day-background-color']; ?>
}
.btn_blue, .btn_red, .btn_green {
	color: <?php echo $_global_config['day-button-font-color']; ?>;
}
</style>
</head>
<script type="text/javascript">
var plugin;
$(function() {
	//$("input.btn_blue, input.btn_red").colorbox({iframe:true, innerWidth:800, innerHeight:640,href:'video.php'});
	//$("input.btn_blue, input.btn_red").colorbox({iframe:true, innerWidth:800, innerHeight:640,href:'http://www.google.com',
	//	onOpen:function(){ alert($(this).attr('url')); }
	//});

    plugin = $('#plugin')[0];

	$(".display_text").ellipsis(true);
	
/*	$("button.btn_blue, button.btn_red, button.btn_green")
		.colorbox({
			innerWidth:1000, 
			innerHeight:540,
			div: $('#pluginWrapper'),
			onOpen: function() {
				if($(this).attr('sound')!='') {
					document.getElementById("dummyspan").innerHTML = "<embed src='"+$(this).attr('sound')+"' hidden=true autostart=true loop=false>";
				}
			},
            onCleanup: function() {
               $('#originalPlace').append($('#pluginWrapper'));
               setupSize();
            },
			onClosed: function() {
              resetSize();
				if($(this).attr('url_close')!='') {
					$.post($(this).attr('url_close'));
				}
			}
		});*/

    $("button.btn_blue, button.btn_red, button.btn_green").click(function() {
      showLightBox();
      var num = $(this).attr('number');
      if(num) plugin.call(num);
    });

    $("#closeBtn").click(function(){
      hideLightBox();
    });
	
	$('#timeswitch').click(function() {
		if($(this).attr("src") == 'images/moon.png') {
			$(this).attr("src", 'images/sun.png');
			$("#pagelogo").attr('src','<?php echo $_global_config['night-logo']; ?>');
			$('body').css({'background-color':'<?php echo $_global_config['night-background-color']; ?>'});
			$('.display_text').css({color:'<?php echo $_global_config['night-button-font-color']; ?>'});
			$('#td_header').css({color:'<?php echo $_global_config['night-header-font-color']; ?>'});
		} else {
			$(this).attr("src", 'images/moon.png');
			$("#pagelogo").attr('src','<?php echo $_global_config['logo']; ?>');
			$('body').css({'background-color':'<?php echo $_global_config['day-background-color']; ?>'});
			$('.display_text').css({color:''});
			$('#td_header').css({color:'<?php echo $_global_config['day-header-font-color']; ?>'});
		}

	});	
	
	
    $('#slider1').slider({
		value:1,
		min: 1,
		max: 2.5,
		step: .1,
		orientation: "vertical",
		slide: function( event, ui ) {
			$('#font_size').html(ui.value + ' cm');
			$('button').css({'font-size':ui.value+'cm'});
		},
		stop: function (event, ui) {
			$(".display_text").each(function(){
				$(this).html($(this).parent().parent().attr('title'));
				$(this).ellipsis();
			});
		}
	});
	$('#font_size').html($('#slider1').slider('value') + ' cm');
	
    lazyStart(); // Initialize sooon!
});


// Fallback for Firebug's console'
if(!window.console) console = {};
if(!console.log) console.log = function() {};



function lazyStart() {
  // Because it crashes, when starting video before the window is displayed properly
  window.setTimeout('completeStart()', 500);
}

function completeStart() {
	console.log('Init:', plugin.init());

	// Setup
	plugin.enableLogs("/tmp/linphone-js.log");
	plugin.enableStun("stun.helemik.cz");

    setupSize();

    // Start it
	console.log('Start:', plugin.start());
}

function setupSize() {
      // Window size
    plugin.width = plugin.videoSize.width;
    plugin.height = plugin.videoSize.height;
}

function resetSize() {
  plugin.height = 1;
  plugin.width = 1;
}

function quit() {
  console.log("Quit:", plugin.quit());
  plugin.width = 1;
  plugin.height = 1;
}

function startCall() {
  var dst = document.getElementById('dst').value;
  console.log("Calling ", dst);
  console.log("Result: ", plugin.call(dst));
}

function showLightBox() {
  $('#pluginWrapper').css({
    visibility: '',
    width: '100%',
    height: '100%',
    paddingTop: '20%'
  });
}

function hideLightBox() {
  $('#pluginWrapper').css({
    visibility: 'hidden',
    width: '1px',
    height: '1px',
    paddingTop: '0px'
  });
}



</script>
<body>
<div id="originalPlace" >
  <div id="pluginWrapper" style="visibility: hidden; text-align: center; backgroundColor: rgba(50, 50, 50, .5), position: absolute; z-index: -100; width: 1px; height: 1px; overlay: hidden;">
  <object id="plugin" type="application/x-linphone" width="1" height="1" onload="alert('The plugin has loaded!');">
  <param name="onload" value="pluginCB" />

  <param name="enableVideo" value="1" />
  <param name="enableVideoPreview" value="0" />
  <param name="embedVideo" value="1" />

  <param name="autoStart" value="0" />
  <param name="autoAccept" value="0" />

  <param name="username" value="571891103" />
  <param name="password" value="dummypass1A" />
  <param name="server" value="sip.helemik.cz" />
</object>

    <span id="closeBtn" style="font-size: 20px; position: absolute; right: 0px; top: 0px; cursor: pointer;">X</span>
</div>
</div>
<div style="float:right;"><img id="timeswitch" src="images/moon.png" /></div>
<center>
<table><tr><td><img id="pagelogo" src="<?php echo $_global_config['logo']; ?>" /></td>
			<td id="td_header" style="font-size:1.4cm;color:<?php echo $_global_config['day-header-font-color']; ?>"><?php echo $_global_config['header']; ?></td></tr></table>
<table><tr valign="top"><td>
	<div class="column">
		<p>
			<label for="font_size">Font size:</label><br />
			<span id="font_size" style="border:0; color:#f6931f; font-weight:bold;"></span>
		</p>
		<div id="slider1" style="height:10cm;width:1cm;"></div>
	</div>
</td><td>
	<div>
	<?php
	foreach($_button_config as $row) {
		echo '<div style="float:left;">
				<button type="button" class="btn_'.$row['button-color'].'" 
					url="shoutbox/'.$row['shoutbox-url'].'?title='.$row['title'].'"
					url_close="'.(!empty($row['shoutbox-close']) ? 'shoutbox/'.$row['shoutbox-close'] : '').'" 
					sound="'.(!empty($row['sound']) ? 'sound/'.$row['sound'] : '').'"
				    title="'.$row['title'].'"
				><div style="padding:0 25px 0 25px;"><div class="display_text" style="overflow:hidden;">'.$row['title'].'</div></div></button>
			</div>';
		/*echo '<div style="float:left;">
				<INPUT type="button" class="btn_'.$row['button-color'].'" 
					value="'.$row['title'].'" 
					url="shoutbox/'.$row['shoutbox-url'].'?title='.$row['title'].'"
					url_close="'.(!empty($row['shoutbox-close']) ? 'shoutbox/'.$row['shoutbox-close'] : '').'" 
					sound="'.(!empty($row['sound']) ? 'sound/'.$row['sound'] : '').'" 
				/>
			</div>';*/
	}
	?>
	</div>
</td></tr></table>

<div id=dummyspan></div>

</body>
</html>
