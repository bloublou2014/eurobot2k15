/**
	Na kraju meca.
*/

function onRun()
{
	CommandChain(new StopMovement())
	.then(new StopMovement(false))
	.ignore_failure()
	.execute();
	CommandChain(new ActuatorCommand('LiftLeft','Unload')).execute();
	CommandChain(new ActuatorCommand('LiftRight','Unload')).execute();
	CommandChain(new ActuatorCommand('LiftCenter','Unload')).execute();
}

function onPause(){}