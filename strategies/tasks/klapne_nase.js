/**
	Udara klapne sa nase strane (dve).
	Udara obe u cugu, ako je prva vec udarena udara odmah drugu, ako je druga vec udarena udara samo prvu.
*/

var udarena = {'prva':false, 'druga':false}

var pos_y = 370; // TO_EDIT // TOME EDIT 350 
var yellow_positions = // TO_EDIT
{
	'prilazna_prva':{'x':-1280, 'y':pos_y}, // TOME EDIT 1278 // 1288
	'prilazna_druga':{'x':-800, 'y':pos_y}, //zuta x:-752 // TOME EDIT 780
};
var green_points = Motion.invert_x(yellow_positions);
var yellow_kraj_prva_x = -1162;

var positions_colored={'YELLOW':yellow_positions, 'GREEN':green_points};
var orientation_colored = {'YELLOW':0, 'GREEN':180};
var flap_colored = {'YELLOW':'Right', 'GREEN':'Left'};
var lift_colored = {'YELLOW': 'LiftRight', 'GREEN' : 'LiftLeft'};
var kraj_prva_x_colored = {'YELLOW':yellow_kraj_prva_x, 'GREEN':-yellow_kraj_prva_x};

var distance = 150;

function setup()
{
	Config['positions'] = positions_colored[Config.color];
	Config['orientation'] = orientation_colored[Config.color];
	Config['flap'] = flap_colored[Config.color];
	Config['lift'] = lift_colored[Config.color];
	Config['kraj_prva_x'] = kraj_prva_x_colored[Config.color];
	Lift.unloading(Config.lift);
}

function onRun()
{
	Config.do_setup(setup);

	Logger.debug('running task: klapne');
	
	if(udarena.prva && udarena.druga) // za svaki slucaj
	{
		Manager.updateState("Finished");
		return;
	}
	
	if(!udarena.prva) // idi na prvu
	{
		CommandChain(new SetSpeedMotion(50))
		.then(new MoveToPosition(Config.positions.prilazna_prva.x,Config.positions.prilazna_prva.y)) // pridji
		.then(new SetSpeedMotion(Config.default_speed))
		.then(new RotateTo(90)) // uvuci se u rikverc
		.then(new SetSpeedMotion(50))
		.then(new MoveForward(-distance))
		.then(new SetSpeedMotion(Config.default_speed))
		.then(new RotateTo(Config.orientation))
		.then(set_flap_otvoren(true))
		.then(function()
		{
			if(!udarena.druga) // idi skroz do kraja, udari obe
			{
				this
				.then(new MoveForward(750)) // kolko da ide napred // TO_EDIT
				.progress(function(msg)
				{
					if(should_close(msg.x))
					{
						CommandChain(set_flap_otvoren(false)) // zatvori dok ides pored protivmicke
						.then(set_udarena('prva'))
						.execute();
					}
					else
					{
						CommandChain(set_flap_otvoren(true)).execute(); // kad prodjes otvori
					}
				})
				.then(set_flap_otvoren(false)) // zatvori
				.then(set_udarena('druga'))
				.then(unload_lights);
			}
			else // udarena druga, udari samo prvu
			{
				this
				.then(new MoveForward(200)) // kolko da ide napred // TO_EDIT
				.then(set_flap_otvoren(false))
				.then(set_udarena('prva'));
			}
		})
		.then(Commands.finish_task)
		.catch(error)
		.execute();
	}
	else // udarena prva, idi odma na drugu
	{
		CommandChain(new MoveToPosition(Config.positions.prilazna_druga.x,Config.positions.prilazna_druga.y+100))
		.then(new MoveToPosition(Config.positions.prilazna_druga.x,Config.positions.prilazna_druga.y))
		.then(set_flap_otvoren(true))
		.then(new RotateTo(Config.orientation))
		.then(new MoveForward(200))
		.then(set_flap_otvoren(false))
		.then(set_udarena('druga'))
		.then(unload_lights) // istovari valjke
		.then(Commands.finish_task)
		.catch(error)
		.execute();
	}
}

function onPause(){}

function error()
{
	Logger.error('error in klapne nase');
	CommandChain(new SetSpeedMotion(Config.default_speed))
	.then(set_flap_otvoren(false))
	.then(function()
	{
		if(udarena.prva && udarena.druga)
		{
			this.then(Commands.finish_task);
		}
		else
		{
			this.then(Commands.ready_after(7000));
		}
	})
	.execute();
}

function set_udarena(klapna)
{
	return function()
	{
		udarena[klapna] = true;
	};
}

function unload_lights()
{
	if(Lift.has_items_to_unload()) // istovari valjke ako ih ima
	{
		this
		.then(new SetSpeedMotion(20))
		.then(new MoveForward(5))
		.then(new ActuatorCommand(Config.lift,'Unload'))
		.then(new MoveForward(-100))
		.then(new SetSpeedMotion(Config.default_speed));
	}
}

var flap_otvoren = false;
function set_flap_otvoren(otvori)
{
	return function()
	{
		if(flap_otvoren && !otvori)
		{
			this.then(new ActuatorCommand("Flap","Unkick"+Config.flap));
			flap_otvoren = false;
		}
		else if(!flap_otvoren && otvori)
		{
			this.then(new ActuatorCommand("Flap","Kick"+Config.flap));
			flap_otvoren = true;
		}
	};
}

function should_close(x)
{
	if(Config.color == 'YELLOW')
	{
		return x > Config.kraj_prva_x && x < Config.positions.prilazna_druga.x;
	}
	else // 'GREEN'
	{
		return x > Config.positions.prilazna_druga.x && x < Config.kraj_prva_x;
	}
}

Manager.updateState("Ready");