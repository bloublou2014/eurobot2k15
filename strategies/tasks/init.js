/**
	Inicijalna podesavanja.
*/

var color = 'GREEN';
var init_positions = {'YELLOW':{'x':-1090, 'y':1000, 'o':0}, 
					  'GREEN' :{'x':1090, 'y':1000 , 'o':180}};
var init_position = init_positions[color];
var init_speed = 100;

function internal_error()
{
	Logger.error("Internal error occured!!! nesto gadno nevalja..");
}

function onRun(){
	Logger.debug('running task: init task');
	Command.send(new SetSpeedMotion(init_speed), function(){
	Command.send(new SetPosition(init_position.x, init_position.y, init_position.o), function(){
	Manager.updateState("Finished");
	Logger.debug('initialization: speed='+init_speed+
	', x='+init_position.x+
	', y='+init_position.y+
	', o='+init_position.o);
	Command.send(new ActuatorCommand("EnemyDetector", "StartBrkon"), function(){
	Logger.debug('brkon started');
	}, function(){Logger.error('brkon nije started');});
	}, internal_error);
	}, internal_error);
}

function onPause(){}

Manager.updateState("Ready");