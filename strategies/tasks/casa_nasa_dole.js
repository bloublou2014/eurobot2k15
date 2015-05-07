
// Pokupi casu i valjak

var color = 'YELLOW';

var distance = 200;

var distance_casa = 200; // djoka 
var distance_valjak = 630; 
var distance_valjak2 = 130;  

var x_casa = 1289;
var y_casa = 540;
var A_casa = -90;  

var x_valjak = 1235; //- 
var y_valjak = 360;
var A_valjak = -99;

var x_valjak_2 = 844;
var y_valjak_2 = 936;
var A_valjak_2 = -68 ;

// time and speed CONFIG 

var delay_get_valjak = 900;
var delay_get_valjak2 = 800;


var delay_get_casa = 2000;
var speed_getting_object1 = 50; 
var speed_getting_object  = 20;

var distance_colored = {
	'YELLOW' : 200, 
	'GREEN' : 205
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
		'A_valjak':A_valjak,
		'A_casa':A_casa,
		'A_valjak_2':A_valjak_2
	},
	'GREEN':
	{
		'A_valjak': -81, // TODO 
		'A_casa': -90, // TODO
		'A_valjak_2': 20 // TODO
	}
};


function setup(){
	Config['positions_case'] = position_colored_casa[Config.color]; 
	Config['positions_valjak'] = positions_colored_valjak[Config.color]; 
	Config['positions_valjak2'] = positions_colored_valjak2[Config.color]; 
	Config['orientations'] = orientations_colored[Config.color];
	Config['distance'] = distance_colored[Config.color]; 
	if(Config.color == 'YELLOW') Config['lift'] = 'LiftRight';
	else Config['lift'] = 'LiftLeft';
}

function onRun(){

	Config.do_setup(setup);

	CommandChain(new MoveToPosition(Config.positions_case.x, Config.positions_case.y, -1))
	.then(new ActuatorCommand('LiftCenter','Unload'))
	.then(new RotateTo(Config.orientations.A_casa))
	.then(new SetSpeedMotion(speed_getting_object1))
	.then(new ActuatorCommand(Config.lift,'StartGetting'))
	.then(new MoveForward(Config.distance))
	.then(new ActuatorCommand(Config.lift,'StopGetting'))
	.then(new SleepCommand(delay_get_valjak))
	.then(new ActuatorCommand(Config.lift,'StartGetting'))
	.then(new RotateTo(Config.orientations.A_valjak))
	.catch(error)
	.then(new MoveForward(distance_valjak2))
	.failure(sucessStuck)
	.success(sucessStuck)
	.execute(); 

}


function onPause(){}

Manager.updateState("Ready");

function error(){
	Logger.debug("PUKLO"); 
	Manager.updateState("Suspended");
}

function sucessStuck(){
	CommandChain(new ActuatorCommand('LiftCenter','Get'))
	.then(new ActuatorCommand(Config.lift,'StopGetting'))
	.then(new SleepCommand(delay_get_valjak2))
	.then(new ActuatorCommand('LiftRight','StopGetting'))
	.then(new SetSpeedMotion(Config.default_speed))
	.then(new MoveForward(-300))
	.ignore_failure()
	.success(function(){
			Manager.updateState("Finished");
	})
	.execute(); 
	
}












