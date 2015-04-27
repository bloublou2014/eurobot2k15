/**
	Globalne funkcije ukljucene u svaki task.
*/

/**
	Motion utils.
*/

var Geometry = 
{
	'euclid_dist':function(point1, point2)
	{
		return Math.sqrt(Math.pow(point1.x - point2.x, 2) + Math.pow(point1.y - point2.y, 2));
	},
	'nearest_point':function(current_position, points)
	{
		return Math.min.apply(Math, points.map(function(point)
			{
				return Geometry.euclid_dist(current_position, point);
			}));
	},
	'nearest_point_name':function(current_position, points_map) // NEZNAM DAL RADI
	{
		var first_point = points_map.values().next().value;
		var first_key = points_map.keys().next().value;
		
		var min_val = Geometry.euclid_dist(current_position, first_point);
		var nearest_name = first_key;
		
		for(var current_key in points_map)
		{
			if(Geometry.euclid_dist(current_position, points_map[current_key]) < min_val)
			{
				nearest_name = current_key;
			}
		}
		return nearest_name;
	}
}

function Point2D(x,y)
{
	this.x = x;
	this.y = y;
}

var Motion = 
{
	'invert_x':function(points)
	{
		var ret = {};
		for(var point_name in points)
		{
			ret[point_name] = {'x':-(points[point_name].x), 'y':points[point_name].y};
		}
		return ret;
	},
}

/**
Config utils.
*/

var Config = 
{
	'color':'COLOR_NOT_SET',
	'do_setup':function do_setup(setup_fun)
	{
		Config.color = Manager.getColor();
		if(setup_fun) setup_fun();
	},
	// globalna podesavanja
	'default_speed':'120',
}

/**
Command chaining utils.
*/

function CommandChainNode(new_command)
{
	this._first = this; // prvi cvor u lancu komandi
	this._command = new_command; // komanda koja treba da se posalje
	this._next = null; // sledeci cvor u lancu
	this._previous = null; // prethodni cvor u lancu
	this._failure_callback = null; // poziva se ako komanda ne uspe, ako je definisan
	this._progress_callback = null; // poziva se kao progres za komandu, ako je definisan
	this._ignore_failure = false;
	this.then = function(next_object) // ubacuje novu komandu ili lanac komandi u ovaj lanac i vraca novi cvor (next_object moze biti komanda ili CommandChainNode)
	{
		if(next_object instanceof CommandChainNode) // dobili smo novi lanac
		{
			return this._chain(next_object);
		}
		else // dobili smo komandu (funkciju ili objekat)
		{
			return this._chain(new CommandChainNode(next_object));
		}
	};
	this._chain = function(new_node) // ulancava novi cvor u lanac i vraca novi cvor
	{
		new_node._first = this._first;
		
		if(this._next) // ako vec postoji _next umetni new_node u lanac
		{
			this._next._previous = new_node;
			new_node._next = this._next;
			new_node._failure_callback = this._failure_callback; // nasledjuje failure callback
		}
		
		this._next = new_node;
		new_node._previous = this;
		return new_node;
	};
	this.failure = function(failure_callback) // podesava failure_callback za trenutni cvor i vraca trenutni cvor
	{
		this._failure_callback = failure_callback;
		return this;
	};
	this.progress = function(progress_callback) // podesava progress_callback za trenutni cvor i vraca trenutni cvor
	{
		this._progress_callback = progress_callback;
		return this;
	};
	this.catch = function(catch_callback) // podesava failure_callback za trenutni cvor i sve prethodne ukoliko nemaju vec podesen i vraca trenutni cvor
	{
		if(!this._failure_callback)
		{
			this._failure_callback = catch_callback;
		}
		if(this._previous)
		{
			this._previous.catch(catch_callback);
		}
		return this;
	};
	this.execute = function() // pocinje izvrsavanje lanca komandi od pocetka
	{
		this._first._execute_self();
	};
	this._execute_self = function(prev_result) // izvrsava trenutni cvor
	{
		// success_fun
		var success_fun = function(_prev_result) // success
		{
			if(this._next) this._next._execute_self(_prev_result); // ako postoji sledeci cvor izvrsava se
		}.bind(this);
		
		// failure_fun
		if(this._ignore_failure) // ako ignorisemo failure pozovi success
		{
			var failure_fun = success_fun;
		}
		else if(this._failure_callback) // ako ne i definisan je callback pozovi callback
		{
			var failure_fun = function()
			{
				this._failure_callback.bind(this)();
				if(this._ignore_failure) // ako callback oce da ignorise gresku pozovi success
				{
					success_fun();
				}
			}.bind(this);
		}
		else // ako nije definisano ne radi nista
		{
			var failure_fun = function(){};
		}
		
		// progress_fun
		if(this._progress_callback)
		{
			var progress_fun = this._progress_callback;
		}
		else
		{
			var progress_fun = function(){};
		}
		
		// execute
		if(typeof(this._command) === "function") // function command
		{
			try
			{
				var result = this._command.bind(this)(prev_result);
				success_fun(result);
			}
			catch(err)
			{
				if(err === 'command_error') failure_fun(); // ako baci 'command_error' tretiraj kao fail
				else throw err;
			}
		}
		else // async command
		{
			Command.send(this._command, success_fun, failure_fun, progress_fun);
		}
	};
	this.ignore_failure = function() // ignorisi fail na ovom cvoru, pozovi success u svakom slucaju
	{
		this._ignore_failure = true;
	};
	this.ignore_all_failure = function() // ignorisi fail u ovom i svim prethodnim
	{
		this.ignore_failure();
		if(this._previous)
		{
			this._previous.ignore_all_failure();
		}
		return this;
	};
	this.abort = function() // prekini dalje izvrsavanje ovog lanca
	{
		this._next = null;
	};
	this.alter = function(next_object) // izmeni lanac, ostatak lanca se odbacuje i nadovezuje se nova komanda/lanac komandi
	{
		this.abort();
		return this.then(next_object);
	};
}

function CommandChain(first_command)
{
	return new CommandChainNode(first_command);
}

var Commands = 
{
	'do_nothing':function(){},
	'finish_task':function(){Manager.updateState("Finished");},
	'suspend_task':function(){Manager.updateState("Suspended");},
}

//////////////////////////
	
/**
	Task utils.
*/

var Task = 
{
	'suspended':false,
	'wake_up_after':function(timeout, wake_up)
	{
		Task.suspended = true;
		CommandChain(new SleepCommand(timeout))
		.then(function()
		{
			Task.suspended = false;
			wake_up();
		})
		.execute();
		Manager.updateState("Suspended");
	},
	'ready_after':function(timeout)
	{
		this.wake_up_after(timeout, function()
		{
			Manager.updateState("Ready");
		});
	}
}

////////////////////////////////////

/**
	Lift utils.
*/
/*
var Lift = 
{
	'item_count':
	{
		'LiftLeft':0,
		'LiftRight':0,
		'LiftCenter':0,	
	},
	'collecting_items':
	{
		'LiftLeft':0,
		'LiftRight':0,
		'LiftCenter':0,
	},
	'max_items':
	{
		'LiftLeft':4,
		'LiftRight':4,
		'LiftCenter':1,
	},
	'collecting':function(number, lift, on_update)
	{
		Lift.collecting_items[lift] = number;
		Notification.subscribe("", function(msg)
		{
			Lift.item_count[lift] = msg.;
			on_update();
		});
	},
	'has_room':function()
	{
		return Lift.has_room_in('LiftLeft') && Lift.has_room_in('LiftRight') && Lift.has_room_in('LiftCenter');
	},
	'has_items_to_unload':function()
	{
		return Lift.has_items_to_unload_with('LiftLeft') || Lift.has_items_to_unload_with('LiftRight') || Lift.has_items_to_unload_with('LiftCenter');
	},
	'has_items_in':function(lift)
	{
		return Lift.item_count[lift] > 0;
	},
	'collects_items_with':function(lift)
	{
		return Lift.collecting_items[lift] > 0;
	},
	'has_items_to_unload_with':function(lift)
	{
		return Lift.collects_items_with(lift) && Lift.has_items_in(lift);
	},
	'has_room_in':function(lift)
	{
		return Lift.item_count[lift] + Lift.collecting_items[lift] <= Lift.max_items[lift];
	},
}
*/

/**
	Dumb path finding.
*/


	var upper_bound = 2000-790; // TODO
	var lower_bound = 790; // TODO
	
	var upper_point = {'x':1500-750,'y':2000-(790/2)}; // TODO
	var middle_point = {'x':1500-750,'y':1000}; // TODO
	var lower_point = {'x':1500-750,'y':790/2}; // TODO
	
function find_path(destination, success, error)
{	
	CommandChain(new GetMotionState())
	.success(function(msg)
	{
		if(((msg.y > upper_bound) && (destination == 'UP')) || // already up
		   ((msg.y > lower_bound && msg.y < upper_bound) && (destination == 'MIDDLE')) || // already middle
			((msg.y < lower_bound) && (destination == 'DOWN'))) // already down
		{
			success();
			return;
		}
		
		if(msg.y > upper_bound)
		{
			var first_point = upper_point;
		}
		else if(msg.y > lower_bound && msg.y < upper_bound)
		{
			var first_point = middle_point;
		}
		else // msg.y < lower_bound
		{
			var first_point = lower_point;
		}
		
		if(destination == 'UP')
		{
			var second_point = upper_point;
		}
		else if(destination == 'MIDDLE')
		{
			var second_point = middle_point;
		}
		else // DOWN
		{
			var second_point = lower_point;
		}
		
		var fx = first_point.x;
		var fy = first_point.y;
		var sx = second_point.x;
		var sy = second_point.y;
		
		if(color == 'YELLOW')
		{
			fx = -fx;
			sx = -sx;
		}
		
		CommandChain(new MoveToPosition(fx, first_point.y))
		.then(new MoveToPosition(sx, second_point.y))
		.success(success)
		.catch(error)
		.execute();
	})
	.ignore_failure()
	.execute();
}

///////////////////////////////////////////////////////////////////
