#ifndef GAME_H
#define GAME_H

#include "graph.h"


class Game
{
private:
	sf::RenderWindow *window;
	sf::Event sfEvent;

	sf::Clock dtClock;
	float dt;

	// Initialitation
	void InitWindow(); 
	void load3DObject();
	void initCube();
	void transformCube(const float& dt);
	
public:
	float theta = 0.f;
	mat4x4 matRotZ, matRotX, matRotY;
	// z rotate
	vect3D vCamera;
	mesh mesh;
	sf::VertexArray vertices;

	//Constructors/Destructors
	Game();
	virtual ~Game();

	// Functions
	void updateDt();
	void updateSFMLEvents();
	void update();	
	void render();
	void run();

};

#endif