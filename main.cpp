#define c 1/20
#include <iostream>
#include "rasterisator.h"
#include "data_parser.h" 
#include <cmath> 
#include <SFML/Graphics.hpp>
//#include <thread>

Quaternion update(){
	float alpha = 0;
	float beta = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
		alpha = -0.05;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		alpha = 0.05;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		beta = 0.05;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		beta = -0.05;

	Quaternion y_rotate = {cos(alpha), {0, sin(alpha), 0}};
	Quaternion x_rotate = {cos(beta), {sin(beta), 0, 0}};

		
	return x_rotate * y_rotate;
}

			
float scale = 0.8;
void draw(Renderer &rend, Triangle* polygons,  int triangles_count){
	Quaternion m = update();
	if (m.alpha == 1) return;
	rend.clear();
	Vector3f vec, AB, AC;
	Triangle tr;
	float intens;
	sf::Color color;
	Vector3f dir = {0, 0, -1};
	//dir.norm();
//	std::cout << m << std::endl;
	for (int i =0; i < triangles_count; i++){
		tr = polygons[i];	

		tr.A = m * tr.A * m.conjugate();
		tr.B = m * tr.B * m.conjugate();
		tr.C = m * tr.C * m.conjugate();
		//tr.B = m * tr.B;
		//tr.C = m * tr.C;
		polygons[i] = tr;
		
		vec = v_mult(tr.B.vec - tr.A.vec, tr.C.vec - tr.A.vec);
		vec.norm();
		
		//dir = m * dir;
		intens = (fabs(s_mult(vec, dir)) * 255);

		//if (intens > 0){
			color = sf::Color(intens, intens, intens);


		
		
			Vector3f A, B, C;
			A = tr.A.vec;
			B = tr.B.vec;
			C = tr.C.vec;
			float Acoef = 1/(1.f - A.z * c);
			float Bcoef = 1/(1.f - B.z * c);
			float Ccoef = 1/(1.f - C.z * c);
			A = {500 + scale * 400 * A.x * Acoef, 300 + scale * 300 * A.y * Acoef, 300 + scale * 300 * A.z * Acoef};
			B = {500 + scale * 400 * B.x * Bcoef, 300 + scale * 300 * B.y * Bcoef, 300 + scale * 300 * B.z * Bcoef};
			C = {500 + scale * 400 * C.x * Ccoef, 300 + scale * 300 * C.y * Ccoef, 300 + scale * 300 * C.z * Ccoef};

		/*	A = {200 + 400 * tr.A.x, 300 + 300 * tr.A.y, 300 + 300 * tr.A.z};
			B = {200 + 400 * tr.B.x, 300 + 300 * tr.B.y, 300 + 300 * tr.B.z};
			C = {200 + 400 * tr.C.x, 300 + 300 * tr.C.y, 300 + 300 * tr.C.z};*/
		
	
			rend.triangle(A, B, C, color);
		//}
	}
}





int main(){
	DataExtractor pars = DataExtractor("dif_out1.txt");



	sf::RenderWindow window(sf::VideoMode (1000, 800), "My window");

	Renderer rend;
	sf::Texture texture;
	texture.create(1000, 800);
	sf::Sprite spr = sf::Sprite(texture);




	while (window.isOpen()){
		sf::Event event;
		while(window.pollEvent(event)){
			if (event.type == sf::Event::Closed){
				window.close();
				std::cout << "close" << std::endl;
				break;
			}
		//	std::cout << "qwerty" << std::endl;
			window.clear();
			//Mat m = Mat({1,0,0}, {0, 1, 0}, {0,0,1});
			//Mat m = update();
			draw(rend, pars.polygons, pars.triangles_count);
			texture.update(*(rend.image));
			spr.setTexture(texture);
			window.draw(spr);
			window.display();
		}
	}

	return 0;
}

