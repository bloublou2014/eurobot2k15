/**
	Izvlacenje iz startne pozicije. start_move_1000
*/

function onRun(){
	//Logger.debug('running task: start move');
	
	//Logger.debug(Manager.getColor());
	/*
	CommandChain(new RotateTo(0))
	.then(new ActuatorCommand('LiftLeft', 'StartGetting'))
	.then(new MoveForward(50))
	.then(new SleepCommand(5000))
	.then(new ActuatorCommand('LiftLeft', 'StopGetting'))
	.execute()
	*/
	CommandChain(new MoveForward(650)) // zabag
	.then(new RotateTo(90))
	.then(new MoveForward(150))
	.then(Commands.finish_task)
	.execute();
	/*CommandChain(Commands.pf_move({'x':800,'y':1000}))
	.then(Commands.finish_task)
	.execute();*/
}

function onPause(){}

Manager.updateState("Ready");
