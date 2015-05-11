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

function onRun(){
	
	Config.do_setup(setup);
	
	CommandChain(new MoveToPosition(Config.prilazna.x, Config.prilazna.y))
	.then(new RotateTo(Config.orientation))
	.then(new MoveForward(distance))
	.success(function()
	{
		CommandChain(new ActuatorCommand('LiftCenter','Unload'))
		.then(new SetSpeedMotion(70))
		.then(new MoveForward(-100))
		.success(function()
		{
			CommandChain(new ActuatorCommand('LiftLeft','Unload')).execute();
			CommandChain(new ActuatorCommand('LiftRight','Unload')).execute();
			
			CommandChain(new SleepCommand(1000))
			.then(new MoveForward(-200))
			.then(new SetSpeedMotion(Config.default_speed))
			.success(function()
			{
				Manager.updateState("Finished");
			})
			.ignore_failure()
			.execute();
		})
		.ignore_failure()
		.execute();
	})
	.catch(function()
	{
		Manager.updateState("Suspended");
	})
	.execute();
}

function onPause(){}

Manager.updateState("Ready");