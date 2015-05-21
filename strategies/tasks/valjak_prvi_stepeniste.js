/**
	Kupi prvi valjak kod stepenista.
*/

var positions_dole_colored = 
{
	'YELLOW':
	{
		'prilazna':{'x':-800, 'y':1400},
		'uvlacenje':{'x':-750, 'y':1535}, // x:733
	},
	'GREEN':
	{
		'prilazna':{'x':800, 'y':1400},
		'uvlacenje':{'x':750, 'y':1585},
	},
}

var orientations_dole_colored = 
{
	'YELLOW':
	{
		'uvlacenje':90+15,
		'prema_drugom':90-10,
		'izvlacenje':210,
	},
	'GREEN':
	{
		'uvlacenje':90-15,
		'prema_drugom':90+5,
		'izvlacenje':-30,
	}
}

var positions_gore_colored = 
{
	'YELLOW':
	{
		'prilazna':{'x':-895, 'y':1700},
		'kupi_1':{'x':-740, 'y':1680},
		'izvlacenje':{'x':-860, 'y':1725},
		'kupi2':{'x':-740, 'y':1760},
	},
	'GREEN':
	{
		'prilazna':{'x':895, 'y':1700},
		'kupi_1':{'x':740, 'y':1680},
		'izvlacenje':{'x':860, 'y':1725},
		'kupi2':{'x':740, 'y':1760},
	},
}

Config.setup = function()
{
	Config['positions_dole'] = positions_dole_colored[Config.color];
	Config['orientations_dole'] = orientations_dole_colored[Config.color];
	
	Config['positions_gore'] = positions_gore_colored[Config.color];

	if(Config.color == 'YELLOW') 
	{
		Config['primary_lift'] = 'LiftLeft';
		Config['secondary_lift'] = 'LiftRight';
	}
	else // GREEN
	{
		Config['primary_lift'] = 'LiftRight';
		Config['secondary_lift'] = 'LiftLeft';
	}
	
	Config['lift'] = Config.primary_lift;
}

var retry_count = 0;

function onRun()
{
	Task.running=true;
	if(Lift.has_room_for(2, Config.primary_lift)) Config['lift'] = Config.primary_lift; // ako ima mesta za primarni ok
	else Config['lift'] = Config.secondary_lift; // inace stavi onaj drugi
	
	if(Config.color == 'YELLOW')
	{
		if(Config.lift == 'LiftRight') Config['side'] = 'dole';
		else Config['side'] = 'gore'; // LiftLeft
	}
	else // GREEN
	{
		if(Config.lift == 'LiftRight') Config['side'] = 'gore';
		else Config['side'] = 'dole'; // LiftLeft
	}
	
	if(Config.side == 'gore')
	{
		CommandChain(Commands.pf_move(Config.positions_gore.prilazna))
		.catch(error)
		.then(new SetSpeedMotion(100))
		.then(new ActuatorCommand(Config.lift,'StartGetting'))
		.then(new MoveToPosition(Config.positions_gore.kupi_1.x, Config.positions_gore.kupi_1.y))
		.then(new SleepCommand(1000))
		//.then(new ActuatorCommand(Config.lift,'StopGetting'))
		//.then(new SetSpeedMotion(Config.default_speed))
		.then(new MoveToPosition(Config.positions_gore.izvlacenje.x, Config.positions_gore.izvlacenje.y, -1))
		.failure(function()
		{
			if(retry_count++ < 2)
			{
				this
				.then(new SleepCommand(1000))
				.then(new MoveToPosition(Config.positions_gore.izvlacenje.x, Config.positions_gore.izvlacenje.y, -1));
			}
			else
			{
				// alter da ide do treceg valjka na poziciju
				this.alter(Commands.finish_task);
			}
			return 'continue';
		})
		.then(new ActuatorCommand(Config.lift,'StartGetting'))
		//.then(new SetSpeedMotion(50))
		.then(new MoveToPosition(Config.positions_gore.kupi2.x, Config.positions_gore.kupi2.y))
		.then(new SleepCommand(1200))
		//.then(new ActuatorCommand(Config.lift,'StopGetting'))
		//.then(new SetSpeedMotion(Config.default_speed))
		.then(new MoveToPosition(Config.positions_gore.izvlacenje.x, Config.positions_gore.izvlacenje.y, -1))
		.then(new SetSpeedMotion(Config.default_speed))
		.then(Commands.finish_task)
		.execute();
	}
	else // dole
	{
		CommandChain(Commands.pf_move(Config.positions_dole.prilazna))
		.then(new MoveToPosition(Config.positions_dole.uvlacenje.x, Config.positions_dole.uvlacenje.y))
		.then(new RotateTo(90))
		.then(new ActuatorCommand(Config.lift,'StartGetting'))
		.then(new SetSpeedMotion(70))
		.catch(error)
		.then(new MoveForward(100))
		.then(new SleepCommand(700))
		.then(new ActuatorCommand(Config.lift,'StopGetting'))
		.then(new RotateTo(Config.orientations_dole.uvlacenje))
		.then(new MoveForward(-50))
		.then(new RotateTo(Config.orientations_dole.prema_drugom))
		.then(new ActuatorCommand(Config.lift,'StartGetting'))
		.then(new MoveForward(150))
		.then(new RotateTo(Config.orientations_dole.izvlacenje))
		.then(new MoveForward(200))
		.then(new SleepCommand(700))
		.then(new ActuatorCommand(Config.lift,'StopGetting'))
		//.then(new MoveForward(-distance))
		.then(new SetSpeedMotion(Config.default_speed))
		.then(new ActuatorCommand(Config.lift,'StopGetting'))
		.then(Commands.finish_task)
		.execute();
	}
}

function error()
{
	Logger.debug('prvi valjak stepeniste error');
	CommandChain(new SetSpeedMotion(Config.default_speed))
	.then(Commands.suspend_task)
	.execute();
}

function onPause()
{
	Task.running=false;
	CommandChain(new ActuatorCommand(Config.lift,'StopGetting')).execute();
}

Notification.subscribe("LiftProgressNotification", function(msg)
{
	if(Task.running && msg.type == 0 && Config.side == 'dole') // video senzor (0 - vidi senzor, 1 - uvatio valjak)
	{
		CommandChain(new ActuatorCommand(Config.lift,'StopGetting'))
		.then(new StopMovement())
		.execute();
	}
});

Lift.subscribe();

Manager.updateState("Ready");
