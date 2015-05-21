
/**
	Kupi tri valjka na sredini sa lopticom.
*/

var lift_colored = 
{
	'GREEN':{'first':'LiftLeft', 'second':'LiftRight'},
	'YELLOW':{'first':'LiftRight', 'second':'LiftLeft'},
}

var positions_colored = 
{
	'GREEN':
	{
		'kupi_prvi':{'x':258, 'y':747},
		'istovari_loptu':{'x':532, 'y':677},
		'kupi_drugu':{'x':633, 'y':734},
		'kupi_trecu':{'x':397, 'y':383},
	},
	'YELLOW':
	{
		'kupi_prvi':{'x':-258, 'y':747},
		'istovari_loptu':{'x':-532, 'y':677},
		'kupi_drugu':{'x':-633, 'y':734},
		'kupi_trecu':{'x':-397, 'y':383},
	},
}

Config.setup = function()
{
	Config['lift'] = lift_colored[Config.color];
	Config['positions'] = positions_colored[Config.color];
}

function onRun()
{
	Logger.debug('running task: tri valjka');
	
	CommandChain(new ActuatorCommand('LiftLeft', 'StartGetting'))
	.then(new ActuatorCommand('LiftRight', 'StartGetting'))
	.then(new MoveToPosition(Config.positions.kupi_prvi.x, Config.positions.kupi_prvi.y))
	.then(new SleepCommand(1200))
	.then(new SetSpeedMotion(70))
	.then(new MoveToPosition(Config.positions.istovari_loptu.x, Config.positions.istovari_loptu.y, -1))
	//.then(new MoveForward(6))
	.then(function()
	{
		CommandChain(new ActuatorCommand('Popcorn', 'Unload')).execute();
	})
	.then(new SleepCommand(1500))
	.then(new SetSpeedMotion(Config.default_speed))
	.then(new MoveForward(70))
	.then(new ActuatorCommand('LiftRight','Unload'))
	.then(new MoveToPosition(Config.positions.kupi_drugu.x, Config.positions.kupi_drugu.y))
	.then(new SleepCommand(700))
	.then(new MoveToPosition(Config.positions.kupi_trecu.x, Config.positions.kupi_trecu.y))
	.then(new SleepCommand(700))
	.then(Commands.finish_task)
	.execute();
}

function onPause()
{
	CommandChain(new ActuatorCommand('LiftLeft', 'StopGetting'))
	.then(new ActuatorCommand('LiftRight', 'StopGetting'))
	.execute();
}

Manager.updateState("Ready");

