#ifndef __PRIMITIVES__
#define __PRIMITIVES__

#include <SFML/Graphics.hpp>
#include <cmath>


class Vector3f{
public:
	float x, y, z;

	Vector3f(float *cordinates);
	Vector3f();
	Vector3f(float x, float y, float z);
	inline void norm();

};

/*inline Vector3f v_mult(Vector3f a, Vector3f b);
inline float s_mult(Vector3f a, Vector3f b);
inline Vector3f operator+(Vector3f a, Vector3f b);
inline Vector3f operator-(Vector3f a, Vector3f b);
inline Vector3f operator*(float a, Vector3f b);
inline Vector3f operator*(Vector3f b, float a);
*/
inline float s_mult(Vector3f a, Vector3f b){
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vector3f v_mult(Vector3f a, Vector3f b){
	Vector3f vec;
	vec.x = a.y * b.z - a.z * b.y;
	vec.y = -a.x * b.z + a.z * b.x;
	vec.z = a.x * b.y - a.y * b.x;
	return vec;
}

inline Vector3f operator+(Vector3f a, Vector3f b){
	return Vector3f(a.x + b.x, a.y + b.y, a.z + b.z);
}


inline Vector3f operator-(Vector3f a, Vector3f b){
	return Vector3f(a.x - b.x, a.y - b.y, a.z - b.z);
}


inline Vector3f operator*(float a, Vector3f b){
	return Vector3f(a * b.x, a * b.y, a * b.z);
}


inline Vector3f operator*(Vector3f b, float a){
	return Vector3f(a * b.x, a * b.y, a * b.z);
}


inline void Vector3f::norm(){
	float mod = sqrt(s_mult(*this, *this));
	this->x /= mod;
	this->y /= mod;
	this->z /= mod;
}


class Quaternion{
public:
	float alpha;
	Vector3f vec;
	Quaternion();
	Quaternion(Vector3f vec);
	Quaternion(float alpha, Vector3f vec);
	Quaternion conjugate();
	Quaternion inverse();
       	void norm();	
};


inline Quaternion operator*(Quaternion A, Quaternion B){
	Quaternion q;
	q.alpha = A.alpha * B.alpha - s_mult(A.vec, B.vec);
	q.vec = A.alpha * B.vec + B.alpha * A.vec + v_mult(A.vec, B.vec);
	return q;
}


inline Vector3f operator*(Quaternion A, Vector3f B){
	return {A.alpha * B + v_mult(A.vec, B)};
}


class Triangle{
public:
	Quaternion A, B, C;
	float inten = 0;

	Triangle(Vector3f A, Vector3f B, Vector3f C);
	Triangle();

};


class Mat{
public:
	int col, str;
	float** elems;

	void transpose();
	void inverse();
	float* operator[](int num);
	Mat operator=(Mat source);
	friend std::ostream& operator<<(std::ostream &s, Mat &mat);
	float det();
	float determinantRecursion(Mat minor);
	Mat getMinor(int str, int col);
	Mat();
	Mat(int col, int str);
	Mat(Vector3f A, Vector3f B, Vector3f C);
	Mat(const Mat &source);
	~Mat();
};


Mat operator*(Mat A, Mat B);
bool operator== (Mat A, Mat B);
inline Vector3f operator*(Mat A, Vector3f vec);

inline Vector3f operator*(Mat A, Vector3f vec){
	Vector3f result;
		result.x = vec.x * A[0][0] + vec.y * A[1][0] + vec.z * A[2][0];
		result.y = vec.x * A[0][1] + vec.y * A[1][1] + vec.z * A[2][1];
		result.z = vec.x * A[0][2] + vec.y * A[1][2] + vec.z * A[2][2];
		
	return result;
}








#endif //__PRIMITIVES__

