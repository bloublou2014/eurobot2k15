// task koji uzima tri komada u isti lift da bi smo mogli poslagati 5 komada u spotlight

var angle_first_colored = {'GREEN':-143, 'YELLOW':-37};
var lift_colored = 
{
	'GREEN':{'lift':'LiftLeft'},
	'YELLOW':{'lift':'LiftRight'},
}


// pozicija na koju ide da pokupi prvi valjak 
var position_first_colored = 
{
	'GREEN':{'x':645, 'y':796},  
	'YELLOW':{'x':-645, 'y':796},
}
// pozicija na kuju ide da pokupo drigi valjak
var position_second_colored = 
{
	'GREEN':{'x':282, 'y':723},
	'YELLOW':{'x':-282, 'y':723},
}

var position_third_colored=
{
	'GREEN':{'x':274, 'y':359},
	'YELLOW':{'x':-274, 'y':359},
}



Config.setup = function()
{
	Config['angle_first'] = angle_first_colored[Config.color];
	Config['lift'] = lift_colored[Config.color];
	Config['position_first'] = position_first_colored[Config.color];
	Config['position_second'] = position_second_colored[Config.color]; 
	Config['position_third'] = position_third_colored[Config.color]; 
}

var retry_count = 0;


function onRun(){

	Logger.debug('running task: tri valjka jedan lift ( kula 5 )');
	
	CommandChain(new ActuatorCommand(Config.lift.lift,'StartGetting'))
	.then(new SetSpeedMotion(100))
	.then(new MoveToPosition(Config.position_first.x, Config.position_first.y))
	.then(new SleepCommand(500))
	//.then(new ActuatorCommand(Config.lift.lift, 'StopGetting'))
	.then(new MoveToPosition(Config.position_second.x, Config.position_second.y))
	.then(new SleepCommand(400))
	.then(new RotateTo(-90))
	.then(new MoveToPosition(Config.position_third.x, Config.position_third.y))
	//.then(new ActuatorCommand(Config.lift.lift, 'StartGetting'))
	.then(new SleepCommand(150)) //400 // 250
	.then(new ActuatorCommand(Config.lift.lift, 'StopGetting'))
	.then(new SetSpeedMotion(Config.default_speed))
	.then(Commands.finish_task)
	.execute(); 

} // end onRun


function onPause()
{
	CommandChain(new ActuatorCommand('LiftLeft', 'StopGetting'))
	.then(new ActuatorCommand('LiftRight', 'StopGetting'))
	.execute();
}


Manager.updateState("Ready");
