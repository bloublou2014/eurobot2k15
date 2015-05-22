/**
	Test path findinga.
*/

function onRun()
{
	Logger.debug('running task: test path finder');
	
	//Logger.debug(Manager.getColor());
	/*
	CommandChain(new RotateTo(0))
	.then(new ActuatorCommand('LiftLeft', 'StartGetting'))
	.then(new MoveForward(50))
	.then(new SleepCommand(5000))
	.then(new ActuatorCommand('LiftLeft', 'StopGetting'))
	.execute()
	*/
	//CommandChain(new AddStaticObject([new Point2D(-500,700),new Point2D(-500,1300),new Point2D(500,1300),new Point2D(500,700)]))
	CommandChain(new MoveToPosition(-900, 400, 1, true))
	// .then(new MoveToPosition(1200, -400, 1, true))
	// .then(new MoveToPosition(1200, 1600, -1, true))
	// .then(new MoveToPosition(-1200, 400, 1, true))
	// .then(new MoveToPosition(1200, 400, -1, true))
	// .then(new MoveToPosition(-1200, 1600, 1, true))
	// .then(new MoveToPosition(1200, 1600, -1, true))
	// .then(new MoveToPosition(-1200, 600, 1, true))
	// .then(new MoveToPosition(1200, 1400, -1, true))
	.then(Commands.finish_task)
	.execute();
}

function onPause(){}

Manager.updateState("Ready");
