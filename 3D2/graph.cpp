#include "graph.h"

std::vector<std::vector<float>> perspective_matrix(float fov, float aspect_ratio, float near_clip, float far_clip) 
{
	float fov_rad = 1.0f / tanf(fov * 0.5f / 180.0f * 3.14159f);

	std::vector<std::vector<float>> matrix = {
		{fov_rad / aspect_ratio, 0, 0, 0},
		{0, fov_rad, 0, 0},
		{0, 0, far_clip / (far_clip - near_clip), 1},
		{0, 0, (-far_clip * near_clip) / (far_clip - near_clip), 0}
	};

	return matrix;
}

//vect3D project_vertex(vect3D vertex, std::vector<std::vector<float>> projection_matrix) {
//	float w = vertex.x * projection_matrix[0][3] + vertex.y * projection_matrix[1][3] + vertex.z * projection_matrix[2][3] + projection_matrix[3][3];
//	vect3D projected_vertex(
//		(vertex.x * projection_matrix[0][0] + vertex.y * projection_matrix[1][0] + vertex.z * projection_matrix[2][0] + projection_matrix[3][0]) / w,
//		(vertex.x * projection_matrix[0][1] + vertex.y * projection_matrix[1][1] + vertex.z * projection_matrix[2][1] + projection_matrix[3][1]) / w,
//		(vertex.x * projection_matrix[0][2] + vertex.y * projection_matrix[1][2] + vertex.z * projection_matrix[2][2] + projection_matrix[3][2]) / w
//	);
//	return projected_vertex;
//}

mat4x4 createPerspective(float fov, float aspect_ratio, float near_plane, float far_plane)
{
	mat4x4 result;

	float tan_half_fov = tan(fov / 2.0f);
	float fFovRad = 1.0f / tanf(far_plane * 0.5f / 180.0f * 3.14159f);
	float range = far_plane - near_plane;

	result.m[0][0] =  aspect_ratio* fFovRad;
	result.m[1][1] = fFovRad;
	result.m[2][2] = far_plane / range;
	result.m[3][2] =  -far_plane * near_plane / range;
	result.m[2][3] = 1.0f;
	result.m[3][3] = 0.f;

	return result;
}

void MultipleMatrixVector(vect3D& i, vect3D& o, mat4x4& m)
{
	o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
	o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
	o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
	float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

	if (w != 0.f)
	{
		o.x /= w; o.y /= w; o.z /= w;
	}
	//std::cout << "x " << o.x << " " << "y " << o.y << " " << "z " << o.z << "\n";
}

void line(float x1, float y1, float x2, float y2, sf::Color color, sf::VertexArray& vertices)
{
	//std::cout << "x1 " << x1 << " y1 " << y1 << " x2 " << x2 << " y2 " << y2 << "\n";
	sf::Vertex vertex;
	float dx = (x2 - x1);
	float dy = (y2 - y1);
	float lenght = std::sqrt(dx * dx + dy * dy);
	float angle = std::atan2(dy, dx);
	for (float i = 0; i < lenght; i++)
	{
		vertex.position = sf::Vector2f(x1 + std::cos(angle) * i, y1 + std::sin(angle) * i);
		vertex.color = color;
		vertices.append(vertex);
	}
}
void fillTriang(float x1, float y1, float x2, float y2, float x3, float y3, sf::VertexArray& vertices)
{
	//sf::Vertex vertex;
	// Urèení minimálního a maximálního Y-ového souøadnice trojúhelníku
	int minY = std::min(std::min(y1, y2), y3);
	int maxY = std::max(std::max(y1, y2), y3);

	// Iterace pøes každou scanline (øádek) uvnitø trojúhelníku
	for (int y = minY; y <= maxY; y++) {
		// Urèení levého a pravého X-ového ohranièení na aktuální scanline
		int leftX = std::numeric_limits<int>::max();
		int rightX = std::numeric_limits<int>::min();

		// Procházení každého hranového segmentu trojúhelníku
		if (y >= y1 && y <= y2) {
			// Hrana mezi vertex1 a vertex2
			int x = x1 + (y - y1) * (x2 - x1) / (y2 - y1);
			leftX = std::min(leftX, x);
			rightX = std::max(rightX, x);
			line(leftX, y, rightX, y, sf::Color::White, vertices);
			
		}

		if (y >= y2 && y <= y3) {
			// Hrana mezi vertex2 a vertex3
			int x = x2 + (y - y2) * (x3 - x2) / (y3 - y2);
			leftX = std::min(leftX, x);
			rightX = std::max(rightX, x);
			line(leftX, y, rightX, y, sf::Color::White, vertices);
		}

		if (y >= y1 && y <= y3) {
			// Hrana mezi vertex1 a vertex3
			int x = x1 + (y - y1) * (x3 - x1) / (y3 - y1);
			leftX = std::min(leftX, x);
			rightX = std::max(rightX, x);
			line(leftX,y,rightX,y, sf::Color::White, vertices);

		}

		// Vyplnìní scanline mezi levým a pravým ohranièením
		//for (int x = leftX; x <= rightX; x++) {
		//	std::cout << "Pixel (" << x << ", " << y << ") vyplnìn" << std::endl;
		//	// Místo tisku mùžete provádìt další operace s vyplnìnými pixely
		//}
	}
}


void fillTri(float x1,float y1, float x2, float y2, float x3, float y3, sf::VertexArray& vertices)
{
	//sf::Vertex vertex;
	float tx,ty,vx,vy;
	
	int	counter;
		//get length of all sides
	float	d1 = sqrt(((y2 - y1) * (y2 - y1)) + ((x2 - x1) * (x2 - x1)));
	float	d2 = sqrt(((y3 - y2) * (y3 - y2)) + ((x3 - x2) * (x3 - x2)));
	float	d3 = sqrt(((y1 - y3) * (y1 - y3)) + ((x1 - x3) * (x1 - x3)));
	if (((d1 < d2) or (d1 = d2)) and ((d1 < d2) or (d1 = d2)))
	{
		float	tx = x1;
		float	ty = y1;
		float	vx = (x2 - x1) / d1;
		float	vy = (y2 - y1) / d1;
				counter = 0;
				while (counter < d1)
				{
					line(x3, y3, tx, ty,sf::Color::White, vertices);
					//drawing a line from point(x3,y3) to point(tx,ty).
					tx = tx + vx;
					ty = ty + vy;
					counter = counter + 1;
				}
	}
	else if ((d2 < d3) or (d2 = d3))
	{
		float tx = x2;
		float ty = y2;
		float vx = (x3 - x2) / d2;
		float vy = (y3 - y2) / d2;
		counter = 0;
	
		while (counter < d2)
		{
			line(x1, y1, tx, ty, sf::Color::White, vertices);
			tx = tx + vx;
			ty = ty + vy;
			counter = counter + 1;
		}
	}
	else
	{
		tx = x3;
		ty = y3;
		vx = (x1 - x3) / d3;
		vy = (y1 - y3) / d3;
		counter = 0;
		while (counter < d3)
		{
			line(x2, y2, tx, ty, sf::Color::White, vertices);
			tx = tx + vx;
			ty = ty + vy;
			counter = counter + 1;
		}
	}
}

void fillTriangle(float x1, float y1, float x2, float y2, float x3, float y3, sf::VertexArray& vertices)
{
	sf::Vertex vertex;
	float x, y;
	if (y1 > y2) {
				std::swap(y1, y2);
				std::swap(x1, x2);
			}
			if (y1 > y3) {
				std::swap(y1, y3);
				std::swap(x1, x3);
			}
			if (y2 > y3) {
				std::swap(y2, y3);
				std::swap(x2, x3);
			}
	
	float dx = (x3 - x2);
	float dy = (y3 - y2);
	float lenght = std::sqrt(dx * dx + dy * dy);
	float angle = std::atan2(dy, dx);

			for (int i = 0; i < y3; i++)
			{
				x = x2 + std::cos(angle) * i;
				y = y2 + std::sin(angle) * i;
				vertex.position = sf::Vector2f(x, y);
				vertex.color = sf::Color::White;
				vertices.append(vertex);
			}

}
void fillTris(float x1, float y1, float x2, float y2, float x3, float y3, sf::VertexArray& vertices)
{
	int minY = std::min(std::min(y1, y2), y3);
	int maxY = std::max(std::max(y1, y2), y3);

	for (int y = minY; y <= maxY; y++)
	{
		float leftX = std::numeric_limits<float>::max();
		float rightX = std::numeric_limits<float>::min();

		if (y >= y1 && y <= y2)
		{
			float x = x1 + (y - y1) * (x2 - x1) / (y2 - y1);
			leftX = std::min(leftX, x);
			rightX = std::max(rightX, x);
			line(leftX, y, rightX, y, sf::Color::White, vertices);
		}

		if (y >= y2 && y <= y3)
		{
			float x = x2 + (y - y2) * (x3 - x2) / (y3 - y2);
			leftX = std::min(leftX, x);
			rightX = std::max(rightX, x);
			line(leftX, y, rightX, y, sf::Color::White, vertices);
		}

		if (y >= y1 && y <= y3)
		{
			float x = x1 + (y - y1) * (x3 - x1) / (y3 - y1);
			leftX = std::min(leftX, x);
			rightX = std::max(rightX, x);
			line(leftX, y, rightX, y, sf::Color::White, vertices);
		}
	}
}
void addTriangle(const float& x1, const float& y1, const float& z1,
	const float& x2, const float& y2, const float& z2,
	const float& x3, const float& y3, const float& z3,mesh& x)
{
	x.tris.push_back({ x1,y1,z1, x2,y2,z2, x3,y3,z3 });
}