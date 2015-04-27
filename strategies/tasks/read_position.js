/**
	Cita poziciju i ispisuje.
*/
var count=0;
function onRun()
{
	CommandChain(new ActuatorCommand("Flap","KickLeft"))
	.then(new ActuatorCommand("Flap",'KickRight'))
	.then(new StopMovement(false))
	.success(function()
	{
		print_position();
	})
	.execute();
}

function print_position()
{
	CommandChain(new SleepCommand(200))
	.then(new GetMotionState())
	.success(function(msg)
	{
		Logger.debug('x='+msg.x+', y='+msg.y+', o='+msg.orientation);
		Logger.debug('count='+count++);
		print_position();
	})
	.execute();
}

function onPause(){}

Manager.updateState("Ready");