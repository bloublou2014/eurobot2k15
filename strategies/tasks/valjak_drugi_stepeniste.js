/**
	Kupi drugi valjak kod stepenista.
*/

var distance_prilaz = 140; // TO_EDIT
var distance_kupljenje = 100; // TO_EDIT

var pos_y = 1705; // TO_EDIT
var pos_x = 932; // TO_EDIT

var position_colored = // TO_EDIT
{
	'YELLOW':{'x':-pos_x, 'y':pos_y},
	'GREEN':{'x':pos_x, 'y':pos_y},
};

var orientation_colored = // TO_EDIT
{
	'YELLOW':17,
	'GREEN':163 // TODO
};

function setup()
{
	Config['prilazna'] = position_colored[Config.color];
	Config['orientation'] = orientation_colored[Config.color];

	if(Config.color == 'YELLOW') Config['lift'] = 'LiftLeft';
	else Config['lift'] = 'LiftRight';
}

var succeeded = false;

function onRun(){
	
	Config.do_setup(setup);
	
	CommandChain(new MoveToPosition(Config.prilazna.x, Config.prilazna.y))
	.then(new RotateTo(Config.orientation))
	.then(new SetSpeedMotion(50))
	.then(new MoveForward(distance_prilaz))
	.then(new RotateTo(270))
	.then(new ActuatorCommand(Config.lift,'StartGetting'))
	.then(new MoveForward(distance_kupljenje))
	.success(function()
	{
		succeeded = true;
	})
	.then(new SetSpeedMotion(Config.default_speed))
	.then(new SleepCommand(2000))
	.then(new ActuatorCommand(Config.lift,'StopGetting'))
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
