
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
var redosled_kupljenja = [config_prvi, config_drugi, config_treci]; // default redosled
var counter = 0;

function onRun(){

	Logger.debug('running task: tri valjka');

	Config.do_setup();
	
	counter = 0; // za svaki slucaj, mada nema smisla da radi dvaput
	
	CommandChain(new SetSpeedMotion(50)) // tome
	.then(new GetMotionState())
	.then(function(msg) // odredi redosled kupljenja
	{
		if(Math.abs(msg.x) > 750) // sa nase strane
		{
			Logger.debug('sa nase strane');
			redosled_kupljenja = [config_prvi, config_treci, config_drugi]; // 1 3 2
		}
		else // sa njihove strane
		{
			Logger.debug('sa njihove strane');
			redosled_kupljenja = [config_drugi, config_prvi, config_treci]; // 2 1 3
		}
	})
	.then(call_next)
	.execute();
}

function onPause(){}

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
	else var lift = 'LiftLeft'; // TO_EDIT
	
	Logger.debug('pokupi_prvi '+odakle+' '+lift);
	
	var prilazna = position_data[Config.color].prvi[odakle].point;
	var orientation = position_data[Config.color].prvi[odakle].orientation[lift];
	
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
	else var lift = 'LiftRight'; // TO_EDIT
	
	Logger.debug('config_drugi '+odakle+' '+lift);
	
	var prilazna = position_data[Config.color].drugi[odakle].point;
	var orientation = position_data[Config.color].drugi[odakle].orientation[lift];
	Logger.debug('odakle:'+odakle+' lift:'+lift);
	Logger.debug('prilazna:'+prilazna.x+','+prilazna.y+' orientation:'+orientation+' lift:'+lift);
	
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
	else var lift = 'LiftLeft'; // TO_EDIT
	
	var prilazna = position_data[Config.color].treci[odakle].point;
	var orientation = position_data[Config.color].treci[odakle].orientation[lift];
	
	Logger.debug('config_treci '+odakle+' '+lift);
	
	return {'prilazna':prilazna, 'orientation':orientation, 'lift':lift};
}

function pokupi_valjak(valjak)
{
	this
	.then(new MoveToPosition(valjak.prilazna.x,valjak.prilazna.y)) // pridji
	.then(new RotateTo(valjak.orientation)) // rotiraj se
	.then(new ActuatorCommand(valjak.lift, 'StartGetting')) // kupi
	.then(new SetSpeedMotion(50))
	.then(new MoveForward(distance))
	.then(new SetSpeedMotion(Config.default_speed))
	.then(new SleepCommand(1500))
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
		this
		.then(new ActuatorCommand('LiftLeft', 'StopGetting'))
		.then(new ActuatorCommand('LiftRight', 'StopGetting'))
		.then(new SetSpeedMotion(Config.default_speed))
		.then(Commands.finish_task);
	}
}

Manager.updateState("Ready");

