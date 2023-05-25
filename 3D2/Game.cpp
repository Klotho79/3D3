#include "Game.h"


//Static functiom

// Initializing functions
void Game::InitWindow()
{
	this->window = new sf::RenderWindow(sf::VideoMode(800,600), "Game SFML RPG");
}

void Game::load3DObject()
{

}
void Game::initCube()
{
	
	addTriangle(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,mesh);
	addTriangle(0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,mesh);
	//east
	addTriangle(1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,mesh);
	addTriangle(1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,mesh);
	//north
	addTriangle(1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,mesh);
	addTriangle(1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,mesh);
	//west
	addTriangle(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,mesh);
	addTriangle(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,mesh);
	//top
	addTriangle(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,mesh);
	addTriangle(0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,mesh);
	//down
	addTriangle(1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,mesh);
	addTriangle(1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,mesh);
	for (auto &i:mesh.tris)
	{
		for (int a = 0; a < 3; a++)
		{
			i.p[a].x -= 0.5f; i.p[a].y -= 0.5f; i.p[a].z -= 0.5f;
		}
	}

}

void Game::transformCube(const float& dt)
{
	theta += dt*2;
	mat4x4 perspProj;
	int poradi =0;
	triangl troj, transl, triRotatedZ, triRotatedZX, triRotatedZYX;
	
	for (int a = 0; a < 3; a++)
	{
		troj.p[a].x = 0.f; troj.p[a].y = 0.f; troj.p[a].z = 0.f;
	}

	matRotZ.m[0][0] = cosf(theta);
	matRotZ.m[0][1] = sinf(theta);
	matRotZ.m[1][0] = -sinf(theta);
	matRotZ.m[1][1] = cosf(theta);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;

	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cosf(theta * 0.5f);
	matRotX.m[1][2] = sinf(theta * 0.5f);
	matRotX.m[2][1] = -sinf(theta * 0.5f);
	matRotX.m[2][2] = cosf(theta * 0.5f);
	matRotX.m[3][3] = 1;

	matRotY.m[0][0] = cosf(theta * 0.5f);
	matRotY.m[0][2] = sinf(theta * 0.5f);
	matRotY.m[1][1] = 1;
	matRotY.m[2][0] = -sinf(theta * 0.5f);
	matRotY.m[2][2] = cosf(theta * 0.5f);
	matRotY.m[3][3] = 1;

	vect3D output = {0};
	perspProj = createPerspective(180.0f * M_PI / 180.0f, 1024.0f / 768.0f, 0.1f, 25.0f);
	for (auto& i : mesh.tris)
	{	
		
		MultipleMatrixVector(i.p[0], triRotatedZ.p[0], matRotZ);
		MultipleMatrixVector(i.p[1], triRotatedZ.p[1], matRotZ);
		MultipleMatrixVector(i.p[2], triRotatedZ.p[2], matRotZ);

		// Rotate in X-Axis
		MultipleMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
		MultipleMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
		MultipleMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

		//Rotate in Y-Axis
		MultipleMatrixVector(triRotatedZX.p[0], triRotatedZYX.p[0], matRotY);
		MultipleMatrixVector(triRotatedZX.p[1], triRotatedZYX.p[1], matRotY);
		MultipleMatrixVector(triRotatedZX.p[2], triRotatedZYX.p[2], matRotY);


		transl = triRotatedZX;
		transl.p[1].z = triRotatedZX.p[1].z + 3.0f;
		transl.p[2].z = triRotatedZX.p[2].z + 3.0f;
		transl.p[0].z = triRotatedZX.p[0].z + 3.0f;


		vect3D normal, line1, line2;

		line1.x = transl.p[1].x - transl.p[0].x;
		line1.y = transl.p[1].y - transl.p[0].y;
		line1.z = transl.p[1].z - transl.p[0].z;

		line2.x = transl.p[2].x - transl.p[0].x;
		line2.y = transl.p[2].y - transl.p[0].y;
		line2.z = transl.p[2].z - transl.p[0].z;

		normal.x = line1.y * line2.z - line1.z * line2.y;
		normal.y = line1.z * line2.x - line1.x * line2.z;
		normal.z = line1.x * line2.y - line1.y * line2.x;

		float l = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
		normal.x /= 1; normal.y /= 1; normal.z /= 1;
		//if (normal.z < 0)

		if(normal.x * (transl.p[0].x - vCamera.x)+
		   normal.y * (transl.p[0].y - vCamera.y)+
		   normal.z * (transl.p[0].z - vCamera.z) <0)
		{
			for (int x = 0; x < 3; x++)
			{
				MultipleMatrixVector(transl.p[x], output, perspProj);
				troj.p[x] = output;
				//std::cout << output.x << " " << output.y << " " << output.z << " ";
			}
		}
		//std::cout<< "\n" << poradi << ". ";
		line(troj.p[0].x * 80 + 300, troj.p[0].y * 80 + 300, troj.p[1].x * 80 + 300, troj.p[1].y * 80 + 300, sf::Color::Blue, vertices);
		line(troj.p[1].x * 80 + 300, troj.p[1].y * 80 + 300, troj.p[2].x * 80 + 300, troj.p[2].y * 80 + 300, sf::Color::Blue, vertices);
		line(troj.p[2].x * 80 + 300, troj.p[2].y * 80 + 300, troj.p[0].x * 80 + 300, troj.p[0].y * 80 + 300, sf::Color::Blue, vertices);
		fillTriang(troj.p[0].x * 80 + 300, troj.p[0].y * 80 + 300, troj.p[1].x * 80 + 300, troj.p[1].y * 80 + 300, troj.p[2].x * 80 + 300, troj.p[2].y * 80 + 300, vertices);
		std::cout << theta << "\n";
	}
}

//Constructors /Destructors

Game::Game()
{
	this->InitWindow();
	this->initCube();
	
	//for (auto& i : mesh.tris)
	//{
	//	line(i.p[0].x * 100 + 250, i.p[0].y * 100 + 250, i.p[1].x * 100 + 250, i.p[1].y * 100 + 250, vertices);
	//	line(i.p[1].x * 100 + 250, i.p[1].y * 100 + 250, i.p[2].x * 100 + 250, i.p[2].y * 100 + 250, vertices);
	//	line(i.p[2].x * 100 + 250, i.p[2].y * 100 + 250, i.p[0].x * 100 + 250, i.p[0].y * 100 + 250, vertices);	}
	///*line(50,120,530,75,vertices);
	//line(120, 530, 752, 245, vertices);*/

}


Game::~Game()
{
	delete this->window;
}


void Game::updateDt()
{
	this->dt = this->dtClock.restart().asSeconds();

}

void Game::updateSFMLEvents()
{
	
	while (this->window->pollEvent(this->sfEvent))
		{
			if (this->sfEvent.type == sf::Event::Closed)
			{
				this->window->close();
			}
		}
	}

void Game::update()
{	
	vertices.clear();
	this->updateSFMLEvents();
	transformCube(dt);
}

void Game::render()
{
	this->window->clear();
	// Render items
	this->window->draw(vertices);
	this->window->display();
}

void Game::run()
{
	while (this->window->isOpen())
	{
		this->updateDt();
		this->update();
		this->render();
	}
}
