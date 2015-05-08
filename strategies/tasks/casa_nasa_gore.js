/**
	Kupi casu ispred naseg startnog polja.
*/

var distance = 130;  // TO_EDIT

var yellow_positions = // TO_EDIT
{
	's_nase':{'x':-780, 'y':1050},
	's_njihove':{'x':123, 'y':234}, // TODO set
};
var green_points = Motion.invert_x(yellow_positions);
var positions_colored={'YELLOW':yellow_positions, 'GREEN':green_points};

var orientations_colored = // TO_EDIT
{
	'YELLOW':
	{
		's_nase':30,
		's_njihove':123,
	},
	'GREEN':
	{
		's_nase':150,
		's_njihove':123,
	}
};

function setup()
{
	Config['positions'] = positions_colored[Config.color];
	Config['orientations'] = orientations_colored[Config.color];
}

var prilazna = null;
var orientation = null;

function onRun()
{	
	Config.do_setup(setup);
	
	CommandChain(new GetMotionState())
	.then(function(msg)
	{
		if(Math.abs(msg.x) > 123) // TODO set
		{
			var side = 's_nase';
		}
		else
		{
			var side = 's_njihove';
		}
		
		Config['prilazna'] = Config.positions[side];
		Config['orientation'] = Config.orientations[side];
	})
	.then(new MoveToPosition(Config.prilazna.x, Config.prilazna.y))
	.then(new RotateTo(Config.orientation))
	.catch(Commands.wake_up_after(7000, check_ready))
	.then(new SetSpeedMotion(50))
	.then(new MoveForward(distance))
	.then(new SetSpeedMotion(Config.default_speed))
	.then(new ActuatorCommand('LiftCenter','Get'))
	.then(Commands.finish_task)
	.catch(function()
	{
		CommandChain(new SetSpeedMotion(Config.default_speed))
		.then(Commands.suspend_task)
		.execute();
	})
	.execute();
}

function check_ready()
{
	if(Lift.has_room_for(1, 'LiftCenter') && !Task.sleeping) Manager.updateState("Ready");
	else Manager.updateState("Suspended");
}

Lift.on_update(check_ready);

Manager.updateState("Ready");

function onPause(){}
