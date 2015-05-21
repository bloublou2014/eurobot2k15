/**
	Kupi jedan valjak u gornjem uglu.
*/

distance_gore = 100;
distance_dole = 170;

var yellow_positions = // TO_EDIT
{
	'prilazna_gore':{'x':-1100, 'y':1692},
	'prilazna_dole':{'x':-1200, 'y':1550},
};

var green_points = // TO_EDIT
{
	'prilazna_gore':{'x':1100, 'y':1692},
	'prilazna_dole':{'x':1200, 'y':1550},
};
var positions_colored={'YELLOW':yellow_positions, 'GREEN':green_points};

var orientations_colored = // TO_EDIT
{
	'YELLOW':
	{
		'prilazna_gore':180,
		'prilazna_dole':90,
	},
	'GREEN':
	{
		'prilazna_gore':0,
		'prilazna_dole':90,
	}
};

Config.setup = function()
{
	Config['positions'] = positions_colored[Config.color];
	Config['orientations'] = orientations_colored[Config.color];
	
	if(Config.color == 'YELLOW') 
	{
		Config['primary_lift'] = 'LiftRight';
		Config['secondary_lift'] = 'LiftLeft';
	}
	else // GREEN
	{
		Config['primary_lift'] = 'LiftLeft';
		Config['secondary_lift'] = 'LiftRight';
	}
	
	Config['lift'] = Config.primary_lift;
}

function onRun()
{
	Logger.debug('running task: valjak gore ugao');
	
	if(Lift.has_room_for(1, Config.primary_lift)) Config['lift'] = Config.primary_lift; // ako ima mesta za primarni ok
	else Config['lift'] = Config.secondary_lift; // inace stavi onaj drugi
	
	// podesi prilazne tacke
	if(Config.color == 'YELLOW')
	{
		if(Config.lift == 'LiftRight')
		{
			var prilazna = Config.positions.prilazna_gore;
			var orientation = Config.orientations.prilazna_gore;
			var direction = 1;
			var distance = distance_gore; // TO_EDIT
		}
		else // LiftLeft
		{
			var prilazna = Config.positions.prilazna_dole;
			var orientation = Config.orientations.prilazna_dole;
			var direction = -1;
			var distance = distance_dole; // TO_EDIT
		}
	}
	else // GREEN
	{
		if(Config.lift == 'LiftRight')
		{
			var prilazna = Config.positions.prilazna_dole;
			var orientation = Config.orientations.prilazna_dole;
			var direction = -1;
			var distance = distance_dole; // TO_EDIT
		}
		else // LiftLeft
		{
			var prilazna = Config.positions.prilazna_gore;
			var orientation = Config.orientations.prilazna_gore;
			var direction = 1;
			var distance = distance_gore; // TO_EDIT
		}
	}
	
	CommandChain(Commands.pf_move(prilazna, direction)) // pridji
	.then(new RotateTo(orientation))
	.catch(Commands.wake_up_after(7000, check_ready)) // ako se desi nesto suspenduj se na 7s
	.then(new ActuatorCommand(Config.lift,'StartGetting')) // pokupi
	.then(new SetSpeedMotion(120))
	.then(new MoveForward(distance))
	.then(new SleepCommand(1100))
	//.then(new ActuatorCommand(Config.lift,'StopGetting'))
	.then(new MoveForward(-distance)) // odmakni se
	.then(new SetSpeedMotion(Config.default_speed))
	.then(new ActuatorCommand(Config.lift,'StopGetting'))
	.then(Commands.finish_task)
	.execute();
}

function check_ready()
{
	if(!Task.sleeping)
	{
		Manager.updateState("Ready");
	}
	else
	{
		Manager.updateState("Suspended");
	}
}

Lift.subscribe();

Notification.subscribe("LiftProgressNotification", function(msg)
{Logger.debug('LiftProgressNotification se desio: '+msg.type);
	if(Task.running && msg.type == 0) // video senzor (0 - vidi senzor, 1 - uvatio valjak)
	{
		CommandChain(new ActuatorCommand(Config.lift,'StopGetting'))
		.then(new StopMovement())
		.execute();
	}
});

function onPause()
{
	CommandChain(new ActuatorCommand(Config.lift,'StopGetting')).execute();
}

Manager.updateState("Ready");