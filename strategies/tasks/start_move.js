/**
	Izvlacenje iz startne pozicije.
*/

function onRun(){
	Logger.debug('running task: start move');
	Command.send(new MoveForward(1000),function(){
	Manager.updateState("Finished");
	}, function(){Logger.error('start move nije stigo de treba');});
}

function onPause(){}

Manager.updateState("Ready");