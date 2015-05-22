/**
	Test kretanja. test_kretanja
*/

function onRun()
{
	Logger.debug('running task: test kretanja');
	
	CommandChain(new ActuatorCommand('Popcorn','Unload'))
	.then(new SleepCommand(10000))
	/*
	CommandChain(new SetSpeedMotion(150))
	
	.then(new RotateTo(0))
	.then(new RotateTo(90))
	.then(new RotateTo(180))
	.then(new RotateTo(270))
	.then(new RotateTo(0))
	.then(new RotateTo(90))
	.then(new RotateTo(180))
	.then(new RotateTo(270))
	.then(new RotateTo(0))
	.then(new SetSpeedMotion(20))
	.then(new MoveForward(-400))
*/
	
	/*
	.then(new SetPosition(0, 0, 0)) // x, y, o
	.then(new MoveForward(500))
	.then(new MoveForward(-500))
	.then(new RotateTo(45))
	.then(new RotateTo(0))
	.then(new RotateTo(90))
	.then(new RotateTo(180))
	.then(new RotateTo(270))
	.then(new RotateTo(0))
	.then(new RotateFor(45))
	.then(new RotateFor(-90))
	.then(new RotateFor(45))
	.then(new MoveToPosition(500, 0))
	.then(new MoveToPosition(500, 300))
	.then(new MoveToPosition(500, -100))
	.then(new MoveToPosition(500, 0))
	.then(new MoveToPosition(0, 0))
	.then(new RotateTo(0))*/
	.execute();
}

function onPause(){}

Manager.updateState("Ready");
