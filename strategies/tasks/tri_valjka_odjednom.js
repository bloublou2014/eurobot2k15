
/**
	Kupi tri valjka na sredini odjednom bez prilaznih tacaka.
*/

var angle_first_colored = {'GREEN':-143, 'YELLOW':-37};
var lift_colored = 
{
	'GREEN':{'first':'LiftLeft', 'second':'LiftRight'},
	'YELLOW':{'first':'LiftRight', 'second':'LiftLeft'},
}

var position_colored = 
{
	'GREEN':{'x':289, 'y':539},
	'YELLOW':{'x':-289, 'y':539},
}

Config.setup = function()
{
	Config['angle_first'] = angle_first_colored[Config.color];
	Config['lift'] = lift_colored[Config.color];
	Config['position'] = position_colored[Config.color];
}

var retry_count = 0;

function onRun()
{
	Logger.debug('running task: tri valjka');
	
	CommandChain(new GetMotionState())
	.then(function(msg) // odredi redosled kupljenja
	{
		if(Math.abs(msg.x) > 750) // sa nase strane
		{
			Logger.debug('sa nase strane');
			this
			.then(new RotateTo(Config.angle_first))
			.then(new SetSpeedMotion(100))
			//.then(new MoveForward(750))
			.then(new MoveToPosition(Config.position.x, Config.position.y))
			//.then(Commands.log_position)
			.progress(function(msg)
			{
				lift_getting(Config.lift.first, (Math.abs(msg.x) < 720 && Math.abs(msg.x) > 200)); // prvi kupi kad je x u nekim granicama
			})
			.failure(function()
			{
				if(retry_count++ < 2)
				{
					this
					.then(new SleepCommand(1000))
					.then(new MoveToPosition(Config.position.x, Config.position.y));
				}
				else
				{
					// alter da ide do treceg valjka na poziciju
					this.alter(Commands.finish_task);
				}
				return 'continue';
			})
			//.then(new SetSpeedMotion(90))
			.then(new SleepCommand(300)) //500 // 350 
			.then(new RotateTo(-90))
			.then(new ActuatorCommand('LiftLeft', 'StartGetting'))
			.then(new ActuatorCommand('LiftRight', 'StartGetting'))
			.then(new MoveForward(200))
			.then(new SleepCommand(150)) //400 // 250
			.then(new ActuatorCommand('LiftLeft', 'StopGetting'))
			.then(new ActuatorCommand('LiftRight', 'StopGetting'))
		}
		else // sa njihove strane
		{
			Logger.debug('sa njihove strane');
			Logger.error('not implemented');
		}
	})
	.then(new SetSpeedMotion(Config.default_speed))
	.then(Commands.finish_task)
	.execute();
}

var lift_started = {'LiftLeft':false,'LiftRight':false};
function lift_getting(lift, start)
{
	if(start)
	{
		if(!lift_started[lift]) 
		{Logger.debug('start lift ' +lift);
			lift_started[lift] = true;
			CommandChain(new ActuatorCommand(lift, 'StartGetting')).execute();
		}
	}
	else // stop
	{
		if(lift_started[lift])
		{Logger.debug('stop lift ' +lift);
			lift_started[lift] = false;
			CommandChain(new ActuatorCommand(lift, 'StopGetting')).execute();
		}
	}
}

function onPause()
{
	CommandChain(new ActuatorCommand('LiftLeft', 'StopGetting'))
	.then(new ActuatorCommand('LiftRight', 'StopGetting'))
	.execute();
}

Manager.updateState("Ready");

