/**
	Kupi jedan valjak u gornjem uglu.
*/

var distance = 100; // TO_EDIT

var yellow_positions = // TO_EDIT
{
	'prilazna_gore':{'x':-1193, 'y':1692},
	'prilazna_dole':{'x':-1269, 'y':1602},
};

var green_points = // TO_EDIT
{
	'prilazna_gore':{'x':1200, 'y':1692},
	'prilazna_dole':{'x':1269, 'y':1602},
};
var positions_colored={'YELLOW':yellow_positions, 'GREEN':green_points};

var orientations_colored = // TO_EDIT
{
	'YELLOW':
	{
		'prilazna_gore':180,
		'prilazna_dole':102,
	},
	'GREEN':
	{
		'prilazna_gore':0,
		'prilazna_dole':123, // TODO
	}
};

function setup()
{
	Config['positions'] = positions_colored[Config.color];
	
	Config['orientations'] = orientations_colored[Config.color];
	
	if(Config.color == 'YELLOW') Config['lift'] = 'LiftRight';
	else Config['lift'] = 'LiftLeft'; // zakucan levi
}

var succeeded = false;

function onRun()
{
	Logger.debug('running task: valjak gore ugao');
	
	Config.do_setup(setup);
	
	// podesi prilazne tacke
	if(Config.color == 'YELLOW')
	{
		if(Config.lift == 'LiftRight')
		{
			var prilazna = Config.positions.prilazna_gore;
			var orientation = Config.orientations.prilazna_gore;
		}
		else // LiftLeft
		{
			var prilazna = Config.positions.prilazna_dole;
			var orientation = Config.orientations.prilazna_dole;
		}
	}
	else // GREEN
	{
		if(Config.lift == 'LiftRight')
		{
			var prilazna = Config.positions.prilazna_dole;
			var orientation = Config.orientations.prilazna_dole;
		}
		else // LiftLeft
		{
			var prilazna = Config.positions.prilazna_gore;
			var orientation = Config.orientations.prilazna_gore;
		}
	}
	
	CommandChain(new MoveToPosition(prilazna.x, prilazna.y)) // pridji
	.then(new RotateTo(orientation))
	.catch(Commands.wake_up_after(7000, check_ready)) // ako se desi nesto suspenduj se na 7s
	.then(new ActuatorCommand(Config.lift,'StartGetting')) // pokupi
	.then(new SetSpeedMotion(50))
	.then(new MoveForward(distance))
	.then(new SleepCommand(700))
	.then(new ActuatorCommand(Config.lift,'StopGetting'))
	.then(new MoveForward(-distance)) // odmakni se
	.then(new SetSpeedMotion(Config.default_speed))
	.then(Commands.finish_task)
	.execute();
}

function check_ready()
{
	if(!Task.sleeping /*&& Lift.has_room()*/)
	{
		Manager.updateState("Ready");
	}
	else
	{
		Manager.updateState("Suspended");
	}
}

function onPause(){}

Manager.updateState("Ready");