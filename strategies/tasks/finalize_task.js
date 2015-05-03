/**
	Na kraju meca.
*/

function onRun()
{
	CommandChain(new StopMovement())
	.then(new StopMovement(false))
	.execute();
	CommandChain(new ActuatorCommand('LiftLeft','Unload')).execute();
	CommandChain(new ActuatorCommand('LiftRight','Unload')).execute();
	CommandChain(new ActuatorCommand('LiftCenter','Unload')).execute();
}

function onPause(){}