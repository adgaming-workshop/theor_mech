#include <iostream>
#include "data_parser.h"
#include <fstream>
#include <cmath>

DataExtractor::DataExtractor(std::string file_name){
	std::ifstream fin;
	fin.open(file_name);
	
	int lines_count1, lines_count2;
	fin >> lines_count1 >> lines_count2;
	std::cout << lines_count1 << " " << lines_count2 << std::endl;

	this->polygons = new Triangle[lines_count1 * (lines_count2 - 1) * 2];
	this->triangles_count = lines_count1 * (lines_count2 - 1) * 2;

	float **points = new float*[lines_count1 * lines_count2];
	for (int i = 0; i < lines_count1 * lines_count2; i++){
		points[i] = new float[3];
	}

	float max_x = -1000000;
	float max_y = -1000000;
	float max_z = -1000000;

	float min_x = 1000000;
	float min_y = 1000000;
	float min_z = 1000000;

	float abs_max_x = 0;
	float abs_max_y = 0;
	float abs_max_z = 0;

	for (int i = 0; i < lines_count1 * lines_count2; i++){
		fin >> points[i][0] >> points[i][1] >> points[i][2];

		if (points[i][0] > max_x )
			max_x = points[i][0];
		if (points[i][1] > max_y )
			max_y = points[i][1];
		if (points[i][2] > max_z )
			max_z = points[i][2];

		if (points[i][0] < min_x )
			min_x = points[i][0];
		if (points[i][1] < min_y )
			min_y = points[i][1];
		if (points[i][2] < min_z )
			min_z = points[i][2];
		
		if (pow(points[i][0], 2) > pow(abs_max_x, 2))
			abs_max_x = sqrt(pow(points[i][0], 2));
		if (pow(points[i][1], 2) > pow(abs_max_y, 2))
			abs_max_y = sqrt(pow(points[i][1], 2));
		if (pow(points[i][2], 2) > pow(abs_max_z, 2))
			abs_max_z = sqrt(pow(points[i][2], 2));
	}

	std::cout << max_x << std::endl;

	for (int i = 0; i < lines_count1 * lines_count2; i++){
		points[i][0] /= abs_max_x;
		points[i][1] /= abs_max_y;
		points[i][2] /= abs_max_z;
		points[i][0] -= 0.5;

	}

	
	 
	fin.close();

	for (int i = 0; i < lines_count2 - 1; i++){
		for (int j = 0; j < lines_count1 - 1; j++){
			Vector3f p1 = {points[i + j * lines_count2]};
			Vector3f p2 = {points[i + 1 + j * lines_count2]};
			Vector3f p3 = {points[i + (j + 1) * lines_count2]};
			Vector3f p4 = {points[i + 1 + (j + 1) * lines_count2]};

			this->polygons[j + i * lines_count1] = {p1, p3, p4};
			this->polygons[j + i * lines_count1 + lines_count1 * (lines_count2 - 1)] = {p1, p4, p2};
		}
	}



	for (int i = 0; i < lines_count1 * lines_count2; i++){
		delete[] points[i];
	}
	delete[] points;
}


DataExtractor::~DataExtractor(){
	delete[] this->polygons;
}


