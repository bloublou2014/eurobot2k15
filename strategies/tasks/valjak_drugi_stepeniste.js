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

Config.setup = function()
{
	Config['prilazna'] = position_colored[Config.color];
	Config['orientation'] = orientation_colored[Config.color];

	if(Config.color == 'YELLOW') Config['lift'] = 'LiftLeft';
	else Config['lift'] = 'LiftRight';
}

function onRun(){
	
	CommandChain(Commands.pf_move(Config.prilazna))
	.then(new RotateTo(Config.orientation))
	.then(new SetSpeedMotion(50))
	.then(new MoveForward(distance_prilaz))
	.then(new RotateTo(270))
	.then(new ActuatorCommand(Config.lift,'StartGetting'))
	.catch(error)
	.then(new MoveForward(distance_kupljenje))
	.then(new SetSpeedMotion(Config.default_speed))
	.then(new SleepCommand(2000))
	.then(new ActuatorCommand(Config.lift,'StopGetting'))
	.then(Commands.finish_task)
	.execute();
}

function error()
{
	Logger.debug('drugi valjak stepeniste error');
	CommandChain(new SetSpeedMotion(Config.default_speed))
	.then(Commands.suspend_task)
	.execute();
}

function onPause(){}

Manager.updateState("Ready");
