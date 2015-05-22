/**
	:D ROFLCOPTER
	Ima dve varijante:
		1. strategija 1, odgura dva valjka do crvenog postolja, ne koristi druge mehanizme, 
		nije dobro ako senzori izignorisu protivnika blizu ivice, koristiti za slucaj da neradi mehanizam kupljenje valjaka.
		2. strategija 2, pokupi jedan valjak i istovari ga na start.
*/

var strategija = 'DRUGA'; // ovde promeniti strategiju: moze PRVA ili DRUGA

var yellow_positions_strat = 
{
	'PRVA':
	{
		'prva':{'x':-800, 'y':1000},
		'druga':{'x':-320, 'y':320},
	},
	'DRUGA':
	{
		'prva':{'x':-800, 'y':1000},
		'druga':{'x':-700, 'y':550},
		'treca':{'x':-900, 'y':1000},
	},
}

var yellow_positions = yellow_positions_strat[strategija];

var green_points = Motion.invert_x(yellow_positions);
var positions_colored={'YELLOW':yellow_positions, 'GREEN':green_points};

Config.setup = function()
{
	Config['positions'] = positions_colored[Config.color];
}

function onRun()
{
	CommandChain(new SetSpeedMotion(60))
	//.then(SleepCommand(5000)) // da ceka da mali izadje
	.then(function()
	{
		if(strategija == 'PRVA')
		{
			this.then(prva_move1);
		}
		else
		{
			this.then(druga_move1);
		}
	})
	.execute();
}

function prva_move1() // strategija 1 ide do crvenog postolja odgura 2 valjka odmakne se nazad
{
	CommandChain(new SleepCommand(500))
	.then(new MoveToPosition(Config.positions.prva.x, Config.positions.prva.y))
	.then(prva_move2)
	.catch(prva_move1)
	.execute();
}

function prva_move2()
{
	CommandChain(new SleepCommand(500))
	.then(new MoveToPosition(Config.positions.druga.x, Config.positions.druga.y))
	.then(prva_move3)
	.catch(prva_move2)
	.execute();
}

function prva_move3()
{
	CommandChain(new SleepCommand(500))
	.then(new MoveToPosition(Config.positions.prva.x, Config.positions.prva.y, -1))
	.then(Commands.finish_task)
	.catch(prva_move3)
	.execute();
}


function druga_move1() // strategija 2 ode pokupi 1 valjak istovari na centar
{
	CommandChain(new SleepCommand(500))
	.then(new MoveToPosition(Config.positions.prva.x, Config.positions.prva.y))
	.then(druga_move2)
	.catch(druga_move1)
	.execute();
}

function druga_move2()
{
	CommandChain(new SleepCommand(500))
	.then(function(){Logger.debug('starting lift get!!!!!!!!');})
	.then(new ActuatorCommand('LiftLeft', 'StartGetting'))
	.then(new ActuatorCommand('LiftRight', 'StartGetting'))
	.then(function(){Logger.debug('started lift get!!!!!!!!');})
	.then(new MoveToPosition(Config.positions.druga.x, Config.positions.druga.y))
	.then(new SleepCommand(3000))
	.then(new ActuatorCommand('LiftLeft', 'StopGetting'))
	.then(new ActuatorCommand('LiftRight', 'StopGetting'))
	.then(druga_move3)
	.catch(druga_move2)
	.execute();
}

function druga_move3()
{
	CommandChain(new SleepCommand(500))
	.then(new MoveToPosition(Config.positions.treca.x, Config.positions.treca.y))
	.then(new ActuatorCommand('LiftLeft', 'Unload'))
	.then(new ActuatorCommand('LiftRight', 'Unload'))
	.then(new SleepCommand(1000))
	.then(druga_move4)
	.catch(druga_move3)
	.execute();
}

function druga_move4()
{
	CommandChain(new SleepCommand(500))
	.then(new MoveToPosition(Config.positions.druga.x, Config.positions.druga.y, -1))
	.then(Commands.finish_task)
	.catch(druga_move4)
	.execute();
}

function onPause(){}

Manager.updateState("Ready");
