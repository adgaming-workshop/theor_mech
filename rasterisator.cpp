#include "rasterisator.h"
#include <iostream>
#include <cmath>

Renderer::Renderer(){
	this->image = new sf::Image;
	(*this->image).create(size_x, size_y);
	this->z_buffer = new int[size_x * size_y];
	for (int i = 0; i < size_x * size_y; i++){
		z_buffer[i] = -1;
	}
}


/*void Renderer::triangle(Vector3f A, Vector3f B, Vector3f C, sf::Color color){
	if (A.y > B.y) 
		std::swap(A, B);
	if (A.y > C.y) 
		std::swap(A, C);
	if (B.y > C.y) 
		std::swap(B, C);

	float total_height = C.y-A.y;
	Vector3f point1;
	Vector3f point2;
        	
	bool second_half = false;
    	for (int i = 0; i <= total_height; i+=1) {
		if (i >= B.y - A.y)
			second_half = true;
		
		float segment_height = second_half ? C.y-B.y : B.y-A.y;
		if (segment_height == 0){
			segment_height = 1;
		}
		if (total_height == 0){
			total_height = 1;
		}
        	float alpha = (float)i / total_height;
        	float beta  = (i - (second_half ? B.y - A.y : 0)) / segment_height; // be careful with divisions by zero
		point1 = A + (C - A) * alpha;
		point2 = second_half ? B + (C - B) * beta : A + (B - A) * beta;
		if (point1.x > point2.x) 
			std::swap(point1, point2);
  	        for (int j = (int)point1.x; j <= (int)point2.x; j+=1) {
			float phi = point2.x==point1.x ? 1. : (j-point1.x)/(point2.x-point1.x);
			Vector3f P = point1 + phi * (point2 - point1);
			   
			if (P.x <= size_x && P.y <= size_y && P.x >= 0 && P.y >= 0){
				int idx = (int)P.x + (int)P.y * size_x;
				if (this->z_buffer[idx] <= (int)P.z){
					this->z_buffer[idx] = (int)P.z;
					(*this->image).setPixel(P.x, P.y, color);
				}
			}
        		
		}

	}

}*/


void Renderer::triangle(Vector3f A, Vector3f B, Vector3f C, sf::Color color){
	int min_x, min_y, max_x, max_y;
	min_x = (int)A.x;
	if (min_x > B.x)
		min_x = (int)B.x;
	if (min_x > C.x)
		min_x = (int)C.x;

	min_y = (int)A.y;
	if (min_y > B.y)
		min_y = (int)B.y;
	if (min_y > C.y)
		min_y = (int)C.y;

	max_x = (int)A.x;
	if (max_x < B.x)
		max_x = (int)B.x;
	if (max_x < C.x)
		max_x = (int)C.x;

	max_y = (int)A.y;
	if (max_y < B.y)
		max_y = (int)B.y;
	if (max_y < C.y)
		max_y = (int)C.y;

	Vector3f AB, AC;
	AB = B - A;
	AC = C - A;
	Vector3f a, b;
	Vector3f bar;
			
	a = {AB.x, AC.x, A.x};
	b = {AB.y, AC.y, A.y};

	//if ((AB.x == 0 && AB.y == 0) || (AC.x == 0 && AC.y == 0)) return;
	for (int i = min_y; i <= max_y; i += 1){
		for (int j = min_x; j <= max_x; j += 1){
			a.z = A.x - j;
			b.z = A.y - i;

			//bar = v_mult(a, b);
			bar.z = a.x * b.y - a.y * b.x;
			bar.x = (a.y * b.z - a.z * b.y) / bar.z;
			bar.y = (-a.x * b.z + a.z * b.x) / bar.z;
			//bar.x /= bar.z;
			//bar.y /= bar.z;
			bar.z = 1;
		//	bar = (1 / bar.z) * bar;


			if (bar.x <0 || bar.y < 0) continue;
			float pz = A.z + bar.x * AB.z + bar.y * AC.z;
			int idx = j + i * this->size_x;
			if (idx < 0) continue;
			if (this->z_buffer[idx] < (int) pz){
				this->z_buffer[idx] = (int)pz;
				(*this->image).setPixel(j, i, color);
			
			}
		}
	}
}







Renderer::~Renderer(){
	delete this->z_buffer;
	delete this->image;
}

void Renderer::clear(){
	delete this->image;
	this->image = new sf::Image;
	(*this->image).create(size_x, size_y);
	for (int i = 0; i < size_x * size_y; i++){
		z_buffer[i] = -1;
	}
}

