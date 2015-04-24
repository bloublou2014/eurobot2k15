/*
#include "PathFindingNode.hpp"
#include "geometry/Point2D.h"
#include <deque>
#include <algorithm>

namespace path_finding
{

ros::Publisher newPathReportPublisher;
ros::Publisher tempObsticleReportPublisher;*/
/*
PathFindingNode::PathFindingNode(ros::NodeHandle nh, std::string robotType):pathFinding(0,0,0,0),
																			robotType(robotType)
{
	// definicije prepreka
	std::vector<geometry::Point2D> obsticlePoints;

	nh.param<int>("/ourRadius", ourRadius, 300);
	ROS_INFO("ourRadius is: %d", ourRadius);

	pathFinding = PathFinding(1500-(ourRadius+50), -1500+(ourRadius+50), 2000-(ourRadius+50), 0+(ourRadius+50));

	int triangleSide;
	int triangleHeight;

	// CENTRALNI KRUG (sestougao)
	const int krugRadius = 200;
	const int krugX = 0;
	const int krugY = 950;

	triangleSide = ourRadius + krugRadius;
	triangleHeight = (sqrt(3)/2)*triangleSide;

	// opisi sestougao
	dodajSestougao(krugX, krugY, triangleSide);

	//MAMUTI

	int mx1,mx2,mx3,mx4,my1,my2,my3,my4;

	// LEVI MAMUT

	mx1 = -1500+350;
	my1 = 2000;
	mx2 = -350;
	my2 = 2000-300;

	obsticlePoints.push_back(geometry::Point2D(mx1-ourRadius,my1));
	obsticlePoints.push_back(geometry::Point2D(mx2+ourRadius,my1));
	obsticlePoints.push_back(geometry::Point2D(mx2+ourRadius,my2-ourRadius));
	obsticlePoints.push_back(geometry::Point2D(mx1-ourRadius,my2-ourRadius));
	pathFinding.addStaticObsticle(obsticlePoints);
	obsticlePoints.clear();

	// DESNI MAMUT

	mx1 = 350;
	mx2 = 1500-350;

	obsticlePoints.push_back(geometry::Point2D(mx1-ourRadius,my1));
	obsticlePoints.push_back(geometry::Point2D(mx2+ourRadius,my1));
	obsticlePoints.push_back(geometry::Point2D(mx2+ourRadius,my2-ourRadius));
	obsticlePoints.push_back(geometry::Point2D(mx1-ourRadius,my2-ourRadius));
	pathFinding.addStaticObsticle(obsticlePoints);
	obsticlePoints.clear();

	// DRVECE

	int drvoTriangleSide = 150 + ourRadius;
	dodajSestougao(1500, 700, drvoTriangleSide);
	dodajSestougao(-1500, 700, drvoTriangleSide);
	dodajSestougao(800, 0, drvoTriangleSide);
	dodajSestougao(-800, 0, drvoTriangleSide);

	//================

	//CUSTOM ZA MALOG
	if(robotType == "mali")
	{
		// da ne prelazi na drugu polovinu
		obsticlePoints.push_back(geometry::Point2D(1600, -100));
		obsticlePoints.push_back(geometry::Point2D(1600, 1000));
		obsticlePoints.push_back(geometry::Point2D(-1600, 1000));
		obsticlePoints.push_back(geometry::Point2D(-1600, -100));
		pathFinding.addStaticObsticle(obsticlePoints);
		obsticlePoints.clear();
	}
	//================
}*/
/*
void PathFindingNode::dodajSestougao(int krugX, int krugY, int triangleSide)
{
	int triangleHeight = (sqrt(3)/2)*triangleSide;
	std::vector<geometry::Point2D> obsticlePoints;

	obsticlePoints.push_back(geometry::Point2D(krugX + triangleSide,     krugY));
	obsticlePoints.push_back(geometry::Point2D(krugX + (triangleSide/2), krugY - triangleHeight));
	obsticlePoints.push_back(geometry::Point2D(krugX - (triangleSide/2), krugY - triangleHeight));
	obsticlePoints.push_back(geometry::Point2D(krugX - triangleSide,     krugY));
	obsticlePoints.push_back(geometry::Point2D(krugX - (triangleSide/2), krugY + triangleHeight));
	obsticlePoints.push_back(geometry::Point2D(krugX + (triangleSide/2), krugY + triangleHeight));
	pathFinding.addStaticObsticle(obsticlePoints);
}*/
/*
bool PathFindingNode::findPathCallback(findPathService::Request &req, findPathService::Response &resp)
{
	ROS_INFO("path_finding findPathCallback with: startX: %d, startY: %d, goalX: %d, goalY: %d, ignoreDynamic=%d", req.start.x, req.start.y, req.goal.x, req.goal.y, req.ignoreDynamicPolygons);

	geometry::Point2D startPoint = geometry::Point2D(req.start.x, req.start.y);
	geometry::Point2D goalPoint = geometry::Point2D(req.goal.x, req.goal.y);

	std::deque<geometry::Point2D> path;

	newPathReport report;
	report.start = req.start;
	report.goal = req.goal;

	bool enemiesClose = false;
	for(std::vector<int>::iterator id_it = dynamicObsticleIDs.begin(); id_it != dynamicObsticleIDs.end(); id_it++)
	{
		// ako je bar jedan od protivnika blizu
		if(dynamicObsticleIPositions[*id_it].euclidDist(startPoint) < 400)
		{
			enemiesClose = true;
		}
	}

	if(!enemiesClose) ROS_INFO("debug nisu blizu enemy, ignorisem ih");

	if(pathFinding.search(startPoint, goalPoint, path, req.ignoreDynamicPolygons || !enemiesClose))
	{
		ROS_INFO("Found path:");

		// prepakuj u vector<geometry::point2D> (generisani rosov tip point2D za poruke)

		resp.path.clear();

		for(std::deque<geometry::Point2D>::iterator pointIter = path.begin(); pointIter != path.end(); pointIter++)
		{
			geometry::point2D point;
			point.x = pointIter->getX();
			point.y = pointIter->getY();

			ROS_INFO("point: X: %d, Y: %d", point.x, point.y);

			resp.path.push_back(point);
			report.path.push_back(point);
		}


		newPathReportPublisher.publish(report);

		return true;
	}
	else
	{
		ROS_INFO("Can't find path.");

		newPathReportPublisher.publish(report);
		return false;
	}
}*/
/*
void PathFindingNode::changeDynamicObsticleCallback(const changeDynObsticleCommand::ConstPtr& command)
{
	ROS_INFO("changeDynamicObsticleCallback");

	for(int i = 0; i < command->points.size(); i++)
	{
		const geometry::point2D &newPoint = command->points[i];
		pathFinding.changeDynamicObsticlePoint(command->obsticleId, i, geometry::Point2D(newPoint.x, newPoint.y));
	}
}*/
/*
void PathFindingNode::setEnemyPositionsCallback(const enemy_detection::allEnemiesInfoMessage::ConstPtr& message)
{
	//ROS_INFO("updated");

	// za sve pristigle pozicije
	for(int i = 0; i < message->enemies.size(); i++)
	{
		int enemyPositionX = message->enemies[i].position.x;
		int enemyPositionY = message->enemies[i].position.y;

		int triangleSide = message->enemies[i].size + ourRadius;
		int triangleHeight = (sqrt(3)/2)*triangleSide;

		// ako postoji prepreka izmeni je
		if(i < dynamicObsticleIDs.size())
		{
			int obstcleId = dynamicObsticleIDs[i];

			pathFinding.changeDynamicObsticlePoint(obstcleId, 0, geometry::Point2D(enemyPositionX+triangleSide,     enemyPositionY));
			pathFinding.changeDynamicObsticlePoint(obstcleId, 1, geometry::Point2D(enemyPositionX+(triangleSide/2), enemyPositionY-triangleHeight));
			pathFinding.changeDynamicObsticlePoint(obstcleId, 2, geometry::Point2D(enemyPositionX-(triangleSide/2), enemyPositionY-triangleHeight));
			pathFinding.changeDynamicObsticlePoint(obstcleId, 3, geometry::Point2D(enemyPositionX-triangleSide,     enemyPositionY));
			pathFinding.changeDynamicObsticlePoint(obstcleId, 4, geometry::Point2D(enemyPositionX-(triangleSide/2), enemyPositionY+triangleHeight));
			pathFinding.changeDynamicObsticlePoint(obstcleId, 5, geometry::Point2D(enemyPositionX+(triangleSide/2), enemyPositionY+triangleHeight));
			dynamicObsticleIPositions[obstcleId] = geometry::Point2D(enemyPositionX, enemyPositionY);
		}
		else // ako ima manjak dinamickih prepreka
		{
			std::vector<geometry::Point2D> obsticlePoints;
			// dodaj novu sa pristiglim podacima
			obsticlePoints.push_back(geometry::Point2D(enemyPositionX+triangleSide,     enemyPositionY));
			obsticlePoints.push_back(geometry::Point2D(enemyPositionX+(triangleSide/2), enemyPositionY-triangleHeight));
			obsticlePoints.push_back(geometry::Point2D(enemyPositionX-(triangleSide/2), enemyPositionY-triangleHeight));
			obsticlePoints.push_back(geometry::Point2D(enemyPositionX-triangleSide,     enemyPositionY));
			obsticlePoints.push_back(geometry::Point2D(enemyPositionX-(triangleSide/2), enemyPositionY+triangleHeight));
			obsticlePoints.push_back(geometry::Point2D(enemyPositionX+(triangleSide/2), enemyPositionY+triangleHeight));
			int id = pathFinding.addDynamicObsticle(obsticlePoints);
			dynamicObsticleIDs.push_back(id);
			dynamicObsticleIPositions[id] = geometry::Point2D(enemyPositionX, enemyPositionY);
		}
	}
}*/
/*
geometry::polygon2D makePoly(Polygon *p)
{
	geometry::polygon2D result;

	std::vector<PfPoint2D*> points = p->getPoints();
	std::vector<PfPoint2D*>::iterator point;

	for(point = points.begin(); point != points.end(); point++)
	{
		int x = (*point)->getX();
		int y = (*point)->getY();

		geometry::point2D tmp_point;
		tmp_point.x = x;
		tmp_point.y = y;

		result.points.push_back(tmp_point);
	}

	return result;
}*/
/*
void PathFindingNode::addTempStaticCallback(const addTempObsticleCommand::ConstPtr& command)
{
	const int triangleSide = command->size;
	const int triangleHeight = (sqrt(3)/2)*triangleSide;

	struct TempObsticle obst;

	std::vector<geometry::Point2D> obsticlePoints;
	// dodaj novu sa pristiglim podacima
	obsticlePoints.push_back(geometry::Point2D(command->point.x+triangleSide,     command->point.y));
	obsticlePoints.push_back(geometry::Point2D(command->point.x+(triangleSide/2), command->point.y-triangleHeight));
	obsticlePoints.push_back(geometry::Point2D(command->point.x-(triangleSide/2), command->point.y-triangleHeight));
	obsticlePoints.push_back(geometry::Point2D(command->point.x-triangleSide,     command->point.y));
	obsticlePoints.push_back(geometry::Point2D(command->point.x-(triangleSide/2), command->point.y+triangleHeight));
	obsticlePoints.push_back(geometry::Point2D(command->point.x+(triangleSide/2), command->point.y+triangleHeight));

	obst.id = pathFinding.addStaticObsticle(obsticlePoints);
	time(&(obst.time_stamp));
	obst.duration = command->duration;
	tempStaticObsticles.push_back(obst);

	tempObsticleReport msg;
	std::map<int, Polygon*> ob_map = pathFinding.getAllStaticObsticles();

	std::map<int, Polygon*>::iterator obsticle;
	for(obsticle = ob_map.begin(); obsticle != ob_map.end(); obsticle++)
		msg.obsticles.push_back( makePoly(obsticle->second) );


	tempObsticleReportPublisher.publish(msg);

	ROS_INFO("added obstacle at x=%d, y=%d", command->point.x, command->point.y);
}*/

/*
bool staticObsticleExpired(const TempObsticle &obst)
{
	time_t now;
	time(&now);
	return difftime(now, obst.time_stamp) > obst.duration;
}*/
/*
void PathFindingNode::cleanUpTempObsticlesCallback(const ros::TimerEvent &e)
{
	for(std::list<TempObsticle>::iterator iter = tempStaticObsticles.begin(); iter != tempStaticObsticles.end(); iter++)
	{
		if(staticObsticleExpired(*iter))
		{
			pathFinding.removeObsticle(iter->id);
		}
	}

	tempStaticObsticles.remove_if(staticObsticleExpired);

	//ROS_INFO("tempStaticObsticles.count = %d", tempStaticObsticles.size());
}*/

//}



