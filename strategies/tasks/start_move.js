/**
	Izvlacenje iz startne pozicije.
*/

function onRun(){
	Logger.debug('running task: start move');
	
	Logger.debug(Manager.getColor());
	/*
	CommandChain(new SleepCommand(5000))
	.then(new ActuatorCommand('LiftRight','StartGetting'))
	.then(new ActuatorCommand('LiftLeft','StartGetting'))
	.execute();*/
	
	CommandChain(new MoveForward(200)) // TO_EDIT
	.success(function()
	{
		Manager.updateState("Finished");
	})
	.ignore_failure()
	.execute();
}

function onPause(){}

Manager.updateState("Ready");
