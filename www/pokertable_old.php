<?php
ob_start();

// check if form was submitted from either web page
if(isset($_POST['program'])){
	// open client connection to TCP server
	if(!$fp=fsockopen('192.168.2.100',1234,$errstr,$errno,30)){
		trigger_error('Error opening socket',E_USER_ERROR);
	}
	//Convert the javascript POST variables to PHP variables.
	$program=$_POST['program'];
	$brightin=$_POST['brightin'];
	$seat[1]=$_POST['seat1'];
	$seat[2]=$_POST['seat2'];
	$seat[3]=$_POST['seat3'];
	$seat[4]=$_POST['seat4'];
	$seat[5]=$_POST['seat5'];
	$seat[6]=$_POST['seat6'];
	$seat[7]=$_POST['seat7'];
	$seat[8]=$_POST['seat8'];
	
	//Normalize the value of brightness such that the Arduino can parse the data
	if(strlen($brightin) == 2)
	{
		$brightin = "0".$brightin;
	}
	if(strlen($brightin) == 1)
	{
		$brightin = "00".$brightin;
	}
	
	$output = "?";					//Include a "?" at the front of the packet for error checking
	$output .= $program;			//Add the program id to the packet
	$output .= $brightin;			//Add the brightness value to the packet
	for ($i = 1; $i < 9; $i++)		//Add the color selections for each seat
	{
		$output .= $seat[$i];
	}
	$output .= "!";					//Include a "!" at the end of the packet for error checking
	
	echo $output;					//For debugging
	
	// write message to socket server
	fputs($fp,$output);
	// close socket connection
	fclose($fp);
}
?>