#pragma once
#include "..\Utils\Vec3.h"
#include <array>
/*
Classe de matrice 4x4, represente les rotation.
*/
class Mat4x4 {
public:
	float m[4][4];
	Mat4x4();
	Mat4x4(float mat[4][4]);
	Mat4x4(float t00_, float t01_, float t02_, float t03_,
		float t10_, float t11_, float t12_, float t13_,
		float t20_, float t21_, float t22_, float t23_,
		float t30_, float t31_, float t32_, float t33_);
		
	/* Transposee
	* m : matrice
	* return : m transposee
	*/
	inline static Mat4x4 transpose(const Mat4x4 &m)
	{
		return Mat4x4(m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0],
					  m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1],
					  m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2],
					  m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]);
	} 

	/* Multiplication matrice, matrice
	* m1 : matrice
	* return : m * m1
	*/
	inline Mat4x4 operator*(const Mat4x4 &m1)
	{
		Mat4x4 res;
		for (unsigned int i = 0; i < 4; ++i)
		{
			for (unsigned int j = 0; j < 4; ++j)
			{
				res.m[i][j] = m[i][0] * m1.m[0][j] +
								m[i][1] * m1.m[1][j] +
								m[i][2] * m1.m[2][j] +
							m[i][3] * m1.m[3][j];
			}
		}
		return res;
	}

	/* Multiplication matrice, scalaire
	* f : scalaire
	* return : m * f
	*/
	inline Mat4x4 operator*(float f)
	{
		Mat4x4 res;
		for (unsigned int i = 0; i < 4; ++i)
		{
			for (unsigned int j = 0; j < 4; ++j)
			{
				res.m[i][j] = m[i][j] * f;
			}
		}
		return res;
	}

	/* Multiplication matrice, vec3
	* p : vector3
	* return : m * p avec le retour deja divise par w et suppose le w de p = 1. 
	*/
	inline Vec3<float> operator*(const Vec3<float> &p)
	{
		float xp = m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z + m[0][3];
		float yp = m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z + m[1][3];
		float zp = m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z + m[2][3];
		float w = m[3][0] * p.x + m[3][2] * p.y + m[3][2] * p.z + m[3][3];
		float invw = 1.f / w;
		return Vec3<float>(xp * invw, yp * invw, zp * invw);
	}
	~Mat4x4();
	
};