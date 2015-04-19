/**
	Udara klapne sa nase strane (dve).
	Udara obe u cugu, ako je prva vec udarena udara odmah drugu, ako je druga vec udarena udara samo prvu.
*/

function invert_x(points)
{
	var ret = {};
	for(var point_name in points)
	{
		ret[point_name] = {'x':-(points[point_name].x), 'y':points[point_name].y};
	}
	return ret;
}

var udarena = {'prva':false, 'druga':false}
var flap_otvoren = false;

var color = 'GREEN';

var pos_y = 400;
var yellow_positions = 
{
	'prilazna_prva':{'x':-1275, 'y':pos_y},
	'prilazna_druga':{'x':-740, 'y':pos_y},
	'kraj_prva':{'x':-1050, 'y':pos_y},
	'kraj_druga':{'x':-500, 'y':pos_y}
};
var green_points = invert_x(yellow_positions);
var positions_colored={'YELLOW':yellow_positions, 'GREEN':green_points};
var positions = positions_colored[color];

var orientation_colored = {'YELLOW':0, 'GREEN':180};
var orientation = orientation_colored[color];

var flap_colored = {'YELLOW':'Right', 'GREEN':'Left'};
var flap = flap_colored[color];

function onRun(){
	Logger.debug('running task: klapne');
	
	if(udarena.prva && udarena.druga) // za svaki slucaj
	{
		Manager.updateState("Finished");
		return;
	}
	
	if(!udarena.prva) // idi na prvu
	{
		Command.send(new MoveToPosition(positions.prilazna_prva.x,positions.prilazna_prva.y),function(){
		Command.send(new RotateTo(orientation),function(){
		set_flap_otvoren(true, function(){ // mozda ovde moze da se otvara i rotira u isto vreme TODO
		if(!udarena.druga) // idi skroz do kraja, udari obe
		{
			Logger.debug('positions.prilazna_druga='+positions.prilazna_druga);
			Logger.debug('positions.kraj_prva='+positions.kraj_prva);
			Logger.debug('should_close(x)='+should_close(800));
			
			Command.send(new MoveToPosition(positions.kraj_druga.x,positions.kraj_druga.y),function(){
			// ako je stigo do kraja druge
			set_flap_otvoren(false);
			set_udarena('druga');
			}, error,function(msg){
				// move to kraj_druga: progress notification
				Logger.debug('x='+msg.x);
				if(should_close(msg.x))
				{
					set_flap_otvoren(false);
					set_udarena('prva');
				}
				else
				{
					set_flap_otvoren(true);
				}
			});
		}
		else // udarena druga, udari samo prvu
		{
			Command.send(new MoveToPosition(positions.kraj_prva.x,positions.kraj_prva.y),function(){
			// ako je stigo do kraja prve
			set_flap_otvoren(false);
			set_udarena('prva');
			}, error);
		}
		});
		}, error);
		}, error);
	}
	else // udarena prva, idi odma na drugu
	{
		Command.send(new MoveToPosition(positions.prilazna_druga.x,positions.prilazna_druga.y),function(){
		Command.send(new RotateTo(orientation),function(){
		set_flap_otvoren(true, function(){ // mozda ovde moze da se otvara i rotira u isto vreme TODO
		Command.send(new MoveToPosition(positions.kraj_druga.x,positions.kraj_druga.y),function(){
		// ako je stigo do kraja druge
		set_flap_otvoren(false);
		set_udarena('druga');
		}, error);
		});
		}, error);
		}, error);
	}
}

function onPause(){}

function error()
{
	Logger.error('error in klapne');
	set_flap_otvoren(false);
	Manager.updateState("Suspended");
}

function set_udarena(klapna)
{
	udarena[klapna] = true;
	if(udarena.prva && udarena.druga)
	{
		Manager.updateState("Finished");
	}
}

function set_flap_otvoren(otvori, success)
{
	if (success === undefined)
	{
		success = function(){};
	}
	
	if(flap_otvoren)
	{
		if(otvori) success(); // vec otvoren
		else // zatvori
		{
			Command.send(new ActuatorCommand("Flap","Unkick"+flap), success, function(){Logger.error('error pri zatvaranju flapa');});
			flap_otvoren = false;
		}
	}
	else // zatvoren je
	{
		if(otvori) 
		{
			Command.send(new ActuatorCommand("Flap","Kick"+flap), success, function(){Logger.error('error pri otvaranju flapa');}); // otvori
			flap_otvoren = true;
		}
		else success(); // vec zatvoren
	}
}

function should_close(x)
{
	if(color == 'YELLOW')
	{
		return x > positions.kraj_prva.x && x < positions.prilazna_druga.x;
	}
	else // 'GREEN'
	{
		return x > positions.prilazna_druga.x && x < positions.kraj_prva.x;
	}
}

Manager.updateState("Ready");