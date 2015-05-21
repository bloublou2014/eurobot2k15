
/**
	Kupi tri valjka na sredini.
	Raspored sa zute:                                   Raspored sa zelene:
    G																		G
  D O=1           G                                         G			O=1   D               
				  D  O=2                                 O=2  D
		                                                	
		                                                	
		ns    nj                                        	     nj    ns
		   O=3                                          	       O=3
	G=gore, D=dole, ns=s_nase, nj=s_njihove
*/

var position_data =  { // TO_EDIT
	'YELLOW':{
		'prvi':{
			'gore':{
				'point':{'x':-634, 'y':843},
				'orientation':{'LiftLeft':-125,'LiftRight':-63} },
			'dole':{
				'point':{'x':-753, 'y':827},
				'orientation':{'LiftLeft':-84,'LiftRight':-27} } },
		'drugi':{
			'gore':{
				'point':{'x':-334, 'y':762},
				'orientation':{'LiftLeft':-85,'LiftRight':-26} },
			'dole':{
				'point':{'x':-376, 'y':450},
				'orientation':{'LiftLeft':12,'LiftRight':19} } },
		'treci':{
			's_nase':{
				'point':{'x':-493, 'y':421},
				'orientation':{'LiftLeft':-82,'LiftRight':-37} },
			's_njihove':{
				'point':{'x':-312, 'y':398},
				'orientation':{'LiftLeft':-151,'LiftRight':-88} } } },
	'GREEN':{
		'prvi':{
			'gore':{
				'point':{'x':634, 'y':843},
				'orientation':{'LiftLeft':243,'LiftRight':-55} },
			'dole':{
				'point':{'x':753, 'y':827},
				'orientation':{'LiftLeft':207,'LiftRight':264} } },
		'drugi':{
			'gore':{
				'point':{'x':334, 'y':762},
				'orientation':{'LiftLeft':206,'LiftRight':265} },
			'dole':{
				'point':{'x':376, 'y':450},
				'orientation':{'LiftLeft':161,'LiftRight':168} } },
		'treci':{
			's_nase':{
				'point':{'x':493, 'y':421},
				'orientation':{'LiftLeft':217,'LiftRight':262} },
			's_njihove':{
				'point':{'x':312, 'y':398},
				'orientation':{'LiftLeft':268,'LiftRight':-29} } } } };

var distance = 70; // distanca za koju ide pravo kad kupi valjak  // TO_EDIT
var redosled_kupljenja = [];
var counter = 0;
var tried = [];

function onRun()
{
	Task.running = true;
	Logger.debug('running task: tri valjka');
	
	counter = 0; // za svaki slucaj, mada nema smisla da radi dvaput
	
	//CommandChain(new SetSpeedMotion(50)) // tome
	CommandChain(new GetMotionState())
	.then(function(msg) // odredi redosled kupljenja
	{
		redosled_kupljenja = [];
		if(Math.abs(msg.x) > 750) // sa nase strane
		{
			Logger.debug('sa nase strane');
			//redosled_kupljenja = [config_prvi, config_treci, config_drugi]; // 1 3 2
			if(!has_tried('config_prvi')) redosled_kupljenja.push(config_prvi);
			if(!has_tried('config_treci')) redosled_kupljenja.push(config_treci);
			if(!has_tried('config_drugi')) redosled_kupljenja.push(config_drugi);
		}
		else // sa njihove strane
		{
			Logger.debug('sa njihove strane');
			//redosled_kupljenja = [config_drugi, config_prvi, config_treci]; // 2 1 3
			if(!has_tried('config_drugi')) redosled_kupljenja.push(config_drugi);
			if(!has_tried('config_prvi')) redosled_kupljenja.push(config_prvi);
			if(!has_tried('config_treci')) redosled_kupljenja.push(config_treci);
		}
		
		if(redosled_kupljenja.length == 0)
		{
			this.abort();
			Commands.finish_task();
		}
	})
	.then(call_next)
	.execute();
}

function has_tried(valjak)
{
	for(var i=0;i<tried.length;i++)
	{
		if(tried[i] == valjak) return true;
	}
	return false;
}

function onPause()
{
	Task.running=false;
	CommandChain(new ActuatorCommand('LiftLeft', 'StopGetting'))
	.then(new ActuatorCommand('LiftRight', 'StopGetting'))
	//.then(new SetSpeedMotion(Config.default_speed))
	.execute();
}

function config_prvi(msg) // konfigurisi za prvi
{
	if(msg.y > 1800)
	{
		var odakle = 'gore';
	}
	else
	{
		var odakle = 'dole';
	}
	
	if(Config.color == 'YELLOW') var lift = 'LiftRight'; // TODO  // TO_EDIT
	else var lift = determine_lift('LiftLeft'); // TO_EDIT
	
	Logger.debug('pokupi_prvi '+odakle+' '+lift);
	
	var prilazna = position_data[Config.color].prvi[odakle].point;
	var orientation = position_data[Config.color].prvi[odakle].orientation[lift];
	
	tried.push('config_prvi');
	
	return {'prilazna':prilazna, 'orientation':orientation, 'lift':lift};
}

function config_drugi(msg) // konfigurisi za drugi
{
	if(msg.y > 900)
	{
		var odakle = 'gore';
	}
	else
	{
		var odakle = 'dole';
	}
	
	if(Config.color == 'YELLOW') var lift = 'LiftLeft'; // TODO  // TO_EDIT
	else var lift = determine_lift('LiftRight'); // TO_EDIT
	
	Logger.debug('config_drugi '+odakle+' '+lift);
	
	var prilazna = position_data[Config.color].drugi[odakle].point;
	var orientation = position_data[Config.color].drugi[odakle].orientation[lift];
	Logger.debug('odakle:'+odakle+' lift:'+lift);
	Logger.debug('prilazna:'+prilazna.x+','+prilazna.y+' orientation:'+orientation+' lift:'+lift);
	
	tried.push('config_drugi');
	
	return {'prilazna':prilazna, 'orientation':orientation, 'lift':lift};
}

function config_treci(msg) // konfigurisi za treci
{
	if(Math.abs(msg.x) > 400)
	{
		var odakle = 's_nase';
	}
	else
	{
		var odakle = 's_njihove';
	}
	
	if(Config.color == 'YELLOW') var lift = 'LiftRight'; // TODO  // TO_EDIT
	else var lift = determine_lift('LiftLeft'); // TO_EDIT
	
	var prilazna = position_data[Config.color].treci[odakle].point;
	var orientation = position_data[Config.color].treci[odakle].orientation[lift];
	
	Logger.debug('config_treci '+odakle+' '+lift);
	
	tried.push('config_treci');
	
	return {'prilazna':prilazna, 'orientation':orientation, 'lift':lift};
}

function pokupi_valjak(valjak)
{
	Logger.debug('pokupi_valjak '+valjak.prilazna+' '+valjak.orientation+' '+valjak.lift+'!!!!!!!!!!!!!!!!!!!!!!!!!');
	this
	.then(new MoveToPosition(valjak.prilazna.x,valjak.prilazna.y, true)) // pridji
	.then(new RotateTo(valjak.orientation)) // rotiraj se
	.then(new ActuatorCommand(valjak.lift, 'StartGetting')) // kupi
	//.then(new SetSpeedMotion(50))
	.then(new MoveForward(distance))
	//.catch(Commands.ready_after(5000))
	//.then(new SetSpeedMotion(Config.default_speed))
	//.then(new SleepCommand(400))
	.then(new ActuatorCommand(valjak.lift, 'StopGetting'))
	.then(call_next); // predji na sledeci
}

function call_next()
{
	if(counter < redosled_kupljenja.length) // ako ima jos valjaka
	{
		this
		.then(new GetMotionState())
		.then(redosled_kupljenja[counter++]) // konfigurisi prilaznu
		.then(pokupi_valjak); // kupi sledeci
	}
	else // ako nema vise, zavrsi
	{
		Logger.debug('suspending from call_next');
		this
		.then(Commands.finish_task);
	}
}

function determine_lift(desired_lift)
{
	if(desired_lift == 'LiftLeft')
	{
		if(Lift.has_room_for(1, 'LiftLeft')) return 'LiftLeft';
		else return 'LiftRight';
	}
	else // LiftRight
	{
		if(Lift.has_room_for(1, 'LiftRight')) return 'LiftRight';
		else return 'LiftLeft';
	}
}

function check_ready()
{
	if(Lift.has_room_for(3-tried.length, 'LeftOrRight'))
	{
		Logger.debug('Ready from check_ready running:'+Task.running);
		Task.ready();
	}
	else
	{
		Logger.debug('suspending from check_ready');
		Task.suspend();
	}
}

Lift.on_update(check_ready);

Manager.updateState("Ready");

