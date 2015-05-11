/**
	Kupi casu ispred naseg startnog polja.
*/

var distance = 130;  // TO_EDIT

var yellow_positions = // TO_EDIT
{
	's_nase':{'x':-780, 'y':1050},
	's_njihove':{'x':123, 'y':234},
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

function onRun(){
	
	Config.do_setup(setup);
	
	Motion.update_current_status(function()
	{
		/*if(Math.abs(Motion.current_status.x) > 123)
		{
			var side = 's_nase';
		}
		else
		{
			var side = 's_njihove';
		}*/
		var side = 's_nase'; // hardcode
		
		var prilazna = Config.positions[side];
		var orientation = Config.orientations[side];
		
		CommandChain(new MoveToPosition(prilazna.x, prilazna.y))
		.then(new RotateTo(orientation))
		.then(new SetSpeedMotion(50))
		.then(new MoveForward(distance))
		.then(new SetSpeedMotion(Config.default_speed))
		.then(new ActuatorCommand('LiftCenter','Get'))
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

Manager.updateState("Ready");