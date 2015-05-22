/**
	Inicijalna podesavanja.
*/

Config.setup = function()
{
	var init_positions = {'YELLOW':{'x':-1230, 'y':1000, 'o':0}, 
						  'GREEN' :{'x':1230, 'y':1000 , 'o':180}};
	Config['init_position'] = init_positions[Config.color];
	Config['init_speed'] = Config.default_speed;
}

function internal_error()
{
	Logger.error("Internal error occured!!! nesto gadno nevalja..");
}

function onRun()
{
	/*CommandChain(new ActuatorCommand('EnemyDetector','StopBrkon'))
	.execute(); */

	Logger.debug('running task: init task');
	CommandChain(new SetSpeedMotion(Config.init_speed))
	.then(new SetPosition(Config.init_position.x, Config.init_position.y, Config.init_position.o))
	//.then(new ActuatorCommand("EnemyDetector", "StartBrkon"))
	.then(init_pf_obstacles)
	.then(Commands.set_world_state('our_color', Config.color))
	.then(function()
	{
		Logger.debug('brkon started');
		Logger.debug('initialization: speed='+Config.init_speed+
		', x='+Config.init_position.x+
		', y='+Config.init_position.y+
		', o='+Config.init_position.o);
	})
	.then(Commands.finish_task)
	.catch(function()
	{
		Logger.error('brkon nije started');
	})
	.execute();
}

function init_pf_obstacles()
{
	Logger.debug("Init obsticles");
	if(Config.color == 'GREEN') // GREEN
	{
		this
		.then(new AddStaticObject([new Point2D(900,600), // zelena donja start precka
								   new Point2D(1500,600),
								   new Point2D(1500,950),
								   new Point2D(905,950)]))
		.then(new AddStaticObject([new Point2D(905,1050), // zelena gornja start precka
								   new Point2D(1500,1050),
								   new Point2D(1500,1400),
								   new Point2D(900,1400)]))
		.then(new AddStaticObject([new Point2D(-1500,600), // enemy polje kad smo mi zeleni
								   new Point2D(-900,600),
								   new Point2D(-900,1400),
								   new Point2D(-1500,1400)]))
		.then(new AddStaticObject([new Point2D(750,750), // nasa casa ispred starta
								   new Point2D(400,1050),
								   new Point2D(400,1350)]))
								   ;
	}
	else // YELLOW
	{
		this
		.then(new AddStaticObject([new Point2D(-1500,600), // zuta donja start precka
								   new Point2D(-900,600),
								   new Point2D(-905,950),
								   new Point2D(-1500,950)]))
		.then(new AddStaticObject([new Point2D(-1500,1050), // zuta gornja start precka
								   new Point2D(-905,1050),
								   new Point2D(-900,1400),
								   new Point2D(-1500,1400)]))
		.then(new AddStaticObject([new Point2D(900,600), // enemy polje kad smo mi zuti
								   new Point2D(1500,600),
								   new Point2D(1500,1400),
								   new Point2D(900,1400)]))
		.then(new AddStaticObject([new Point2D(-750,750), // nasa casa ispred starta
								   new Point2D(-400,1350),
								   new Point2D(-400,1050)]))
								   ;
	}
	
	this
	.then(new AddStaticObject([new Point2D(-700,1250), // stepeniste
							   new Point2D(700,1250),
							   new Point2D(700,2000),
							   new Point2D(-700,2000)]))
	.then(new AddStaticObject([new Point2D(-450,0), // centralno crveno postolje
							   new Point2D(450,0),
							   new Point2D(450,300),
							   new Point2D(-450,300)]));
	// dodati dispenzere za loptice po potrebi
}

function onPause(){}

Manager.updateState("Ready");