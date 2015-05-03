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

function onRun()
{
	Config.do_setup(setup);
	
	CommandChain(new MoveToPosition(Config.prilazna.x, Config.prilazna.y))
	.then(new RotateTo(90))
	.then(new ActuatorCommand(Config.lift,'StartGetting'))
	.then(new SetSpeedMotion(50))
	.catch(error)
	.then(new MoveForward(distance))
	.then(new SleepCommand(2000))
	.then(new ActuatorCommand(Config.lift,'StopGetting'))
	.then(new MoveForward(-distance))
	.then(new SetSpeedMotion(Config.default_speed))
	.then(Commands.finish_task)
	.execute();
}

function error()
{
	Logger.debug('prvi valjak stepeniste error');
	CommandChain(new SetSpeedMotion(Config.default_speed))
	.then(Commands.suspend_task)
	.execute();
}

function onPause(){}

Manager.updateState("Ready");
