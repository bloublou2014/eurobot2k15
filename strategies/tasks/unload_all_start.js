/**
	Istovara sve na nasu startnu poziciju.
*/

var pos_y = 1000; // TO_EDIT
var pos_x = 700; // TO_EDIT

var position_colored = 
{
	'YELLOW':{'x':-pos_x, 'y':pos_y},
	'GREEN':{'x':pos_x, 'y':pos_y},
};

var orientation_colored = // TO_EDIT
{
	'YELLOW':180,
	'GREEN':0,
};

function setup()
{
	Config['prilazna'] = position_colored[Config.color];
	Config['orientation'] = orientation_colored[Config.color];
}

var distance = 300;

function onRun()
{
	Config.do_setup(setup);
	
	CommandChain(new MoveToPosition(Config.prilazna.x, Config.prilazna.y)) // pridji
	.then(new RotateTo(Config.orientation))
	.then(new MoveForward(distance)) // udji u centar
	.catch(Commands.suspend_task)
	.then(new ActuatorCommand('LiftCenter','Unload')) // istovari casu
	.then(new SetSpeedMotion(70))
	.then(new MoveForward(-100)) // odmakni se
	.then(function() // istovari valjke
	{
		CommandChain(new ActuatorCommand('LiftLeft','Unload')).execute();
		CommandChain(new ActuatorCommand('LiftRight','Unload')).execute();
	})
	.then(new SleepCommand(1000))
	.then(new MoveForward(-200)) // odmakni se
	.then(new SetSpeedMotion(Config.default_speed))
	.then(Commands.finish_task)
	.execute();
}

function onPause(){}

Manager.updateState("Ready");