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
	else Config['lift'] = 'LiftLeft';
}

var succeeded = false;

function onRun(){
	
	Logger.debug('running task: valjak gore ugao');
	
	Config.do_setup(setup);
	
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
	
	CommandChain(new MoveToPosition(prilazna.x, prilazna.y))
	.then(new RotateTo(orientation))
	.then(new ActuatorCommand(Config.lift,'StartGetting'))
	.then(new SetSpeedMotion(50))
	.then(new MoveForward(distance))
	.success(function()
	{
		succeeded = true;
	})
	.then(new SleepCommand(700))
	.then(new ActuatorCommand(Config.lift,'StopGetting'))
	.then(new MoveForward(-distance))
	.then(new SetSpeedMotion(Config.default_speed))
	.success(function()
	{
		if(succeeded)
		{
			Manager.updateState("Finished");
		}
		else
		{
			Logger.debug('catch!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!');
			CommandChain(new SetSpeedMotion(Config.default_speed))
			.success(function()
			{
				Manager.updateState("Suspended");
			})
			.ignore_failure()
			.execute();
		}
	})
	.ignore_failure()
	.execute();
}

function onPause(){}

Manager.updateState("Ready");