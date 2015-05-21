
var preconditions = 
{
	'casa_nasa_dole':function()
	{
		return Scheduler.getWorldState('nosi_casu') == 'false';
	},
	'casa_nasa_gore':function()
	{
		return Scheduler.getWorldState('nosi_casu') == 'false';
	},
	'unload_loptice':function()
	{
		return Scheduler.getWorldState('pokupio_loptice') == 'true' && Scheduler.getWorldState('istovario_start') == 'false';
	},
	'unload_all_start':function()
	{
		return Scheduler.getWorldState('istovario_loptice') == 'true' || phase == 'unloading';
	},
	'unload_casa_enemy':function()
	{
		return Scheduler.getWorldState('nosi_casu') == 'true';
	},
}

var phase = 'initial'; // unloading, finalize

var unloading_tasks = ['unload_all_center', 'unload_casa_start', 'unload_lights_center', 'unload_casa_enemy', 'unload_all_start', 'unload_loptice'];
var current_area = 'none'; // enemy, our_down, our_up, none
var color = 'GREEN';

function init_world_state()
{
	Scheduler.setWorldState('tried_casa_nasa_dole','false');
	Scheduler.setWorldState('nosi_casu','false');
	Scheduler.setWorldState('pokupio_loptice','false');
	Scheduler.setWorldState('istovario_loptice','false');
	Scheduler.setWorldState('istovario_start','false');
}

var area_tasks = 
{
	'enemy':['klapna_njihova', 'unload_casa_enemy'],
	'our_down':['casa_nasa_dole', 'klapne_nase'],
	'our_up':['valjak_prvi_stepeniste', 'valjak_gore_ugao', 'loptice'],
	'none':[],
}

function area_bonus(task_name)
{
	if(is_in_array(task_name, area_tasks[current_area])) return 100;
	else return 0;
}

function init_task_props()
{
	for (var task_name in Tasks)
	{
		Tasks[task_name]['finished'] = false;
		Tasks[task_name]['impossible'] = false;
	}
}

function onRun()
{
	Logger.debug('scheduler started');
	
	init_world_state();
	init_task_props(); // budz
	run_best_task();
}

function run_best_task()
{
	var best_task = find_best_task();
	if(best_task) 
	{
		Logger.debug("Starting task: "+best_task.name);
		Scheduler.startTask(best_task); // ako postoji task koji moze da se izvrsi poteraj ga
	}
	else Logger.debug('no best task');
}

function onStateUpdate(task, oldState, newState)
{
	Logger.debug("Task updated state: "+task.name+" to state "+newState);
	
	if(newState == 'Finished') // budz
	{
		task.finished = true;
	}
	if(newState == 'Impossible') // budz
	{
		task.impossible = true;
	}
	
	if(!Scheduler.getRunningTask()) // ako nema taska koji se izrvrsava trenutno ili trenutni task nije ready
	{
		run_best_task(); // poteraj najbolji
	}
}

function find_best_task()
{
	/*if(phase == 'initial' || phase == 'unloading') // dok traje mec
	{
		return find_max();
	}
	else if(phase == 'finalize') // zavrsi mec
	{
		return Tasks['finalize_task'];
	}*/
	
	return find_max();
}

function find_max()
{
	var best_task = null;
	var max_rank = -1;
	for (var task_name in Tasks)
	{
		var task = Tasks[task_name];
		
		if(task.finished || task.impossible) continue; // preskoci ako je finished ili impossible
		
		if(task.state == 'Ready')
		{
			var precondition = preconditions[task_name];
			if(precondition) // ako ima preduslov
			{
				if(!preconditions[task_name]()) continue; // preskoci ako ne ispunjava preduslove
			}
			
			if(!allowed_in_current_phase(task.name)) continue; // filtriraj za trenutnu fazu
			
			var rank = task.rank + area_bonus(task_name);
			
			if(rank > max_rank) // nadji sa max rankom
			{
				best_task = task;
				max_rank = rank;
			}
		}
	}
	return best_task;
}

Notification.subscribe("TimePassedNotification",function(t)
{
	Logger.debug("Current time: "+t.passedTime);
	if (t.passedTime==70)
	{
		phase = 'unloading';
		for (var i=0;i<unloading_tasks.length;i++) // digni prioritet unloading_tasks za 100
		{
			var task = Tasks[unloading_tasks[i]];
			if(task) task.rank = task.rank+500;
		}
		/*if(!is_in_array(Scheduler.getRunningTask().name, unloading_tasks)) // ako trenutno ne radimo istovaranje
		{
			Scheduler.pauseActiveTask(); // zaustavi trenutni i pokreni najbolje istovaranje
			run_best_task();
		}*/
	}
	if(t.passedTime==89)
	{
		phase = 'finalize';
		Scheduler.pauseActiveTask();
		Tasks['finalize_task'].rank = 1000;
		run_best_task();
	}
});

function allowed_in_current_phase(task_name)
{
	if(task_name == 'finalize_task') // finalize_task moze samo ako je faza finalize
	{
		return phase == 'finalize';
	}
	else // za ostale taskove
	{
		if(phase == 'initial')
		{
			if(task_name == 'unload_lights_center') return false; // inicijalno ignorisi unload na centar
		}
		else if(phase == 'finalize') // ako je faza finalize
		{
			return false; // nijedan sem finalize_task nije dozvolkjen
		}
	}
	
	return true;
}

Notification.subscribe("MotionNotification",function(msg)
{
	if((msg.x >= 0 && color == 'GREEN') || (msg.x < 0 && color == 'YELLOW'))
	{
		current_area = 'enemy';
	}
	else // sa nase strae smo
	{
		if(msg.y < 780)
		{
			current_area = 'our_down';
		}
		else if(msg.y > 1210)
		{
			current_area = 'our_up';
		}
		else
		{
			current_area = 'none';
		}
	}
});

function is_in_array(el, arr)
{
	for(var i=0;i>arr.length;i++)
	{
		if(arr[i] == el) return true;
	}
	return false;
}

function onStop()
{
	Logger.debug("Scheduler destroyed");
}
