/**
	Test kretanja.
*/

function onRun()
{
	Logger.debug('running task: test kretanja');
	
	CommandChain(new SleepCommand(5000))
	.then(Commands.ready_after(1000))
	.execute();
}

function onPause(){}

Manager.updateState("Ready");
