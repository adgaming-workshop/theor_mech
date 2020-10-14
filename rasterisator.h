#ifndef __RASTERISATOR__
#define __RASTERISATOR__

#include <SFML/Graphics.hpp>
#include "primitives.h"


class Renderer{
public:
	sf::Image *image;
	int size_x = 1000;
	int size_y = 800;
	int *z_buffer;

	void triangle(Vector3f A, Vector3f B, Vector3f C, sf::Color color);
	void clear();
	Renderer();
	~Renderer();
};


#endif // __RASTERISATOR__
