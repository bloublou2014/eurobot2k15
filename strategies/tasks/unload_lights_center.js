/**
	Istovara valjke na centar.
*/

var positions = // TO_EDIT
{
	'levo':{'x':-1275, 'y':111},
	'desno':{'x':-740, 'y':333}
};

var left_count = 0;
var right_count = 0;

var orientations = // TO_EDIT
{
	'levo':{'LiftRight':123, 'LiftLeft':234},
	'desno':{'LiftRight':123, 'LiftLeft':234}
};

var distance = 100; // TO_EDIT

var current_position = {'x':0, 'y':0};

function onRun(){
	
	Motion.update_current_status(function(){
		if(Motion.current_status.x > 0)
		{
			var side = 'desno';
		}
		else
		{
			var side = 'levo';
		}
		
		var lift = 'LiftRight'; // TODO
		
		CommandChain(new MoveToPosition(points[side].x, points[side].y))
		.then(new RotateTo(orientations[side][lift]))
		.then(new MoveForward(distance))
		.then(new ActuatorCommand(lift, 'Unload'))
		.success(function()
		{
			Manager.updateState("Finished");
		})
		.catch(function()
		{
			Manager.updateState("Suspended");
		})
		.execute();
	});
}

function onPause(){}

Manager.updateState("Ready"); // TODO