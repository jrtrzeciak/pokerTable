// This function passes the color, brightness, and program data to the server side code from the rgb/index page.
// A status code is then returned from the code to be displayed on the web page beneath the respective form.

$(function() {
	$("#submit_r").click(function() {

		//Gather values from page when submit is clicked
		var program = $("select#program").val();
		var brightin = $("input#brightin").val();
		var seat1 = $("input#seat1").val().substring(1);
		var seat2 = $("input#seat2").val().substring(1);
		var seat3 = $("input#seat3").val().substring(1);
		var seat4 = $("input#seat4").val().substring(1);
		var seat5 = $("input#seat5").val().substring(1);
		var seat6 = $("input#seat6").val().substring(1);
		var seat7 = $("input#seat7").val().substring(1);
		var seat8 = $("input#seat8").val().substring(1);

		//Acknowledge pressing of the button
		$('#status').html("Sending...");
		$(this).css("background-color","white");
		
		//Assemble a data packet to send to the server side code
		var dataString = "program="+program+"&brightin="+brightin+"&seat1="+seat1+"&seat2="+seat2+"&seat3="+seat3+"&seat4="+seat4+"&seat5="+seat5+"&seat6="+seat6+"&seat7="+seat7+"&seat8="+seat8;

		$.ajax({
			type: "POST",					//Send as POST
			url: "pokertable.php",			//To server side code
			data: dataString,				//Send the data from the page
			datatype: 'html',				//As HTML
			success: function(data) {		//Acknowledge completion of delivery
				//$('#status').html("Submission Complete");
				$('#status').html(data);
				$('#submit_r').css("background-color","#0D1122");
			}
		});
		return false;
	});
});

//This function moves the radio button to the next active seat on the tournament page

function nextSeat(seatCheck, currentSeat)
{
	var i;					//Set up index variable
	i = currentSeat + 1;	//Increment the seat number by one
	
	//Loop through the seats clockwise until the next active one is found
	for (;;)
	{
		if (i > 8)			//If you get to 8, go back to 1
			i = 1;
		if (seatCheck[i] == true)	//If the seat is active, make it the new seat
		{
			newSeat = i;
			break;
		}
		else						//If not, keep looking
		{	
			i++;
		}
	}
	return newSeat;					//Return the new seat value
}

//This function strips a seat div of its background color and add the appropriate new color

function setColor(index, attribute)
{
	//Get the color of the specific attribute from the input boxes at the top of the page
	var color = $('input#'+attribute).val();  
	//Change the div color of the specified seat accordingly
	$('input:radio[value='+index+']').change(function (){
		$(this).removeClass('dealer sm_blind big_blind active inactive')	//remove old seat type
		var backColor = color;		//CSS requires a # in front of the hex code.
		$(this).css("background-color",backColor);			//add new color based on the input boxes
		$(this).parent().css("background-color",backColor);
		$(this).addClass(attribute);						//add new seat type from function call
	}).change();
	return color.substring(1);		//return the color to be passed along to the server, then controller
}

//This function passes the active seat and dealer and blind colors to the server side code from the tournament page.
$(function() {
	$("#submit_t").click(function() {
		
		var i;
		//Set up variables for the different seat types
		var deal_pos;
		var sm_blind_pos;
		var big_blind_pos;
		//Stop the controller from starting a program
		var program = '0';
		//Gather the values from the input boxes
		var dealer = $("input#dealer").val().toUpperCase();
		var sm_blind = $("input#sm_blind").val().toUpperCase();
		var big_blind = $("input#big_blind").val().toUpperCase();
		var active = $("input#active").val().toUpperCase();
		var inactive = $("input#inactive").val().toUpperCase();
		
		//Find out where the dealer is located
		var deal_select = $('input:radio[name=deal_select]:checked').val();
		//Find what seats are still in the game 
		var seatCheck = new Array();
		seatCheck[1] = $("#seat1").is(":checked");
		seatCheck[2] = $("#seat2").is(":checked");
		seatCheck[3] = $("#seat3").is(":checked");
		seatCheck[4] = $("#seat4").is(":checked");
		seatCheck[5] = $("#seat5").is(":checked");
		seatCheck[6] = $("#seat6").is(":checked");
		seatCheck[7] = $("#seat7").is(":checked");
		seatCheck[8] = $("#seat8").is(":checked");
		var seat = new Array();

		//Acknowledge the button was pressed
		$('#status').html("Sending...");
		$(this).css("background-color","white");
		
		//Make sure at least one player is active
		if($('input[type=checkbox]:checked').size() < 1)
		{
			$('#status').html("Please select at least one active player.");
			$(this).css("background-color","");
			return false;
		}
		//If no dealer is selected, default the current value to 8.
		//That way, when the nextSeat function is called the dealer will default to
		//the numerically lowest active player
		if($('input[type=radio]:checked').size() < 1)
		{
			deal_pos = 8;
		}
		//Otherwise, just set the dealer position to the seat with the selected radio button
		else
		{
			deal_pos = parseInt($('input:radio[name=deal_select]:checked').val());
		}
		
		//This next if statement assigns values to the position variables.
		//That way, the next for loop can compare the number of each seat to the 
		//values assigned to the position variables. If they match, the setColor function
		//is called with the correct seat number, attribute pair.
		
		//The logic was set up this way to avoid making the seat variable a
		//two-dimensional array. It will have to be changed once I incorporate feedback
		//from the controller anyway.
		
		//When there are only two people left, exceptions for the big blind must be
		//taken into account.	
		if($('input[type=checkbox]:checked').size() == 2)
		{
			sm_blind_pos = 0;	//The small blind, not big blind, goes away
			//Find the current big blind position
			big_blind_seat = parseInt($('input:radio[class=big_blind]').val());
			
			//If the dealer went out, and the big blind stayed in:
			if((!seatCheck[deal_pos]) && (seatCheck[big_blind_seat]))
			{
				//The rule is that the big blind will always advance. Do this first.
				//Otherwise, with the standard logic, the dealer would go to the other
				//active seat, and the big blind wouldn't move
				big_blind_pos = nextSeat(seatCheck, deal_pos);
				//Then, just make the dealer the other seat after the big blind advances
				deal_pos = nextSeat(seatCheck, big_blind_pos);
			}
			//If the dealer doesn't go out, the standard logic is fine.
			//The only difference is the small blind is absent.
			else
			{
				//Advance the dealer to the next available seat
				deal_pos = nextSeat(seatCheck, deal_pos);
				//The big blind goes to the next available seat after that
				big_blind_pos = nextSeat(seatCheck, deal_pos);
			}				
		}
		//If there are more than two players active, the "standard logic" is as follows
		else
		{
			//Advance the dealer to the next available seat
			deal_pos = nextSeat(seatCheck, deal_pos);
			//The small blind goes to the next available seat after that
			sm_blind_pos = nextSeat(seatCheck, deal_pos);
			//The big blind goes to the next available seat after that
			big_blind_pos = nextSeat(seatCheck, sm_blind_pos);
		}
		
		$("input:radio[name=deal_select]")[value= deal_pos - 1].checked = true;
		
		//Loop through the seats and set the colors accordingly
		//If the seat number and the value of the position match, call the set color
		//function with the correct position and attribute
		for (i = 1; i < 9; i++)
		{
			if (i == deal_pos)
				seat[i] = setColor(i, 'dealer');
			else if (i == sm_blind_pos)
				seat[i] = setColor(i, 'sm_blind');
			else if (i == big_blind_pos)
				seat[i] = setColor(i, 'big_blind');
			else if (seatCheck[i])
				seat[i] = setColor(i, 'active');
			else
				seat[i] = setColor(i, 'inactive');
		}
		
		//Package the now complete packet
		var dataString = "brightin=000&program="+program+"&seat1="+seat[1]+"&seat2="+seat[2]+"&seat3="+seat[3]+"&seat4="+seat[4]+"&seat5="+seat[5]+"&seat6="+seat[6]+"&seat7="+seat[7]+"&seat8="+seat[8];
		
		$.ajax({
			type: "POST",					//Send as POST
			url: "pokertable.php",			//To server side code
			data: dataString,				//Send the data from the page
			datatype: 'html',				//As HTML
			success: function(data) {		//Acknowledge completion of delivery
			//$('#status').html("Submission Complete");
			$('#status').html(data);
			$('#submit_t').css("background-color","#0D1122");
			}
		});
		return false;
	});
});