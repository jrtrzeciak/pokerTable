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
<h2>Tournament Mode</h2>
<form oninput="brightout.value=parseInt(brightin.value)" action="">
	<div id="top_forms_t">
		<?php 
			$position[0] = 'dealer';
			$position[1] = 'sm_blind';
			$position[2] = 'big_blind';
			$position[3] = 'active';
			$position[4] = 'inactive';
			$position[5] = 'Dealer: ';
			$position[6] = 'Small Blind: ';
			$position[7] = 'Big Blind: ';
			$position[8] = 'Active: ';
			$position[9] = 'Inactive: ';
			
			for ($i = 0; $i < 5; $i++)		//Add the color selections for each seat
			{	
				echo	
				"
					<label class='label' for='{$position[$i]}'>{$position[$i+5]}</label>
					<select class='top_select' name='{$position[$i]}' id='{$position[$i]}'>
						<option class='000000' value='000000'>Off</option>
						<option class='FF0000' value='FF0000'>Red</option>
						<option class='FFFF00' value='FFFF00'>Yellow</option>
						<option class='00FF00' value='00FF00'>Green</option>
						<option class='00FFFF' value='00FFFF'>Teal</option>
						<option class='0000FF' value='0000FF'>Blue</option>
						<option class='FF00FF' value='FF00FF'>Purple</option>
						<option class='FFFFFF' value='FFFFFF'>White</option>
					</select>
				";
			}
		?>
		<label class="label" for="brightin">Brightness: </label>
		<input type="range" name="brightin" min="1" max="127" id="brightin" />
		<output name="brightout" id="brightout" for="brightin"></output><br />
	</div>
	<div id="table">
		<?php 
			for ($i = 1; $i < 9; $i++)
			{
				echo	
				"
					<div id='seat{$i}-container' class='seat-container'>
						<label for='seat{$i}'>Seat {$i}:</label><br />
						D:<input type='radio' name='deal_select' id='seat{$i}_d'  value='{$i}' />
						A:<input type='checkbox' name='seat{$i}' id='seat{$i}' value='{$i}' checked />
					</div>
				";
			}
		?>
		<div class="submit" id="submit_t">Next Hand</div>
	</div>
</form>
<div id="status">Sumbit the initial configuration when ready.</div>
</body>
</html>