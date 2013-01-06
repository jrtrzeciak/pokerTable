<?php 
	for ($i = 1; $i < 9; $i++)		//Add the color selections for each seat
	{
		echo	
		"
			<div id='seat{$i}-container' class='seat-container'>
				<label for='seat{$i}'>Seat {$i}</label><br />
				<input name='seat{$i}' class='color-input'>
			</div>
		";
	}
?>