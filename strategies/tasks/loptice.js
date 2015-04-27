// I ostavie samo pored u komentaru stare vrednosti da ostane 

// Podesavanje 
// tacka na kojoj treba da se robot nalazi pre poravnavanja - sam podesava vrendnosti u odnosu na boju 
var color = 'YELLOW'; 
var position_X_prilazna = 1275; 
var position_Y_prilazna = 1650; 
var angle_porilzna = 0; //  

var udri_mujo_uzid = 40; 


// tacka u koju treba da dodje kad pre nego sto ose okrene, mozda bude i samo MoveTo ..  

var position_X_poravnavanje = 100; 
var position_Y_poravnavanje = 100; 


// distance koji treba da predje od zaglavljivanja do tacke gde se okrece 


var prilazna_colored = {

	'YELLOW':{'x':-position_X_prilazna, 'y':position_Y_prilazna},
	'GREEN':{'x':position_X_prilazna, 'y':position_Y_prilazna},
}; 

var prilazna_angle_colored = {

	'YELLOW':{'angle_porilzna':-angle_porilzna},
	'GREEN':{'angle_porilzna':angle_porilzna + 180},
}

var poravnavanja_colored = {

	'YELLOW':{'x':-position_X_poravnavanje, 'y':position_Y_poravnavanje},
	'GREEN':{'x':position_X_poravnavanje, 'y':position_Y_poravnavanje},
}; 

function setup(){
	Config['prilazna_position'] = prilazna_colored[Config.color]; 
	Config['poravnavanja'] = poravnavanja_colored[Config.color]; 
	Config['prilazna_ugao'] = prilazna_angle_colored[Config.color]; 
}; 

function onRun(){
		Config.do_setup(setup);
		CommandChain( new MoveToPosition(Config.prilazna_position.x, Config.prilazna_position.y, -1))
		.then( new RotateTo(Config.prilazna_ugao.angle_porilzna))
		.then(new SetSpeedMotion(udri_mujo_uzid))
		.catch(error)
		.then (new MoveForward(-400))
		.failure(onStuck)
		.execute(); 
}; 

function onStuck(){
	CommandChain(new MoveForward (313))
	.then(new RotateTo(-90))
	.catch(error)
	.then(new MoveForward(-400))
	.failure(onGetPopcorn)
	.execute(); 
}; 

function error(){
	Logger.debug('PUKLO'); 
	Manager.updateState("Suspended");

};

function onGetPopcorn(){
	CommandChain(new ActuatorCommand('Popcorn','Get'))
	.then( new MoveForward(200))
	.then( new SetSpeedMotion(Config.default_speed))
	.then(onFinish)
	.execute()
}

function onFinish(){
	Manager.updateState("Finished");
}; 


function onPause(){
	Loger.debug("PUASE");
}; 


Manager.updateState("Ready");
