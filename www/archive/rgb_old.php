<!DOCTYPE html>
<html>
<head>
<title>Poker Table Light Controller</title>
<meta name = "viewport" content = "width = device-width">
<script language="javascript" type="text/javascript" src="jquery.js"></script>
<script language="javascript" type="text/javascript" src="pokertable.js"></script>
<link type="text/css" href="pokertable.css" rel="stylesheet">
</head>
<body>

<h1>Poker Table Light Controller</h1>
<h2>RGB Set Mode</h2>
<a href="http://www.w3schools.com/tags/ref_colorpicker.asp" target="_blank">Color Picker</a>
<a href="index.php">Manual Set Mode</a>
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
						<input name='seat{$i}' id='seat{$i}' class='color-input' value='000000'>
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