#include "AstroKitty.h"

#include <vector>
#include <iostream>
#include <cmath>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"

using namespace std;
AstroKitty::AstroKitty()
{
}

AstroKitty::~AstroKitty()
{
}

//	Functie folosita pentru detectarea coliziunii intre un asteroid si 
//	astronaut. Daca distanta dintre centrul cercului si unul din varfurile
//	triunghiului este mai mica decat raza cercului ce aproximeaza asteroidul,
//	atunci cele doua figuri se intersecteaza.
//	Parametri:
//	- asteroid: o structura care descrie astroidul sub forma unui cerc(are raza 
//             si coordonatele centrului)
//	- triangle_tip1: unul din varfurile triunghiului
//	- triangle_tip2: al doilea varf al triunghiului
//	- triangle_tip3: al treilea varf al triunghiului
bool astronaut_vs_asteroid(Circle asteroid, Point triangle_tip1, Point triangle_tip2, Point triangle_tip3) {
	float centerX1, centerX2, centerX3, centerY1, centerY2, centerY3;
	centerX1 = asteroid.centerX - triangle_tip3.x;
	centerY1 = asteroid.centerY - triangle_tip3.y;
	if (sqrt(pow(centerX1, 2) + pow(centerY1, 2)) <= asteroid.radius) {
		return true;
	}

	centerX2 = asteroid.centerX - triangle_tip2.x;
	centerY2 = asteroid.centerY - triangle_tip2.y;
	if (sqrt(pow(centerX2, 2) + pow(centerY2, 2)) <= asteroid.radius) {
		return true;
	}

	centerX3 = asteroid.centerX - triangle_tip1.x;
	centerY3 = asteroid.centerY - triangle_tip1.y;
	if (sqrt(pow(centerX1, 3) + pow(centerY1, 3)) <= asteroid.radius) {
		return true;
	}
}

//	Functie care verifica apartenenta unui punct la o dreapta.
//	Parametri:
//	- point1: unul dintre capetele dreptei de interes
//	- point2: celalalt capat al dreptei
//	- check_point: punctul care urmeaza sa fie verificat
bool OnSegment(Point point1, Point point2, Point check_point)
{
	if (point2.x <= max(point1.x, check_point.x) && point2.x >= min(point1.x, check_point.x) &&
		point2.y <= max(point1.y, check_point.y) && point2.y >= min(point1.y, check_point.y))
		return true;

	return false;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise

//	Functie ce intoarce orientarea tripletului (point1, point2, check_point).
//	Functia intoarce:
//	- 0 daca point1, point2 si check_point sunt coliniare
//	- 1 daca orientarea este in sensul ceasului
//	- (-1) daca orientarea este in sens trigonometric
// See http://www.geeksforgeeks.org/orientation-3-ordered-points/
// for details of below formula.
float orientation(Point point1, Point point2, Point check_point)
{
	float val = (point2.y - point1.y) * (check_point.x - point2.x) -
		(point2.x - point1.x) * (check_point.y - point2.y);

	if (val == 0) return 0;  // colinear

	return (val > 0) ? 1 : 2; // clock or counterclock wise
}
// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
//	Functie care intoarce true daca segmentele P1Q1 se intersecteaza
//	cu segmentul P2Q2.
bool doIntersect(Point P1, Point Q1, Point P2, Point Q2)
{
	//	Calculam cele 4 orientari necesare pentru cazul general
	//	si pentru cazurile de baza.
	float O1 = orientation(P1, Q1, P2);
	float O2 = orientation(P1, Q1, Q2);
	float O3 = orientation(P2, Q2, P1);
	float O4 = orientation(P2, Q2, Q1);

	// Cazul general
	if (O1 != O2 && O3 != O4)
		return true;

	// Cazuri speciale
	// P1, Q1 si P2 sunt coliniare si P2 se afla pe segmentul P1Q1
	if (O1 == 0 && OnSegment(P1, P2, Q1)) return true;

	// P1, Q1 si P2 sunt coliniare si Q2 se afla pe segmentul P1Q1
	if (O2 == 0 && OnSegment(P1, Q2, Q1)) return true;

	// P2, Q2 si P1 sunt coliniare si P1 se afla pe segmentul P2Q2
	if (O3 == 0 && OnSegment(P2, P1, Q2)) return true;

	// P2, Q2 si Q1 sunt coliniare si Q1 se afla pe segmentul P2Q2
	if (O4 == 0 && OnSegment(P2, Q1, Q2)) return true;

	return false;
}


//	Functie ce calculeaza punctul de intersectie a doua drepte.
//	Parametri:
//	- P1: un capat al primei drepte
//	- Q1: celalalt capat al primei drepte
//	- P2: un capat al celei de-a doua drepte
//	- Q2: celalalt capat al celei de-a doua drepte
Point intersectionPoint(Point P1, Point Q1, Point P2, Point Q2)
{
	Point intersectionPoint;
	intersectionPoint.x = (((P2.x * Q2.y - Q2.x * P2.y) * (Q1.x - P1.x)) - ((P1.x * Q1.y - Q1.x * P1.y) * (Q2.x - P2.x)));
	intersectionPoint.x /= (((P1.y - Q1.y) * (Q2.x - P2.x)) - ((P2.y - Q2.y) * (Q1.x - P1.x)));
	intersectionPoint.y = (-(P1.x * Q1.y - Q1.x * P1.y) - (P1.y - Q1.y) * intersectionPoint.x) / (Q1.x - P1.x);

	return intersectionPoint;
}

void AstroKitty::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);
	triangleSide = 60;
	triangleHight = triangleSide * sqrt(3) / 2;
	rectangleWidth = 30;
	rectangleLength = 200;
	decagonRadius = 30;

	// initializam factorii de translatie
	translateX = 0;
	translateY = 0;
	asteroid_translateX = 0;

	// initializam factorii de scalare
	scaleX = 1;
	scaleY = 1;

	// initializam factorul de rotatie
	astronaut_angularStep = 0;
	asteroid_angularStep = 0;

	move_astronaut = 0;

	//initializam distanta maxima pe care asteroid1 poate translata
	curr_dist = 0;
	curr_scale = 0;
	sign = sign2 = 1;
	stationary_state = 0;
	reflection_state = 0;

	//astronautul
	Mesh* astronaut = Object2D::CreateTriangle("astronaut", corner, triangleSide, glm::vec3(0, 0.5f, 0.25f), false);
	AddMeshToList(astronaut);

	//varfurile triunghiului
	for (int i = 0; i < 3; i++) {
		triangle_tip[i].x = astronaut->vertices[i].position[0];
		triangle_tip[i].y = astronaut->vertices[i].position[1];
	}

	//platforma de stationare
	Mesh* stationaryPlatform = Object2D::CreateRectangle("stationaryPlatform", glm::vec3(900, 100, 0), rectangleWidth, rectangleLength + 100, glm::vec3(1, 1, 0), true);
	AddMeshToList(stationaryPlatform);

	//varfurile platformei de stationare
	for (int i = 0; i < 4; i++) {
		stationaryPlatformTips[i].x = stationaryPlatform->vertices[i].position[0];
		stationaryPlatformTips[i].y = stationaryPlatform->vertices[i].position[1];
	}

	//platforme reflexie
	Mesh* reflectionPlatform = Object2D::CreateRectangle("reflectionPlatform", glm::vec3(500, 680, 0), rectangleWidth, rectangleLength + 100, glm::vec3(0.5f, 1, 0), true);
	AddMeshToList(reflectionPlatform);

	//varfurile platformei de reflexie
	for (int i = 0; i < 4; i++) {
		reflectionPlatformTips[i].x = reflectionPlatform->vertices[i].position[0];
		reflectionPlatformTips[i].y = reflectionPlatform->vertices[i].position[1];
	}

	//platforme finish
	Mesh* finishPlatform = Object2D::CreateRectangle("finishPlatform", glm::vec3(950, 680, 0), rectangleWidth, rectangleLength + 100, glm::vec3(1, 0, 0), true);
	AddMeshToList(finishPlatform);

	//varfurile platformei de finish
	for (int i = 0; i < 4; i++) {
		finishPlatformTips[i].x = finishPlatform->vertices[i].position[0];
		finishPlatformTips[i].y = finishPlatform->vertices[i].position[1];
	}

	//colturile scenei
	for (int i = 0; i < 4; i++) {
		if (i == 0) {
			sceneCorners[0].x = 0;
			sceneCorners[0].y = 0;
		}
		else if (i == 1) {
			sceneCorners[1].x = 1280;
			sceneCorners[1].y = 0;
		}
		else if (i == 2) {
			sceneCorners[2].x = 1280;
			sceneCorners[2].y = 720;
		}
		else if (i == 3) {
			sceneCorners[3].x = 0;
			sceneCorners[3].y = 720;
		}
	}

	//variabile folosite pentru distrugerea asteroizilor la coliziune
	eraseTranslatingAsteroid = 0;
	eraseScalingAsteroid = 0;
	eraseRotatingAsteroid = 0;
	eraseAll = 0;

	//asteroidul ce efectueaza translatie
	Mesh* translatingAsteroid = Object2D::CreateDecagon("translatingAsteroid", decagonRadius, glm::vec3(1, 1, 0.5f), false);
	AddMeshToList(translatingAsteroid);

	//coordonatele varfurilor asteroidului de translatie
	for (int i = 0; i < 9; i++) {
		translatingAsteroidTips[i].x = 200 + translatingAsteroid->vertices[i].position[0];
		translatingAsteroidTips[i].y = 400 + translatingAsteroid->vertices[i].position[1];
	}

	//valorile campurilor structurii Circle folosita la aproximarea pentru coliziune
	//raza cercului circumscris asteroidul
	transAsteroidCircle.radius = triangleSide;

	//coordonatele centrului asteroidului
	transAsteroidCircle.centerX = intersectionPoint(translatingAsteroidTips[0], translatingAsteroidTips[5], translatingAsteroidTips[2], translatingAsteroidTips[7]).x;
	transAsteroidCircle.centerY = intersectionPoint(translatingAsteroidTips[0], translatingAsteroidTips[5], translatingAsteroidTips[2], translatingAsteroidTips[7]).y;

	//asteroidul ce efectueaza scalare
	Mesh* scalingAsteroid = Object2D::CreateDecagon("scalingAsteroid", decagonRadius, glm::vec3(1, 0.5f, 1), false);
	AddMeshToList(scalingAsteroid);

	//coordonatele varfurilor asteroidului ce scaleaza
	for (int i = 0; i < 9; i++) {
		scalingAsteroidTips[i].x = 400 + scalingAsteroid->vertices[i].position[0];
		scalingAsteroidTips[i].y = 200 + scalingAsteroid->vertices[i].position[1];
	}

	scalingAsteroidCircle.radius = triangleSide;
	scalingAsteroidCircle.centerX = intersectionPoint(scalingAsteroidTips[0], scalingAsteroidTips[5], scalingAsteroidTips[2], scalingAsteroidTips[7]).x;
	scalingAsteroidCircle.centerY = intersectionPoint(scalingAsteroidTips[0], scalingAsteroidTips[5], scalingAsteroidTips[2], scalingAsteroidTips[7]).y;

	//asteroidul ce efectueaza rotatie
	Mesh* rotatingAsteroid = Object2D::CreateDecagon("rotatingAsteroid", decagonRadius, glm::vec3(0.5f, 1, 1), false);
	AddMeshToList(rotatingAsteroid);

	//coordonatele varfurilor asteroidului ce se roteste
	for (int i = 0; i < 9; i++) {
		rotatingAsteroidTips[i].x = 800 + rotatingAsteroid->vertices[i].position[0];
		rotatingAsteroidTips[i].y = 400 + rotatingAsteroid->vertices[i].position[1];
	}

	rotatingAsteroidCircle.radius = triangleSide;
	rotatingAsteroidCircle.centerX = intersectionPoint(rotatingAsteroidTips[0], rotatingAsteroidTips[5], rotatingAsteroidTips[2], rotatingAsteroidTips[7]).x;
	rotatingAsteroidCircle.centerY = intersectionPoint(rotatingAsteroidTips[0], rotatingAsteroidTips[5], rotatingAsteroidTips[2], rotatingAsteroidTips[7]).y;

}

void AstroKitty::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void AstroKitty::Update(float deltaTimeSeconds)
{
	modelMatrix = glm::mat3(1);
	//calculam directia pe X si pe Y prin diferenta intre vectorul ce indica pozitia initiala si
	//vectorul care indica pozitia actuala
	directionX = translateX - startX;
	directionY = translateY - startY;

	//calculam distanta intre cei doi vectori, pentru a-i normaliza
	distance = sqrt(pow(directionX, 2) + pow(directionY, 2));

	//ii normalizam
	directionX /= distance;
	directionY /= distance;

	//if butonul mouse-ului a fost apasat
	if (move_astronaut == 1)
	{
		//calculam factorii de translatare si translatam astronautul
		temp_transX += directionX * deltaTimeSeconds * 500;
		temp_transY += directionY * deltaTimeSeconds * 500;
		modelMatrix *= Transform2D::Translate(temp_transX, temp_transY);

		//daca primim un nou punct petru translatare, oprim astronautul si calculam unghiul pentru rotatie
		//Centru triunghiului echilateral este situat la 2/3 fata de varf si 1/3 fata de baza => (triangleSide/2, 1/3 * triangleHight)
		//aducem aastronautul in pozitia initiala
		modelMatrix *= Transform2D::Translate(30, 1 / 3 * triangleHight);

		//conversie unghiului de roatie in radiani
		temp_angularStep = (astronaut_angularStep - 90)* M_PI / 180;

		//il rotim
		modelMatrix *= Transform2D::Rotate(temp_angularStep);

		//ducem astronautul in originie
		modelMatrix *= Transform2D::Translate(-30, -1 / 3 * triangleHight);

		//actualizam coordonatele varfurilor
		for (int i = 0; i < 3; i++) {
			triangle_tip[i].x += directionX * deltaTimeSeconds * 500;
			triangle_tip[i].y += directionY * deltaTimeSeconds * 500;
		}

		//verficam coliziunea intre doua laturi ale triunghiului(fara baza) si platforma de stationare(pe ambele laturi de stationare)
		//, extragand punctul de intersectie
		if (doIntersect(triangle_tip[1], triangle_tip[2], stationaryPlatformTips[0], stationaryPlatformTips[1]) == true &&
			stationary_state != 2)
		{
			stationary_state = 1;
			intersection = intersectionPoint(triangle_tip[1], triangle_tip[2], stationaryPlatformTips[0], stationaryPlatformTips[1]);
		}
		else if (doIntersect(triangle_tip[0], triangle_tip[2], stationaryPlatformTips[0], stationaryPlatformTips[1]) == true &&
			stationary_state != 2)
		{
			stationary_state = 1;
			intersection = intersectionPoint(triangle_tip[0], triangle_tip[2], stationaryPlatformTips[0], stationaryPlatformTips[1]);
		}
		else if (doIntersect(triangle_tip[1], triangle_tip[2], stationaryPlatformTips[2], stationaryPlatformTips[3]) == true &&
			stationary_state != 2)
		{
			stationary_state = 3;
			intersection = intersectionPoint(triangle_tip[1], triangle_tip[2], stationaryPlatformTips[2], stationaryPlatformTips[3]);
		}
		else if (doIntersect(triangle_tip[0], triangle_tip[2], stationaryPlatformTips[2], stationaryPlatformTips[3]) == true &&
			stationary_state != 2)
		{
			stationary_state = 3;
			intersection = intersectionPoint(triangle_tip[0], triangle_tip[2], stationaryPlatformTips[2], stationaryPlatformTips[3]);
		}

		//verficam coliziunea intre doua laturi ale triunghiului( fara baza) si platforma de reflexie( pe ambele laturi de reflexie)
		//, extragand punctul de intersectie
		else if (doIntersect(triangle_tip[1], triangle_tip[2], reflectionPlatformTips[2], reflectionPlatformTips[3]) == true &&
			reflection_state != 2)
		{
			reflection_state = 1;
			intersection = intersectionPoint(triangle_tip[1], triangle_tip[2], reflectionPlatformTips[2], reflectionPlatformTips[3]);
		}
		else if (doIntersect(triangle_tip[0], triangle_tip[2], reflectionPlatformTips[2], reflectionPlatformTips[3]) == true &&
			reflection_state != 2)
		{
			reflection_state = 1;
			intersection = intersectionPoint(triangle_tip[0], triangle_tip[2], reflectionPlatformTips[2], reflectionPlatformTips[3]);
		}
		else if (doIntersect(triangle_tip[1], triangle_tip[2], reflectionPlatformTips[0], reflectionPlatformTips[1]) == true &&
			reflection_state != 2)
		{
			reflection_state = 1;
			intersection = intersectionPoint(triangle_tip[1], triangle_tip[2], reflectionPlatformTips[0], reflectionPlatformTips[1]);
		}
		else if (doIntersect(triangle_tip[0], triangle_tip[2], reflectionPlatformTips[0], reflectionPlatformTips[1]) == true &&
			reflection_state != 2) {
			reflection_state = 1;
			intersection = intersectionPoint(triangle_tip[0], triangle_tip[2], reflectionPlatformTips[0], reflectionPlatformTips[1]);
		}

		//verficam coliziunea intre doua laturi ale triunghiului(fara baza) si marginile scenei
		//, extragand punctul de intersectie
		else if (doIntersect(triangle_tip[0], triangle_tip[2], sceneCorners[0], sceneCorners[1]) == true) {
			reflection_state = 3;
			intersection = intersectionPoint(triangle_tip[0], triangle_tip[2], sceneCorners[0], sceneCorners[1]);
		}
		else if (doIntersect(triangle_tip[1], triangle_tip[2], sceneCorners[0], sceneCorners[1]) == true) {
			reflection_state = 3;
			intersection = intersectionPoint(triangle_tip[1], triangle_tip[2], sceneCorners[0], sceneCorners[1]);
		}
		else if (doIntersect(triangle_tip[0], triangle_tip[2], sceneCorners[1], sceneCorners[2]) == true && reflection_state != 5) {
			reflection_state = 4;
			intersection = intersectionPoint(triangle_tip[0], triangle_tip[2], sceneCorners[1], sceneCorners[2]);
		}
		else if (doIntersect(triangle_tip[1], triangle_tip[2], sceneCorners[1], sceneCorners[2]) == true && reflection_state != 5) {
			reflection_state = 4;
			intersection = intersectionPoint(triangle_tip[1], triangle_tip[2], sceneCorners[1], sceneCorners[2]);
		}
		else if (doIntersect(triangle_tip[0], triangle_tip[2], sceneCorners[2], sceneCorners[3]) == true) {
			reflection_state = 3;
			intersection = intersectionPoint(triangle_tip[0], triangle_tip[2], sceneCorners[2], sceneCorners[3]);
		}
		else if (doIntersect(triangle_tip[1], triangle_tip[2], sceneCorners[2], sceneCorners[3]) == true) {
			reflection_state = 3;
			intersection = intersectionPoint(triangle_tip[1], triangle_tip[2], sceneCorners[2], sceneCorners[3]);
		}
		else if (doIntersect(triangle_tip[0], triangle_tip[2], sceneCorners[0], sceneCorners[3]) == true && reflection_state != 5) {
			reflection_state = 4;
			intersection = intersectionPoint(triangle_tip[0], triangle_tip[2], sceneCorners[0], sceneCorners[3]);
		}
		else if (doIntersect(triangle_tip[1], triangle_tip[2], sceneCorners[0], sceneCorners[3]) == true && reflection_state != 5) {
			reflection_state = 4;
			intersection = intersectionPoint(triangle_tip[1], triangle_tip[2], sceneCorners[0], sceneCorners[3]);
		}

		//verficam coliziunea intre triunghiului si fiecare tip de asteroid din scena
		else if (astronaut_vs_asteroid(transAsteroidCircle, triangle_tip[0], triangle_tip[1], triangle_tip[2]) == true && eraseTranslatingAsteroid == 0) {
			eraseTranslatingAsteroid = 1;
			reflection_state = 7;
		}
		else if (astronaut_vs_asteroid(scalingAsteroidCircle, triangle_tip[0], triangle_tip[1], triangle_tip[2]) == true && eraseScalingAsteroid == 0) {
			eraseScalingAsteroid = 1;
			reflection_state = 7;
		}
		else if (astronaut_vs_asteroid(rotatingAsteroidCircle, triangle_tip[0], triangle_tip[1], triangle_tip[2]) == true && eraseRotatingAsteroid == 0) {
			eraseRotatingAsteroid = 1;
			reflection_state = 7;
		}

		//verifica coliziunea triunghiului cu platforma de finish
		else if (doIntersect(triangle_tip[0], triangle_tip[2], finishPlatformTips[0], finishPlatformTips[1]) == true ||
			doIntersect(triangle_tip[0], triangle_tip[2], finishPlatformTips[2], finishPlatformTips[3]) == true ||
			doIntersect(triangle_tip[1], triangle_tip[2], finishPlatformTips[0], finishPlatformTips[1]) == true ||
			doIntersect(triangle_tip[1], triangle_tip[2], finishPlatformTips[2], finishPlatformTips[3]) == true) {
			exit(0);
		}
	}

	//daca astronautul a lovit o platforma de stationare in latura din partea de jos
	if (stationary_state == 1) {
		//translatam obiectul in punctul de intersectie => vine cu varful in sus
		modelMatrix *= Transform2D::Translate(intersection.x + triangleSide, intersection.y);
		//=> il rotim la 180 de grade
		modelMatrix *= Transform2D::Rotate(M_PI);
		move_astronaut = 0;
	}
	//daca astronautul a lovit platforma de stationare in latura de sus
	else if (stationary_state == 3) {
		modelMatrix *= Transform2D::Translate(intersection.x - triangleSide, intersection.y);
		move_astronaut = 0;
	}
	//daca astronautul a lovit o platforma de reflexie aflata la orizontala
	else if (reflection_state == 1 || reflection_state == 3) {
		//proiectam pozitia actuala a astronautului fata de normala la suprafata
		//, care este paralela cu Oy
		reflectedX = intersection.x + sin(-temp_angularStep) * distance;
		if (startY <= 720 && startY >= 0) {
			reflectedY = startY;
		}
		else if (startY < 0) {
			reflectedY = 0;
		}
		else reflectedY = 720;
		//modificam directia de deplasare a astronautului
		startX = intersection.x;
		startY = intersection.y;
		translateX = reflectedX - 30;
		translateY = reflectedY - 1 / 3 * triangleHight;

		//calculam unghiul cu care se va roti dupa reflexie
		astronaut_angularStep = atan2(translateY - startY, translateX - startX);
		astronaut_angularStep *= 180 / M_PI;
		if (astronaut_angularStep < 0)
		{
			astronaut_angularStep = 360 - (-astronaut_angularStep);
		}
		if (reflection_state == 3) {
			reflection_state = 6;
		}
		else reflection_state = 2;
		stationary_state = 0;
	}
	//daca astronautul loveste o platforma de reflexie aflata la verticala
	else if (reflection_state == 4) {
		//proiectam pozitia actuala a astronautului fata de normala la suprafata,
		//care va fi paralela cu Ox
		if (startX <= 1280 && startX > 0) {
			reflectedX = startX;
		}
		else if (startX < 0) {
			reflectedX = 0;
		}
		else reflectedX = 1280;
		reflectedY = intersection.y + sin(abs(temp_angularStep)) * distance;

		//modificam directia de deplasare a astronautului
		startX = intersection.x;
		startY = intersection.y;
		translateX = reflectedX - 30;
		translateY = reflectedY - 1 / 3 * triangleHight;

		//calculam unghiul sub care se va deplasa astronautul dupa reflexie
		astronaut_angularStep = atan2(translateY - startY, translateX - startX);
		astronaut_angularStep *= 180 / M_PI;
		if (astronaut_angularStep < 0)
		{
			astronaut_angularStep = 360 - (-astronaut_angularStep);
		}
		reflection_state = 5;
		stationary_state = 0;
	}
	else if (reflection_state == 7) {
		reflection_state = 8;
		startX = triangle_tip[2].x;
		startY = triangle_tip[2].y;
		translateX = 1280 - translateX;
		translateY = 720 - translateY;
		//calculam unghiul sub care se va deplasa astronautul dupa coliiune
		astronaut_angularStep = atan2(translateY - startY, translateX - startX);
		astronaut_angularStep *= 180 / M_PI;
	}

	//daca nu am atins platforma de finish, desenam astronautul
	RenderMesh2D(meshes["astronaut"], shaders["VertexColor"], modelMatrix);

	//daca nu am atins platforma de finish, desenam platforma de stationare
	modelMatrix = glm::mat3(1);
	RenderMesh2D(meshes["stationaryPlatform"], shaders["VertexColor"], modelMatrix);

	//daca nu am atins platforma de finish, desenam platforma de reflexie
	modelMatrix = glm::mat3(1);
	RenderMesh2D(meshes["reflectionPlatform"], shaders["VertexColor"], modelMatrix);

	//daca nu am atins platforma de finish, desenam platforma de finish
	modelMatrix = glm::mat3(1);
	RenderMesh2D(meshes["finishPlatform"], shaders["VertexColor"], modelMatrix);

	//asteroid cu translatie care se va deplasa stanga-dreapta pe Ox
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(200, 200);
	if (curr_dist < 300 && curr_dist >= 0) {
		asteroid_translateX += deltaTimeSeconds * 50 * sign;
		curr_dist = 200 + asteroid_translateX;
		for (int i = 0; i < 9; i++) {
			translatingAsteroidTips[i].x += deltaTimeSeconds * 50 * sign;
			transAsteroidCircle.radius = triangleSide;
			transAsteroidCircle.centerX = intersectionPoint(translatingAsteroidTips[0], translatingAsteroidTips[5], translatingAsteroidTips[2], translatingAsteroidTips[7]).x;
			transAsteroidCircle.centerY = intersectionPoint(translatingAsteroidTips[0], translatingAsteroidTips[5], translatingAsteroidTips[2], translatingAsteroidTips[7]).y;
		}
	}
	else  if (curr_dist >= 300) {
		sign = -1;
		curr_dist = 0;
	}
	else if (curr_dist < 0) {
		sign = 1;
		curr_dist = 0;
	}

	//desenam asteroidul cu translatie, daca nu am intersectat astronautul
	modelMatrix *= Transform2D::Translate(asteroid_translateX, 200);
	if (eraseTranslatingAsteroid == 0) {
		RenderMesh2D(meshes["translatingAsteroid"], shaders["VertexColor"], modelMatrix);
	}

	//astroid cu scalare va creste pana la o anumita dimensiune, dupa care va
	//scade pana la 0
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(400, 200);
	if (curr_scale < 400 && curr_scale >= 0) {
		scaleX += deltaTimeSeconds * 0.3 * sign2;
		scaleY += deltaTimeSeconds * 0.3 * sign2;
		curr_scale += scaleX;
		for (int i = 0; i < 9; i++) {
			scalingAsteroidTips[i].x *= deltaTimeSeconds * 0.3 * sign2;
			scalingAsteroidTips[i].y *= deltaTimeSeconds * 0.3 * sign2;
		}
		scalingAsteroidCircle.radius = triangleSide * scaleX;
	}
	else  if (curr_scale >= 400) {
		sign2 = -1;
		curr_scale = 0;
	}
	else if (curr_scale < 0) {
		sign2 = 1;
		curr_scale = 0;
	}
	modelMatrix *= Transform2D::Translate(200, 50);
	modelMatrix *= Transform2D::Scale(scaleX, scaleY);
	modelMatrix *= Transform2D::Translate(-200, -50);

	//daca astronautul nu a atins asteroidul ce 
	//scaleaza, atunci il desenam pe  cel din urma
	if (eraseScalingAsteroid == 0) {
		RenderMesh2D(meshes["scalingAsteroid"], shaders["VertexColor"], modelMatrix);
	}

	//asteroid cu rotatie
	modelMatrix = glm::mat3(1);
	asteroid_angularStep += deltaTimeSeconds;
	modelMatrix *= Transform2D::Translate(800, 400);
	modelMatrix *= Transform2D::Translate(200, 50);
	modelMatrix *= Transform2D::Rotate(asteroid_angularStep);
	modelMatrix *= Transform2D::Translate(-200, -50);

	//daca astronautul nu a atins asteroidul ce
	//se roteste, atunci il desenam
	if (eraseRotatingAsteroid == 0) {
		RenderMesh2D(meshes["rotatingAsteroid"], shaders["VertexColor"], modelMatrix);
	}
}

void AstroKitty::FrameEnd()
{

}

void AstroKitty::OnInputUpdate(float deltaTime, int mods)
{

}

void AstroKitty::OnKeyPress(int key, int mods)
{
	// add key press event
}

void AstroKitty::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void AstroKitty::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void AstroKitty::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) == true)
	{
		if (move_astronaut == 0) {
			move_astronaut = 1;
			if (stationary_state == 1 || stationary_state == 3) {
				stationary_state = 2;
				reflection_state = 0;
			}
			else stationary_state = 0;

			reflection_state = 0;
			startX = temp_transX;
			startY = temp_transY;
			translateX = mouseX - 30;
			translateY = 720 - mouseY - 1 / 3 * triangleHight;
			astronaut_angularStep = atan2(translateY - temp_transY, translateX - temp_transX);
			//conversie in grade
			astronaut_angularStep = astronaut_angularStep * 180 / M_PI;
		}
	}
}

void AstroKitty::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void AstroKitty::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void AstroKitty::OnWindowResize(int width, int height)
{
}
