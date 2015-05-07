/**
	Udara klapnu sa njihove strane (jednu).
*/

var pos_y = 200; // TO_EDIT

var yellow_positions = // TO_EDIT
{
	'prilazna':{'x':1050, 'y':pos_y},
	'kraj':{'x':640, 'y':pos_y},
};
var green_points = Motion.invert_x(yellow_positions);
var positions_colored={'YELLOW':yellow_positions, 'GREEN':green_points};


var orientation_colored = {'YELLOW':180, 'GREEN':0};


var flap_colored = {'YELLOW':'Left', 'GREEN':'Right'};


function setup()
{
	Config['positions'] = positions_colored[Config.color];
	Config['orientation'] = orientation_colored[Config.color];
	Config['flap'] = flap_colored[Config.color];
}

function onRun(){

	Config.do_setup(setup);

	CommandChain(new MoveToPosition(Config.positions.prilazna.x, Config.positions.prilazna.y, -1))
	.then(new RotateTo(Config.orientation))
	.then(new ActuatorCommand("Flap","Kick"+Config.flap))
	.then(new MoveForward(200)) // TO_EDIT
	.then(new ActuatorCommand("Flap","Unkick"+Config.flap))
	.success(function()
	{
		Manager.updateState("Finished");
	})
	.catch(function()
	{
		Logger.error('error in klapna njihova');
		CommandChain(new ActuatorCommand("Flap","Unkick"+Config.flap)).execute();
		//Task.ready_after(7000);
		Manager.updateState("Suspended");
	})
	.execute();
}

function onPause(){}

Manager.updateState("Ready");