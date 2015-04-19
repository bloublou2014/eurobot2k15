/**
	Monitor task.
*/

function onRun(){}
function onPause(){}

Notification.subscribe("EnemyDetectedNotification", function(msg)
{
	Logger.debug('enemy detected, '+msg.type+', angle='+msg.angle+', detected='+msg.detected);
});