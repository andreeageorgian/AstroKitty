#include "Object2D.h"

#include <Core/Engine.h>

Mesh* Object2D::CreateTriangle(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	//setezi coordonatele coltului stagna jos
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), glm::vec3(0.5f ,0.5f, 0)),
		VertexFormat(corner + glm::vec3(length/2, length * sqrt(3) / 2, 0), glm::vec3(0.75f, 1, 0))
	};

	//3 colturi => 3 indici???
	Mesh* triangle = new Mesh(name);
	std::vector<unsigned short> indices = {0, 1, 2};

	if (!fill) {
		triangle->SetDrawMode(GL_TRIANGLE_STRIP);
	}

	triangle->InitFromData(vertices, indices);
	return triangle;
}

Mesh* Object2D::CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float width, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, width, 0), color),
		VertexFormat(corner + glm::vec3(0, width, 0), color)
	};

	Mesh* rectangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		rectangle->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	rectangle->InitFromData(vertices, indices);
	return rectangle;
}


//nu merge bine daca fill este true
Mesh* Object2D::CreateDecagon(std::string name, float length, glm::vec3 color, bool fill)
{
	//un triunghi dintr-un decagon are varful de sus de 36 de grade + este isoscel => 
	//celelalte unghiuri vor avea 72 de grade(1.25663706 in radiani)
	double triangleSide = (length / 2) / cos(1.25663706);
	
	//inaltimea unui triunghi
	float triangleHight = sqrt((triangleSide*triangleSide) - ((length*length) / 4));
	glm::vec3 corner = glm::vec3 (200 + triangleHight*cos(0), 50 + triangleHight*sin(0),0);

	std::vector<VertexFormat> vertices =
	{
		//triangleHight*cos((2*M_PI)/10) unde unghiul de la cos este (i * 2 * pi)/nr de laturi ale poligonului
		//iar i merge de la 0 pana la numarul de laturi ale poligonului - 1
		//(246.165,50,0)
		VertexFormat(corner, color),
		VertexFormat(glm::vec3(200 + triangleHight*cos((2*M_PI)/10), 50 + triangleHight*sin((2*M_PI)/10), 0), color),
		VertexFormat(glm::vec3(200 + triangleHight*cos((4*M_PI) / 10), 50 + triangleHight*sin((4* M_PI) / 10), 0), color),
		VertexFormat(glm::vec3(200 + triangleHight*cos((6*M_PI) / 10), 50 + triangleHight*sin((6* M_PI) / 10), 0), color),
		VertexFormat(glm::vec3(200 + triangleHight*cos((8*M_PI) / 10), 50 + triangleHight*sin((8 * M_PI) / 10), 0), color),
		VertexFormat(glm::vec3(200 + triangleHight*cos((10*M_PI) / 10), 50 + triangleHight*sin((10 * M_PI) / 10), 0), color),
		VertexFormat(glm::vec3(200 + triangleHight*cos((12*M_PI) / 10), 50 + triangleHight*sin((12 * M_PI) / 10), 0), color),
		VertexFormat(glm::vec3(200 + triangleHight*cos((14*M_PI) / 10), 50 + triangleHight*sin((14 * M_PI) / 10), 0), color),
		VertexFormat(glm::vec3(200 + triangleHight*cos((16*M_PI) / 10), 50 + triangleHight*sin((16 * M_PI) / 10), 0), color),
		VertexFormat(glm::vec3(200 + triangleHight*cos((18*M_PI) / 10), 50 + triangleHight*sin((18 * M_PI) / 10), 0), color)
	};

	Mesh* decagon = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2 , 3 , 4, 5, 6 , 7, 8, 9};

	if (!fill) {
	decagon->SetDrawMode(GL_TRIANGLE_FAN);
	}

	decagon->InitFromData(vertices, indices);
	return decagon;
}