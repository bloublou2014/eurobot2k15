/**
	Istovara loptice.
*/

var pos_y = 1000; // TO_EDIT
var pos_x = 800; // TO_EDIT

var position_colored = 
{
	'YELLOW':{'x':-pos_x, 'y':pos_y},
	'GREEN':{'x':pos_x, 'y':pos_y},
};

var orientation_colored = // TO_EDIT
{
	'YELLOW':0,
	'GREEN':180,
};

Config.setup = function()
{
	Config['prilazna'] = position_colored[Config.color];
	Config['orientation'] = orientation_colored[Config.color];
}

var distance = 480; // 500 je previse

function onRun()
{
	CommandChain(Commands.pf_move(Config.prilazna))
	.then(new RotateTo(Config.orientation))
	.catch(Commands.ready_after(7000))
	.then(new MoveForward(-distance))
	.then(new ActuatorCommand('Popcorn','Unload'))
	.then(Commands.set_world_state('istovario_loptice', 'true'))
	//.then(new SetSpeedMotion(70))
	.then(new MoveForward(400))
	//.then(new SetSpeedMotion(Config.default_speed))
	.then(Commands.finish_task)
	.execute();
}

function onPause()
{
	//Logger.debug("PUASE");
	CommandChain(new SetSpeedMotion(Config.default_speed))
	.execute();
}; 

Manager.updateState("Ready");