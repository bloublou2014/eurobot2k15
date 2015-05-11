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
var redosled_kupljenja = [pokupi_prvi, pokupi_drugi, pokupi_treci]; // default redosled
var counter = 0;

function onRun(){

	Logger.debug('running task: tri valjka');

	Config.do_setup();
	
	counter = 0; // za svaki slucaj, mada nema smisla da radi dvaput
	
	/*CommandChain(new ActuatorCommand('LiftRight','StartGetting'))
	.then(new ActuatorCommand('LiftLeft','StartGetting'))
	.success(function()
	{*/
		CommandChain(new SetSpeedMotion(50)) // tome
		.execute(); 
		Motion.update_current_status(function(){
		if(Math.abs(Motion.current_status.x) > 750) // sa nase strane
		{
			Logger.debug('sa nase strane');
			redosled_kupljenja = [pokupi_prvi, pokupi_treci, pokupi_drugi]; // 1 3 2
		}
		else // sa njihove strane
		{
			Logger.debug('sa njihove strane');
			redosled_kupljenja = [pokupi_drugi, pokupi_prvi, pokupi_treci]; // 2 1 3
		}
		
		call_next();
		});
	/*})
	.ignore_failure()
	.execute();*/
}

function onPause(){}

function pokupi_prvi()
{
	if(Motion.current_status.y > 1800)
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
	
	pokupi_valjak(prilazna, orientation, lift);
}

function pokupi_drugi()
{
	if(Motion.current_status.y > 900)
	{
		var odakle = 'gore';
	}
	else
	{
		var odakle = 'dole';
	}
	
	if(Config.color == 'YELLOW') var lift = 'LiftLeft'; // TODO  // TO_EDIT
	else var lift = 'LiftRight'; // TO_EDIT
	
	Logger.debug('pokupi_drugi '+odakle+' '+lift);
	
	var prilazna = position_data[Config.color].drugi[odakle].point;
	var orientation = position_data[Config.color].drugi[odakle].orientation[lift];
	Logger.debug('odakle:'+odakle+' lift:'+lift);
	Logger.debug('prilazna:'+prilazna.x+','+prilazna.y+' orientation:'+orientation+' lift:'+lift);
	
	/*CommandChain(new SetSpeedMotion(50))
	.success(function()
	{*/
		pokupi_valjak(prilazna, orientation, lift);
	/*})
	.ignore_failure()
	.execute();*/
}

function pokupi_treci()
{
	if(Math.abs(Motion.current_status.x) > 400)
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
	
	Logger.debug('pokupi_treci '+odakle+' '+lift);
	
	pokupi_valjak(prilazna, orientation, lift);
}

function pokupi_valjak(prilazna, orientation, lift)
{Logger.debug('prilazna:'+prilazna.x+','+prilazna.y+' orientation:'+orientation+' lift:'+lift+'!!!!!!!!!!!!!!!!!!!!!!!!!!');
	CommandChain(new MoveToPosition(prilazna.x,prilazna.y))
	.then(new RotateTo(orientation))
	.then(new ActuatorCommand(lift, 'StartGetting'))
	//.then(new ActuatorCommand('LiftLeft', 'StartGetting'))
	//.then(new ActuatorCommand('LiftRight', 'StartGetting'))
	.then(new SetSpeedMotion(50))
	.then(new MoveForward(distance))
	.then(new SetSpeedMotion(Config.default_speed))
	.then(new SleepCommand(1500))
	.then(new ActuatorCommand(lift, 'StopGetting'))
	//.then(new ActuatorCommand('LiftLeft', 'StopGetting'))
	//.then(new ActuatorCommand('LiftRight', 'StopGetting'))
	.success(function()
	{
		/*CommandChain(new SetSpeedMotion(Config.default_speed)) // verovatno netreba uopste, resetuje se vec iznad
		.success(function()
		{*/
			call_next();
		/*})
		.ignore_failure()
		.execute();*/
	})
	.ignore_failure()
	.execute();
}

function call_next()
{
	Motion.update_current_status(function(){
	if(counter < redosled_kupljenja.length) // ako ima jos valjaka
	{
		redosled_kupljenja[counter++](); // kupi sledeci
	}
	else // ako nema vise, zavrsi
	{
		CommandChain(new ActuatorCommand('LiftLeft', 'StopGetting'))
		.then(new ActuatorCommand('LiftRight', 'StopGetting'))
		.then(new SetSpeedMotion(Config.default_speed))
		.success(function()
		{
			Manager.updateState("Finished");
		})
		.ignore_failure()
		.execute();
	}
	});
}

Manager.updateState("Ready");
