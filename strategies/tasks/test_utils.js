/**
	Test kretanja.
*/

function onRun()
{
	Logger.debug('running task: test kretanja');
	
	CommandChain(new SetSpeedMotion(100))
	.then(new SetPosition(0, 0, 0)) // x, y, o
	.then(new MoveForward(500))
	/*.then(function()
	{
		Logger.debug('111111111111111111111111111111111');
		return 'message';
	})
	.then(function(msg)
	{
		Logger.debug('!!!!!!!!!!!!msg!!!!!!!!!!!!!!');
		Logger.debug(msg);
	})
	.then(new SleepCommand(2000))
	.then(new MoveForward(-200))
	.then(new GetMotionState())
	.then(function(msg)
	{
		if(msg.x < 1000)
		{
			throw 'command_error';
		}
	})
	.ignore_failure()*/
	/*.failure(function()
	{
		Logger.debug('failure command_error!!!!!!!!!!!!!!!!!!!!!!!!!!!s');
		return 'continue';
	})*/
	/*.then(new RotateTo(90))
	.then(new MoveToPosition(0,0))
	.then(new RotateTo(0))
	.then(new MoveForward(500))
	.then(new MoveForward(-500))
	.catch(function()
	{
		Logger.debug('catch!!!!!!!!!!!!!!!!!!!!!!!!!!!s');
		this
		.then(new SleepCommand(3000));
		//return 'continue';
	})*/
	.then(new MoveForward(-500))
	.then(Commands.wake_up_after(3000, function()
	{
		Logger.debug('wake_up_after awoken1!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!');
		Manager.updateState("Ready");
	}))
	.execute();
}

function onPause(){}

Manager.updateState("Ready");
