/**
	DEBEG
*/

function onRun()
{
	// valjak gore ugao
	//CommandChain(Commands.pf_move({'x':1200, 'y':1550}, -1)) // pridji
		//CommandChain(new MoveToPosition(1200,1550, -1)) // pridji // -1
	/*.then(new RotateTo(90))
	//.catch(Commands.wake_up_after(7000, check_ready)) // ako se desi nesto suspenduj se na 7s
	.then(new ActuatorCommand('LiftRight','StartGetting')) // pokupi
	.then(new SetSpeedMotion(50))
	.then(new MoveForward(170))
	.then(new SleepCommand(700))
	.then(new ActuatorCommand('LiftRight','StopGetting'))*/
	//.then(new MoveForward(-170)) // odmakni se
	//.then(new SetSpeedMotion(Config.default_speed))
	
	// loptice
	//.then(new SetSpeedMotion(120))
	//.then(Commands.pf_move({'x':1240, 'y':1650}))
		//.then(new MoveToPosition(1240, 1650))
	//.then(new RotateTo(180))
		//.then(new SetSpeedMotion(20)) // poludi brze
	//.then(new SleepCommand(3000))
		//.then(new MoveForward(-400)) // ovde poludi
	//.then(new MoveToPosition(1640, 1650))
	
	CommandChain(new MoveForward(100))
	.then(new SetSpeedMotion(20))
	.then(new MoveForward(-400))
	
	.then(Commands.finish_task)
	.execute();
}

function onPause(){}

Manager.updateState("Ready");
