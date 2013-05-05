<!DOCTYPE html>
<html>
<head>
<title>Poker Table Light Controller</title>
<meta name = "viewport" content = "width = device-width">
<script language="javascript" type="text/javascript" src="jquery.js"></script>
<script src='spectrum.js'></script>
<script language="javascript" type="text/javascript" src="pokertable.js"></script>
<link rel='stylesheet' href='spectrum.css' />
<link type="text/css" href="pokertable.css" rel="stylesheet">
<script>
$(document).ready(function() {
	var i;	//Set up counter variable.
	//For each seat, initialize color value and parameters for color picker.
	for(i=1; i<9; i++)
	{
		$("#seat"+i).spectrum({
    		color: "#000000",			//initialize color in the color picker
    		clickoutFiresChange: true,	//allow clicking out to choose color
    		preferredFormat: "hex6",	//make sure the format is #xxxxxx
    		showInput: true,			//show the input for the hell of it
    		showPalette: true,			//show standard colors to pick from
    		showSelectionPalette: true,	//show previously picked colors
    		palette: [
        		['red', 'yellow', 'lime'],
        		['aqua', 'blue', 'fuchsia'],
        		['white', 'grey', 'black']
    		],
    		change: function() {
    			backColor();
			}
		});
		$("input#seat"+i).val('#000000');	//initialize the color in the input
	}
	
	backColor();	//Initialize the background colors when the page loads
	
	function backColor()
	{
		var s = new Array();
		var t = new Array();
		var u = new Array();
		for(i=1; i<9; i++)
		{
			s[i] = $("#seat"+i).spectrum("get");	//Get the color from spectrum
			t[i] = s[i].toHexString();				//Convert to Hex for Css
			u[i] = s[i].toRgb();	//Convert to Rgb to figure out relative brightness
			//Change the background color based on the input
			$('#seat'+i+'-container').css("background-color",t[i]);
			//If the background is relatively bright, make the text and border black.
			if (u[i].r+u[i].g+u[i].b >= 255*3/2)
			{
				$('#seat'+i+'-container').css("color",'black');
				$('#seat'+i+'-container').css("border-color",'black');
			}
			//Otherwise, keep it white.
			else
			{
				$('#seat'+i+'-container').css("color",'white');
				$('#seat'+i+'-container').css("border-color",'white');
			}
		}
	}
});
</script>
</head>
<body>

<h1>Poker Table Light Controller</h1>
<h2>RGB Set Mode</h2>
<a href="http://bgrins.github.com/spectrum" target="_blank">jQuery Color Picker</a>
<a href="tournament.php">Tournament Mode</a>
<form oninput="brightout.value=parseInt(brightin.value)" action="">
	<div id="top_forms">
		<label class="label" for="program">Program: </label>
		<select class="top_select" name="program" id="program">
			<option value="0">Off</option>
			<option value="1">Color Chase</option>
			<option value="2">Color Wipe</option>
			<option value="3">Rainbow</option>
			<option value="4">Rainbow Cycle</option>
			<option value="5">Fade</option>
		</select><br />
		<label class="label" for="brightin">Brightness: </label>
		<input type="range" name="brightin" min="1" max="127" id="brightin" />
		<output name="brightout" id="brightout" for="brightin"></output><br />
	</div>
	<div id="table">
		<?php 
			for ($i = 1; $i < 9; $i++)		//Add the color selections for each seat
			{
				echo	
				"
					<div id='seat{$i}-container' class='seat-container'>
						<label for='seat{$i}'>Seat {$i}</label><br />
						<input name='seat{$i}' id='seat{$i}' class='color-input'>
					</div>
				";
			}
		?>
		<div class="submit" id="submit_r">Submit</div>
	</div>
</form>
<div id="status">Sumbit the initial configuration when ready.</div>
</body>
</html>