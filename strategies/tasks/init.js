/**
	Inicijalna podesavanja.
*/

function setup()
{
	var init_positions = {'YELLOW':{'x':-1100, 'y':1000, 'o':0}, 
						  'GREEN' :{'x':1100, 'y':1000 , 'o':180}};
	Config['init_position'] = init_positions[Config.color];
	Config['init_speed'] = Config.default_speed;
}

function internal_error()
{
	Logger.error("Internal error occured!!! nesto gadno nevalja..");
}

function onRun(){

	CommandChain(new ActuatorCommand('EnemyDetector','StopBrkon'))
	.execute(); 

	Config.do_setup(setup);

	Logger.debug('running task: init task');
	Command.send(new SetSpeedMotion(Config.init_speed), function(){
	Command.send(new SetPosition(Config.init_position.x, Config.init_position.y, Config.init_position.o), function(){
	Command.send(new ActuatorCommand("EnemyDetector", "StartBrkon"), function(){
	Logger.debug('brkon started');
	Logger.debug('initialization: speed='+Config.init_speed+
	', x='+Config.init_position.x+
	', y='+Config.init_position.y+
	', o='+Config.init_position.o);
	Manager.updateState("Finished");
	}, function(){Logger.error('brkon nije started');});
	}, internal_error);
	}, internal_error);
}

function onPause(){}

Manager.updateState("Ready");