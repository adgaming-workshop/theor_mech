#include "primitives.h"
#include <cmath>
#include <iostream>

Vector3f::Vector3f(float *cordinates){
	this->x = cordinates[0];
	this->y = cordinates[1];
	this->z = cordinates[2];
}


Vector3f::Vector3f(){};


Vector3f::Vector3f(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
}






Triangle::Triangle(Vector3f A, Vector3f B, Vector3f C){
	this->A.vec = A;
	this->B.vec = B;
	this->C.vec = C;

	this->A.alpha = 0;
	this->B.alpha = 0;
	this->C.alpha = 0;
}


Triangle::Triangle(){};


Mat::Mat(){};


Mat::Mat(int str, int col){
	this->str = str;
	this->col = col;
	this->elems = new float*[str];
	for (int i = 0; i < str; i++){
		this->elems[i] = new float[col];
	}
	for (int i = 0; i < this->str; i++){
		for (int j = 0; j < this->col; j++){
			this->elems[i][j] = 0;
		}
	}
}


Mat::Mat(const Mat &source){
	this->str = source.str;
	this->col = source.col;
	this->elems = new float*[source.str];
	for (int i = 0; i < source.str; i++){
		this->elems[i] = new float[source.col];
	}	
	
	for (int i = 0; i < source.str; i++){
		for (int j = 0; j < source.col; j++){
			this->elems[i][j] = source.elems[i][j];
		}
	}
}


Mat Mat::operator=(Mat source){
	for(int i = 0; i < this->str; i++){ //Тут может быть сегфолт
		delete[] this->elems[i];
	}
	this->str = source.str;
	this->col = source.col;
	this->elems = new float*[source.str];
	for (int i = 0; i < source.str; i++){
		this->elems[i] = new float[source.col];
	}
	
	for (int i = 0; i < source.str; i++){
		for (int j = 0; j < source.col; j++){
			this->elems[i][j] = source.elems[i][j];
		}
	}

	return *this;
}


Mat::~Mat(){
	for(int i = 0; i < this->str; i++){
		delete[] this->elems[i];
	}
	delete[] this->elems;
}


float* Mat::operator[](int num){
	return this->elems[num];
}


void Mat::transpose(){
	float** tr_elems = new float*[this->col];
	for (int i = 0; i < this->col; i++){
		tr_elems[i] = new float[this->str];
	}
	
	for (int i = 0; i < this->str; i++){
		for (int j = 0; j < this->col; j++){
			tr_elems[j][i] = this->elems[i][j];
		}
	}

	for (int i = 0; i < this->str; i++){
		delete[] this->elems[i];
	}

	this->elems = tr_elems;
}


Mat Mat::getMinor(int str, int col){
	Mat minor = Mat(this->str - 1, this->col - 1);

	int i_minor = 0;
	int j_minor = 0;

	for (int i = 0; i < this->str; i++){
		if (i != str){
			for (int j = 0; j < this->col; j++){
				if (j != col){
					minor[i_minor][j_minor] = this->elems[i][j];
					j_minor++;
				}
			}
			i_minor++;
			j_minor = 0;
		}
	}

	return minor;
}


std::ostream& operator<<(std::ostream &s, Mat &mat){
	for (int i = 0; i < mat.str; i++){
		for (int j = 0; j < mat.col; j++){
			s << mat.elems[i][j] << " ";
		}
		s << std::endl;
	}
	return s;
}


float Mat::determinantRecursion(Mat minor){
	if (minor.str == 1){
		return minor[0][0];
	}

	float result = 0;

	for (int i = 0; i < minor.str; i++){
	       result += pow(-1, i) * minor[i][0] * determinantRecursion(minor.getMinor(i, 0));
	}
	
	return result;
}


float Mat::det(){
	return this->determinantRecursion(*this);
}


void Mat::inverse(){
	float d = this->det();

	Mat m = Mat(this->str, this->col);

	for (int i = 0; i < this->str; i++){
		for (int j = 0; j < this->col; j++){
			m[i][j] = pow(-1, i + j) * this->getMinor(i, j).det() / d;
		}
	}
	m.transpose();
	*this = m;
}


Mat operator*(Mat A, Mat B){
	Mat result = Mat(A.str, B.col);

	for (int ii = 0; ii < A.str; ii++){
		for (int jj = 0; jj < B.col; jj++){
			for (int j = 0; j < A.col; j++){
				result[ii][jj] += A[ii][j] * B[j][jj];
			}
		}
	}

	return result;
}
	      		       



Mat::Mat(Vector3f A, Vector3f B, Vector3f C){
	this->str = 3;
	this->col = 3;

	this->elems = new float*[3];
	for (int i = 0; i < this->str; i++){
		this->elems[i] = new float[this->col];
	}

	this->elems[0][0] = A.x;
	this->elems[1][0] = A.y;
	this->elems[2][0] = A.z;

	this->elems[0][1] = B.x;
	this->elems[1][1] = B.y;
	this->elems[2][1] = B.z;

			
	this->elems[0][2] = C.x;
	this->elems[1][2] = C.y;
	this->elems[2][2] = C.z;
}


bool operator==(Mat A, Mat B){
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (A[i][j] != B[i][j])
				return false;
	return true;
}


Quaternion::Quaternion(){};


Quaternion::Quaternion(float alpha, Vector3f vec){
	this->alpha = alpha;
	this->vec =vec;
}


Quaternion::Quaternion(Vector3f vec){
	this->vec = vec;
}


Quaternion Quaternion::conjugate(){
	return {this->alpha, (-1) * this->vec};
}


Quaternion Quaternion::inverse(){
	Quaternion q;
	q = this->conjugate();
	float mod = ((*this) * q).alpha;
	q.alpha /= mod;
	q.vec = q.vec * (1 / mod);
	return q;
}


void Quaternion::norm(){
	Quaternion q;
	q = this->conjugate();
	float mod = ((*this) * q).alpha;
	this->alpha /= mod;
	this->vec = this->vec * (1 / mod);
}

