/**
	Istovara valjke na centar.
*/

var positions = // TO_EDIT
{
	'levo':{'x':-1275, 'y':111},
	'desno':{'x':-740, 'y':333}
};

var orientations = // TO_EDIT
{
	'levo':{'LiftRight':123, 'LiftLeft':234},
	'desno':{'LiftRight':123, 'LiftLeft':234}
};

var distance = 100; // TO_EDIT

var current_position = {'x':0, 'y':0};

function onRun()
{
	CommandChain(GetMotionState())
	.then(function(msg)
	{
		if(msg.x > 0)
		{
			var side = 'desno';
		}
		else
		{
			var side = 'levo';
		}
		
		var lift = 'LiftRight'; // TODO
		
		this
		.then(Commands.pf_move(points[side]))
		.then(new RotateTo(orientations[side][lift]))
		.then(new MoveForward(distance))
		.then(new ActuatorCommand(lift, 'Unload'))
		.catch(Commands.suspend_task)
		.then(new MoveForward(-distance));
	})
	.then(Commands.finish_task)
	.execute();
}

function onPause(){}

Manager.updateState("Ready"); // TODO