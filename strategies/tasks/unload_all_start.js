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

Config.setup = function()
{
	Config['prilazna'] = position_colored[Config.color];
	Config['orientation'] = orientation_colored[Config.color];
}

var distance = 350;
var unloaded = false;

function onRun()
{
	CommandChain(new ActuatorCommand('LiftLeft','StopGetting'))
	.then(new ActuatorCommand('LiftRight','StopGetting'))
	.then(new GetMotionState())
	.then(function(msg)
	{
		if(!(Math.abs(msg.x > 750) && msg.y > 780 && msg.y < 1210))
		{
			this.then(Commands.pf_move(Config.prilazna));
		}
	})
	//.then(Commands.pf_move(Config.prilazna)) // pridji
	.then(new RotateTo(Config.orientation))
	.then(new MoveForward(distance)) // udji u centar
	.catch(Commands.wake_up_after(7000, check_ready))
	.then(new ActuatorCommand('LiftCenter','Unload')) // istovari casu
	.then(Commands.set_world_state('nosi_casu', 'false'))
	.then(Commands.set_world_state('istovario_start', 'true'))
	.then(new SetSpeedMotion(70))
	.then(new MoveForward(-100)) // odmakni se
	.then(function(){unloaded=true;})
	.then(function() // istovari valjke
	{
		CommandChain(new ActuatorCommand('LiftLeft','Unload')).execute();
		CommandChain(new ActuatorCommand('LiftRight','Unload')).execute();
	})
	.then(new SleepCommand(1200))
	.then(new MoveForward(-300)) // odmakni se
	.then(new SetSpeedMotion(Config.default_speed))
	.then(Commands.finish_task)
	.execute();
}

function onPause(){}

function check_ready()
{
	if(!unloaded)
	{
		if(Lift.has_items_to_unload() && !Task.sleeping) Manager.updateState("Ready");
		else Manager.updateState("Suspended");
	}
}

Lift.unloading('LiftLeft');
Lift.unloading('LiftRight');
Lift.unloading('LiftCenter');
Lift.on_update(check_ready);

Manager.updateState("Ready");