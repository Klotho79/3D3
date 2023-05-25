#define _USE_MATH_DEFINES
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <ctime>
#include <cstdlib>

#include <SFML/System.hpp>
#include <SFML/window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <math.h>
struct vect2D {
	float x, y;
};
struct vect3D {
	float x, y, z;
};

struct triangl
{
	vect3D p[3];
};

struct mesh
{
	std::vector<triangl> tris;
};
struct mat4x4
{
	float m[4][4] = { 0 };
};

std::vector<std::vector<float>> perspective_matrix(float fov, float aspect_ratio, float near_clip, float far_clip);

mat4x4 createPerspective(float fov, float aspect_ratio, float near_plane, float far_plane);
void MultipleMatrixVector(vect3D& i, vect3D& o, mat4x4& m);
void line(float x1, float y1, float x2, float y2, sf::Color color, sf::VertexArray& vertices);
void fillTris(float x1, float y1, float x2, float y2, float x3, float y3, sf::VertexArray& vertices);
void fillTriang(float x1, float y1, float x2, float y2, float x3, float y3, sf::VertexArray& vertices);
void fillTri(float x1, float y1, float x2, float y2, float x3, float y3, sf::VertexArray& vertices);
void fillTriangle(float x1, float y1, float x2, float y2, float x3, float y3, sf::VertexArray& vertices);
void addTriangle(const float& x1, const float& y1, const float& z1,
	const float& x2, const float& y2, const float& z2,
	const float& x3, const float& y3, const float& z3, mesh& x);

class graph
{

public:

	//Functions

};


#endif // !GRAPH_H