/**
	Istovara casu na polje kod protivnickog startnog polja.
*/

var pos_y = 450; // TO_EDIT
var pos_x = 450; // TO_EDIT

var position_colored = 
{
	'YELLOW':{'x':-pos_x, 'y':pos_y},
	'GREEN':{'x':pos_x, 'y':pos_y},
};

var orientation_colored = // TO_EDIT
{
	'YELLOW':45,
	'GREEN':135,
};

function setup()
{
	Config['prilazna'] = position_colored[Config.color];
	Config['orientation'] = orientation_colored[Config.color];
}

var distance = 100;

function onRun(){
	
	Config.do_setup(setup);
	
	CommandChain(new MoveToPosition(Config.prilazna.x, Config.prilazna.y))
	.then(new RotateTo(Config.orientation))
	.then(new MoveForward(distance))
	.success(function()
	{
		CommandChain(new ActuatorCommand('LiftCenter','Unload'))
		.then(new MoveForward(-distance))
		.success(function()
		{
			Manager.updateState("Finished");
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