/**
	Odgura/obidje casu ispred naseg startnog polja.
*/

var distance = 150;  // TO_EDIT

var yellow_positions = // TO_EDIT
{
	's_nase':{'x':-820, 'y':1050},
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
	
	var side = 's_nase'; // hardcode
		
	var prilazna = Config.positions[side];
	
	CommandChain(new MoveToPosition(prilazna.x, prilazna.y))
	.then(Commands.finish_task)
	.execute();
}

function onPause(){}

Manager.updateState("Ready");