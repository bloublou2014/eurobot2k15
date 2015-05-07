/**
	Udara klapne sa nase strane (dve).
	Udara obe u cugu, ako je prva vec udarena udara odmah drugu, ako je druga vec udarena udara samo prvu.
*/

var udarena = {'prva':false, 'druga':false}

var pos_y = 370; // TO_EDIT // TOME EDIT 350 
var yellow_positions = // TO_EDIT
{
	'prilazna_prva':{'x':-1280, 'y':pos_y}, // TOME EDIT 1278 // 1288
	'prilazna_druga':{'x':-800, 'y':pos_y}, //zuta x:-752 // TOME EDIT 780  
 	'kraj_prva':{'x':-1162, 'y':pos_y}, // kad da zatvori kad udara obe (gleda se samo y, prebaci cu kasnije da bude samo obicna promenljiva)
};
var green_points = Motion.invert_x(yellow_positions);
var positions_colored={'YELLOW':yellow_positions, 'GREEN':green_points};

var orientation_colored = {'YELLOW':0, 'GREEN':180};

var flap_colored = {'YELLOW':'Right', 'GREEN':'Left'};

var lift_colored = {
	'YELLOW': 'LiftRight',
	'GREEN' : 'LiftLeft'
}; 

var distance = 150;

function setup()
{
	Config['positions'] = positions_colored[Config.color];
	Config['orientation'] = orientation_colored[Config.color];
	Config['flap'] = flap_colored[Config.color];
	Config['lift'] = lift_colored[Config.color]; 
}

function onRun(){

	Config.do_setup(setup);

	Logger.debug('running task: klapne');
	
	if(udarena.prva && udarena.druga) // za svaki slucaj
	{
		Manager.updateState("Finished");
		return;
	}
	
	if(!udarena.prva) // idi na prvu
	{
		CommandChain(new SetSpeedMotion(50))
		.then(new MoveToPosition(Config.positions.prilazna_prva.x,Config.positions.prilazna_prva.y))
		//.then(new MoveToPosition(Config.positions.prilazna_prva.x,Config.positions.prilazna_prva.y))
		.then(new SetSpeedMotion(Config.default_speed))
		.then(new RotateTo(90))
		.then(new SetSpeedMotion(50))
		.then(new MoveForward(-distance))
		.then(new SetSpeedMotion(Config.default_speed))
		.then(new RotateTo(Config.orientation))
		.success(function()
		{
			set_flap_otvoren(true, function()
			{
				if(!udarena.druga) // idi skroz do kraja, udari obe
				{
					CommandChain(new MoveForward(750)) // kolko da ide napred // TO_EDIT
					.progress(function(msg){
						if(should_close(msg.x))
						{
							set_flap_otvoren(false);
							set_udarena('prva');
						}
						else
						{
							set_flap_otvoren(true);
						}
					})
					.success(function(){
						// ako je stigo do kraja druge
						set_flap_otvoren(false);
						set_udarena('druga');
					})
					.catch(error)
					.execute();
				}
				else // udarena druga, udari samo prvu
				{
					CommandChain(new MoveForward(200)) // kolko da ide napred // TO_EDIT
					.success(function(){
						// ako je stigo do kraja prve
						set_flap_otvoren(false);
						set_udarena('prva');
					})
					.catch(error)
					.execute();
				}
			});
		})
		.catch(error)
		.execute();
	}
	else // udarena prva, idi odma na drugu
	{
		CommandChain(new MoveToPosition(Config.positions.prilazna_druga.x,Config.positions.prilazna_druga.y+100))
		.then(new MoveToPosition(Config.positions.prilazna_druga.x,Config.positions.prilazna_druga.y))
		.success(function()
		{
			set_flap_otvoren(true);
		})
		.then(new RotateTo(Config.orientation))
		.success(function(){
			CommandChain(new MoveForward(200)) // kolko da ide napred // TO_EDIT
			.success(function(){
				// ako je stigo do kraja druge
				set_flap_otvoren(false);
				set_udarena('druga');
			})
			.catch(error)
			.execute();
		})
		.catch(error)
		.execute();
	}
}

function onPause(){}

function error()
{
	Logger.error('error in klapne nase');
	CommandChain(new SetSpeedMotion(Config.default_speed))
	.success(function()
	{
		set_flap_otvoren(false, function()
		{
			/*Task.suspend_for(7000, function() //10s
			{
				// TODO da proveri dal je uradjen casa nasa dole
				Manager.updateState("Ready");
			});*/
			//Task.ready_after(7000);
			Manager.updateState("Suspended");
		});
	})
	.execute()
}

function set_udarena(klapna)
{
	//Logger.debug('udarena klapna: '+klapna);
	udarena[klapna] = true;
	if(udarena.prva && udarena.druga)
	{
		CommandChain(new SetSpeedMotion(20))
		.then(new MoveForward(5))
		.then(new ActuatorCommand(Config.lift,'Unload'))
		.then(new MoveForward(-100))
		.then(new SetSpeedMotion(Config.default_speed))
		.success(function(){
			Manager.updateState("Finished");
		})
		.catch(error)
		.execute();
	}
}

var flap_otvoren = false;
function set_flap_otvoren(otvori, success)
{
	if (success === undefined)
	{
		success = function(){};
	}
	
	if(flap_otvoren)
	{
		if(otvori) success(); // vec otvoren
		else // zatvori
		{
			Command.send(new ActuatorCommand("Flap","Unkick"+Config.flap), success, function(){Logger.error('error pri zatvaranju flapa');});
			flap_otvoren = false;
		}
	}
	else // zatvoren je
	{
		if(otvori) 
		{
			Command.send(new ActuatorCommand("Flap","Kick"+Config.flap), success, function(){Logger.error('error pri otvaranju flapa');}); // otvori
			flap_otvoren = true;
		}
		else success(); // vec zatvoren
	}
}

function should_close(x)
{
	if(Config.color == 'YELLOW')
	{
		return x > Config.positions.kraj_prva.x && x < Config.positions.prilazna_druga.x;
	}
	else // 'GREEN'
	{
		return x > Config.positions.prilazna_druga.x && x < Config.positions.kraj_prva.x;
	}
}

Manager.updateState("Ready");