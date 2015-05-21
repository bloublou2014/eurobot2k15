// I ostavie samo pored u komentaru stare vrednosti da ostane 

// Podesavanje 
// tacka na kojoj treba da se robot nalazi pre poravnavanja - sam podesava vrendnosti u odnosu na boju 
var position_X_prilazna = 1240; 
var position_Y_prilazna = 1650; 
var angle_porilzna = 0; //  

// distance koji treba da predje od zaglavljivanja do tacke gde se okrece 


var prilazna_colored = {

	'YELLOW':{'x':-position_X_prilazna, 'y':position_Y_prilazna},
	'GREEN':{'x':position_X_prilazna, 'y':position_Y_prilazna},
}; 

var prilazna_angle_colored = {

	'YELLOW':{'angle_porilzna':-angle_porilzna},
	'GREEN':{'angle_porilzna':angle_porilzna + 180},
}

Config.setup = function(){
	Config['prilazna_position'] = prilazna_colored[Config.color]; 
	Config['prilazna_ugao'] = prilazna_angle_colored[Config.color]; 
}; 

function onRun()
{
	CommandChain(new GetMotionState())
	.then(function(msg)
	{
		if(!(Math.abs(msg.x) > 880 && msg.y > 1500)) // ako je vec tamo nemora na prilaznu
		{
			this
			.then(new SetSpeedMotion(130))
			.then(Commands.pf_move(Config.prilazna_position));
		}
	})
	.then(new RotateTo(Config.prilazna_ugao.angle_porilzna))
	.then(new SetSpeedMotion(40))
	.catch(Commands.ready_after(7000))
	.then(new MoveForward(-1000)) // poravnaj se
	.then(new GetMotionState())
	.then(function(msg)
	{
		if(Config.color == 'YELLOW') var new_x = -1360; // YELLOW
		else var new_x = 1360; // GREEN
		this.then(new SetPosition(new_x, msg.y, msg.orientation));
	})
	.then(new SetSpeedMotion(130))
	.then(new MoveForward(313)).failure(Commands.ready_after(7000))
	.then(new RotateTo(-90))
	.then(new SetSpeedMotion(30))
	.then(new MoveForward(-400)) // idi do kokica
	.then(new MoveForward(4))
	.then(new ActuatorCommand('Popcorn','Get')) // pokupi
	.then(Commands.set_world_state('pokupio_loptice', 'true'))
	.then(new SetSpeedMotion(Config.default_speed))
	.then(new MoveForward(200))
	.then(Commands.finish_task)
	.execute();
};

function onPause()
{
	//Logger.debug("PUASE");
	/*CommandChain(new SetSpeedMotion(Config.default_speed))
	.execute();*/
}; 

Manager.updateState("Ready");
