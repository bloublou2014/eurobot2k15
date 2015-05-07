/**
	Kupi prvi valjak kod stepenista.
*/

var distance = 100; // TO_EDIT

var pos_x = 733; // TO_EDIT

var position_colored = 
{
	'YELLOW':{'x':-pos_x, 'y':1591},
	'GREEN':{'x':pos_x, 'y':1585},
};

function setup()
{
	Config['prilazna'] = position_colored[Config.color];

	if(Config.color == 'YELLOW') Config['lift'] = 'LiftRight';
	else Config['lift'] = 'LiftLeft';
}

var succeeded = false;

function onRun(){
	
	Config.do_setup(setup);
	
	CommandChain(new MoveToPosition(Config.prilazna.x, Config.prilazna.y))
	.then(new RotateTo(90))
	.then(new ActuatorCommand(Config.lift,'StartGetting'))
	.then(new SetSpeedMotion(50))
	.then(new MoveForward(distance))
	.success(function()
	{
		succeeded = true;
	})
	.then(new SleepCommand(2000))
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
