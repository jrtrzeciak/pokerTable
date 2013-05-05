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
<h2>Manual Set Mode</h2>
<a href="rgb.php">RGB Set Mode</a>
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
						<label for='seat{$i}'>Seat {$i}:</label><br />
						<select name='seat{$i}' id='seat{$i}' class='seat'>
							<option class='000000' value='000000'>Off</option>
							<option class='FF0000' value='FF0000'>Red</option>
							<option class='FFFF00' value='FFFF00'>Yellow</option>
							<option class='00FF00' value='00FF00'>Green</option>
							<option class='00FFFF' value='00FFFF'>Teal</option>
							<option class='0000FF' value='0000FF'>Blue</option>
							<option class='FF00FF' value='FF00FF'>Purple</option>
							<option class='FFFFFF' value='FFFFFF'>White</option>
						</select>
					</div>
				";
			}
		?>
		<div class="submit" id="submit">Submit</div>
	</div>
</form>
<div id="status">Sumbit the initial configuration when ready.</div>
</body>
</html>