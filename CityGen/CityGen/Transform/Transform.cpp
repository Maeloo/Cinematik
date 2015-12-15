#include "Transform.h"


Transform::Transform()
{
}

Transform::Transform(Mat4x4 m_) : m(m_)
{
}

/* Composition de Transformation
* t : transformation
* return : compose de la transformation courant et de t
*/
Transform Transform::operator*(const Transform &t)
{
	return Transform(m * t.m);
}

/* Translation
* delta : vecteur
* return : transformation de translation du vecteur delta
*/
Transform Transform::translate(const Vec3<float> &delta)
{
	return Transform(Mat4x4(1.f, 0.f, 0.f, delta.x,
		0.f, 1.f, 0.f, delta.y,
		0.f, 0.f, 1.f, delta.z,
		0.f, 0.f, 0.f, 1.f));
}

/* Mise a l'echelle
* x, y, z : scalaires
* return : Transformation de mise a l'echelle des scalaires
*/
Transform Transform::scale(float x, float y, float z)
{
	return Transform(Mat4x4(x, 0.f, 0.f, 0.f,
		0.f, y, 0.f, 0.f,
		0.f, 0.f, z, 0.f,
		0.f, 0.f, 0.f, 1.f));
}

/* Rotation autour de l'axe x
* angle : angle de la rotation (radian)
* return : Transformation de rotation d'angle radian autour de l'axe x
*/
Transform Transform::rotateX(float angle)
{
	float sin_a = sinf(angle);
	float cos_a = cosf(angle);
	return Transform(Mat4x4(1.f, 0.f, 0.f, 0.f,
		0.f, cos_a, -sin_a, 0.f,
		0.f, sin_a, -cos_a, 0.f,
		0.f, 0.f, 0.f, 1.f));
}

/* Rotation autour de l'axe y
* angle : angle de la rotation (radian)
* return : Transformation de rotation d'angle radian autour de l'axe y
*/
Transform Transform::rotateY(float angle)
{
	float sin_a = sinf(angle);
	float cos_a = cosf(angle);
	return Transform(Mat4x4(cos_a, 0.f, sin_a, 0.f,
		0.f, 1.f, 0.f, 0.f,
		-sin_a, 0.f, cos_a, 0.f,
		0.f, 0.f, 0.f, 1.f));
}

/* Rotation autour de l'axe z
* angle : angle de la rotation (radian)
* return : Transformation de rotation d'angle radian autour de l'axe z
*/
Transform Transform::rotateZ(float angle)
{
	float sin_a = sinf(angle);
	float cos_a = cosf(angle);
	return Transform(Mat4x4(cos_a, -sin_a, 0.f, 0.f,
		sin_a, cos_a, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f));
}

Transform Transform::Shrink(float factor, Vec3<float> pivot){
	
	return translate(pivot)*scale(factor,factor,factor)*translate(-pivot);
}


/* Rotation axe X par rapport a un point Pivot
* factor : angle de rotation
* pivot : point pivot
*/
Transform Transform::RotatelocalX(float angle, Vec3<float> pivot){

	return translate(pivot)*rotateX(angle)*translate(-pivot);
}

/* Rotation axe Y par rapport a un point Pivot
* factor : angle de rotation
* pivot : point pivot
*/
Transform Transform::RotatelocalY(float angle, Vec3<float> pivot){

	return translate(pivot)*rotateY(angle)*translate(-pivot);
}

/* Rotation axe Z par rapport a un point Pivot
* factor : angle de rotation
* pivot : point pivot
*/
Transform Transform::RotatelocalZ(float angle, Vec3<float> pivot){

	return translate(pivot)*rotateZ(angle)*translate(-pivot);
}

Transform::~Transform()
{
}
