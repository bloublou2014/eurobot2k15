/**
	Izvlacenje iz startne pozicije sa kupljenjem loptice.
*/

function onRun(){
	Logger.debug('running task: start move');
	
	Logger.debug(Manager.getColor());

	CommandChain(new ActuatorCommand('Popcorn', 'Unload')).execute();
	CommandChain(new SleepCommand(500))
	.then(new MoveForward(-15))
	.then(new SleepCommand(1200))
	.then(new MoveForward(15))
	.then(new SleepCommand(2000)) // 2000
	.then(new ActuatorCommand('EnemyDetector', 'StartSensor'))
	.then(new MoveForward(350)) // TO_EDIT // 350
	.then(Commands.finish_task)
	.execute();
}

function onPause(){}

Manager.updateState("Ready");
