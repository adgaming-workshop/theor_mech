#ifndef __DATA_PARSER__
#define __DATA_PARSER__

#include "primitives.h"
#include <string>


class DataExtractor{
public:
	Triangle *polygons;
	float triangles_count;

	DataExtractor(std::string file_name);
	~DataExtractor();
};


#endif
