/**
	:D ROFLCOPTER
*/

var yellow_positions = // TO_EDIT
{
	'prva':{'x':-900, 'y':1000},
	'druga':{'x':-400, 'y':400},
};
var green_points = Motion.invert_x(yellow_positions);
var positions_colored={'YELLOW':yellow_positions, 'GREEN':green_points};

function setup()
{
	Config['positions'] = positions_colored[Config.color];
}

function onRun(){

	Config.do_setup(setup);

	CommandChain(new SetSpeedMotion(60))
	.then(move1)
	.execute();
}

function move1()
{
	CommandChain(new SleepCommand(500))
	.then(new MoveToPosition(Config.positions.prva.x, Config.positions.prva.y))
	.then(move2)
	.catch(move1)
	.execute();
}

function move2()
{
	CommandChain(new SleepCommand(2000))
	.then(new MoveToPosition(Config.positions.druga.x, Config.positions.druga.y))
	.then(Commands.finish_task)
	.catch(move2)
	.execute();
}

function onPause(){}

Manager.updateState("Ready");
