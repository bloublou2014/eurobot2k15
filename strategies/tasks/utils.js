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

var Motion = 
{
	'current_status':
	{
		'x':0,
		'y':0,
		'orientation':0,
		'direction':'',
		'speed':'',
		'state':'',
	},
	'invert_x':function(points)
	{
		var ret = {};
		for(var point_name in points)
		{
			ret[point_name] = {'x':-(points[point_name].x), 'y':points[point_name].y};
		}
		return ret;
	},
	'update_current_status':function(success)
	{
		return CommandChain(new GetMotionState())
		.success(function(msg)
		{
			Motion.current_status = msg;
			success();
		}).execute();
	}
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
	this._success_callbacks = []; // pozivaju se ako komanda uspe, ako su definisani
	this._failure_callback = null; // poziva se ako komanda ne uspe, ako je definisan
	this._progress_callback = null; // poziva se kao progres za komandu, ako je definisan
	this._ignore_failure = false;
	this.then = function(next_object) // ubacuje novu komandu ili lanac komandi u ovaj lanac i vraca novi cvor (next_object moze biti komanda ili CommandChainNode)
	{
		if(next_object instanceof CommandChainNode) // dobili smo novi lanac
		{
			return this._chain(next_object);
		}
		else // dobili smo komandu
		{
			return this._chain(new CommandChainNode(next_object));
		}
	};
	this._chain = function(next_node) // ulancava novi cvor u lanac i vraca novi cvor
	{
		next_node._first = this._first;
		this._next = next_node;
		next_node._previous = this;
		return next_node;
	};
	this.success = function(success_callback) // podesava success_callback za trenutni cvor i vraca trenutni cvor
	{
		this._success_callbacks.push(success_callback);
		return this;
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
	this._execute_self = function() // izvrsava trenutni cvor
	{
		var success_fun = function(msg) // success
		{
			for(var i=0;i<this._success_callbacks.length;i++) // pozovi sve success callbacke ako ih ima redom kako su zadati
			{
				if(this._success_callbacks[i].length > 0)
				{
					this._success_callbacks[i](msg);
				}
				else
				{
					this._success_callbacks[i]();
				}
			}
			if(this._next) this._next._execute_self(); // ako postoji sledeci cvor izvrsava se
		}.bind(this);
		
		if(this._ignore_failure)
		{
			var failure_fun = success_fun;
		}
		else if(this._failure_callback)
		{
			var failure_fun = this._failure_callback;
		}
		else
		{
			var failure_fun = function(){};
		}
		
		if(this._progress_callback)
		{
			var progress_fun = this._progress_callback;
		}
		else
		{
			var progress_fun = function(){};
		}
		
		Command.send(this._command, success_fun, failure_fun, progress_fun);
	};
	this.ignore_failure = function()
	{
		this._ignore_failure = true;
		if(this._previous)
		{
			this._previous.ignore_failure();
		}
		return this;
	}
}

function CommandChain(first_command)
{
	return new CommandChainNode(first_command);
}

var Commands = 
{
	'do_nothing':function(){}
}

//////////////////////////
	
/**
	Task utils.
*/

var Task = 
{
	'suspended':false,
	'suspend_for':function(timeout, wake_up)
	{
		Task.suspended = true;
		Manager.updateState("Suspended");
		setTimeout(function()
		{
			Task.suspended = false;
			wake_up();
		}, timeout);
	},
	'ready_after':function(timeout)
	{
		Task.suspend_for(timeout, function(){Manager.updateState("Ready");});
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
