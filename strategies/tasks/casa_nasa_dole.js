
// Pokupi casu i valjak

var color = 'YELLOW';

var distance = 200;

var distance_casa = 200; // djoka 
var distance_valjak = 630; 
var distance_valjak2 = 130;  

var x_casa = 1270;
var y_casa = 540;

var x_valjak = 1235; //- 
var y_valjak = 360;

var x_valjak_2 = 1280; // 844
var y_valjak_2 = 240; // 240

// time and speed CONFIG 

var delay_get_valjak = 500; // 800
var delay_get_valjak2 = 500; // 900 // 700 


var delay_get_casa = 2000;
var speed_getting_object1 = 50; 
var speed_getting_object  = 20;

var distance_colored = {
	'YELLOW' : 230, // tome edit, old 220 
	'GREEN' : 230	// tome edit old 220
}; 

var position_colored_casa = 
{
	'YELLOW':{'x':-x_casa, 'y':y_casa},
	'GREEN':{'x':x_casa+1, 'y':y_casa},
};

var positions_colored_valjak = 
{
	'YELLOW':{'x':-x_valjak, 'y':y_valjak},
	'GREEN':{'x':x_valjak, 'y':y_valjak},
};

var positions_colored_valjak2 = 
{	
	'YELLOW':{'x':-x_valjak_2, 'y':y_valjak_2},
	'GREEN':{'x':x_valjak_2, 'y':y_valjak_2},
}; 


var orientations_colored = 
{
	'YELLOW':
	{
		'A_casa':-90,
	},
	'GREEN':
	{
		'A_casa': -90, // TODO
	}
};

var collected = 0; // kolko je skupio

Config.setup = function()
{
	Config['positions_case'] = position_colored_casa[Config.color]; 
	Config['positions_valjak'] = positions_colored_valjak[Config.color]; 
	Config['positions_valjak2'] = positions_colored_valjak2[Config.color]; 
	Config['orientations'] = orientations_colored[Config.color];
	Config['distance'] = distance_colored[Config.color]; 
	if(Config.color == 'YELLOW') Config['lift'] = 'LiftRight';
	else Config['lift'] = 'LiftLeft';
}

function onRun()
{
	Task.running=true;

	CommandChain(Commands.pf_move(Config.positions_case, Motion.BACKWARD))
	.then(new ActuatorCommand('LiftCenter','Unload'))
	.then(new RotateTo(Config.orientations.A_casa))
	.catch(Commands.wake_up_after(7000, check_ready))
	.then(Commands.set_world_state('tried_casa_nasa_dole', 'true'))
	.then(new SetSpeedMotion(speed_getting_object1))
	.then(new ActuatorCommand(Config.lift,'StartGetting'))
	.then(new MoveForward(Config.distance)) // TODO videti dal ovde moze da detektuje, da se prebaci catch
	.then(new ActuatorCommand(Config.lift,'StopGetting'))
	.then(new SleepCommand(delay_get_valjak))
	.then(new ActuatorCommand(Config.lift,'StartGetting'))
	.then(new MoveToPosition(Config.positions_valjak2.x, Config.positions_valjak2.y))
	.then(new ActuatorCommand(Config.lift,'StopGetting'))
	.then(new ActuatorCommand('LiftCenter','Get'))
	.then(Commands.set_world_state('nosi_casu', 'true'))
	.then(new SleepCommand(delay_get_valjak2))
	.then(new SetSpeedMotion(Config.default_speed))
	.then(new MoveForward(-100))
	.then(Commands.finish_task)
	.execute();
}

function onPause()
{
	Task.running=false;
	CommandChain(new SetSpeedMotion(Config.default_speed))
	.then(new ActuatorCommand(Config.lift,'StopGetting'))
	.execute();
}

function error()
{
	Logger.debug("PUKLO"); 
	Manager.updateState("Suspended");
}

function check_ready()
{
	if(Lift.has_room_for(2 - collected, Config.lift) && !Task.sleeping)
	{
		Logger.debug('Ready from check_ready running:'+Task.running);
		Task.ready();
	}
	else
	{
		Logger.debug('suspending from check_ready');
		Logger.debug('collected '+collected);
		Task.suspend();
	}
}

Notification.subscribe("LiftProgressNotification", function(msg)
{Logger.debug('LiftProgressNotification se desio: '+msg.type);
	if(Task.running && msg.type == 0) // video senzor (0 - vidi senzor, 1 - uvatio valjak)
	{
		collected++;
		/*Logger.debug('LiftProgressNotification se desio!!!!!!: '+msg.type);
		CommandChain(new ActuatorCommand(Config.lift,'StopGetting'))
		.then(new StopMovement())
		.execute();*/
	}
});

Lift.on_update(check_ready);

Manager.updateState("Ready");


