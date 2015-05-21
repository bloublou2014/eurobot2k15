/**
	Na kraju meca.
*/

var lift_counter = 0;

function onRun()
{
	CommandChain(new StopMovement()).execute();
	CommandChain(new ActuatorCommand('LiftLeft','Unload')).then(move_back).execute();
	CommandChain(new ActuatorCommand('LiftRight','Unload')).then(move_back).execute();
	CommandChain(new ActuatorCommand('LiftCenter','Unload')).then(move_back).execute();
	CommandChain(new ActuatorCommand('EnemyDetector','StopBrkon')).execute();
}

function move_back()
{
	lift_counter++;
	if(lift_counter >= 3)
	{
		CommandChain(new MoveForward(-50))
		.then(new StopMovement(false))
		.execute();
	}
}

function onPause(){}

Manager.updateState("Ready");