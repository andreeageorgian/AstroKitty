#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

struct Point {
	float x;
	float y;
};

struct Circle {
	float radius;
	float centerX;
	float centerY;
};

class AstroKitty : public SimpleScene
{
public:
	AstroKitty();
	~AstroKitty();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;
	
	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:
	glm::mat3 modelMatrix;
	float triangleHight, triangleSide;
	float rectangleWidth, rectangleLength;
	float decagonRadius;
	float translateX, translateY, asteroid_translateX;
	float scaleX, scaleY;
	float astronaut_angularStep, asteroid_angularStep, temp_angularStep;
	int move_astronaut;
	float temp_transX ,temp_transY;
	float distance;
	float directionX, directionY;
	float startX, startY;
	float curr_dist, curr_scale;
	int sign, sign2;
	

	//varfuri triunghi
	Point triangle_tip[3];

	//varfuri asteroizi
	Point translatingAsteroidTips[9], scalingAsteroidTips[9], rotatingAsteroidTips[9];
	//cercurile folosit pentru aproximarea asteroizilor
	Circle transAsteroidCircle, scalingAsteroidCircle, rotatingAsteroidCircle;
	
	//varfuri platforma stationare
	Point stationaryPlatformTips[4];

	//varfuri platforma reflexie
	Point reflectionPlatformTips[4];

	//varfuri platforma finish
	Point finishPlatformTips[4];

	//colturile scenei
	Point sceneCorners[4];


	int stationary_state;
	
	//normala folosita la reflexie
	float normalX, normalY;
	float translate_dot_normal;

	//coordonatele vectorului de reflexie
	float reflectedX, reflectedY;
	int reflection_state;

	//panta platformei de reflexie
	float astronaut_slope;
	float reflection_platform_slope;
	//unghiul de reflexie
	float reflection_angle;

	//panta normalei la suprafata platformei de reflexie
	float normal_reflection_slope;
	
	//variabile folosite pentru determinarea directiei de deplasare a astronautului in cazul reflexiei
	float init_directionX, init_directionY, init_distance;
	Point point_on_normal;
	float norm_directionX, norm_directionY, norm_distance;
	float init_dot_norm;

	int check;
	Point intersection;

	//variabila folosita pentru ditrugerea asteroizilor
	int eraseTranslatingAsteroid, eraseScalingAsteroid, eraseRotatingAsteroid;

	//variabila pentru terminarea jocului
	int eraseAll;
};
