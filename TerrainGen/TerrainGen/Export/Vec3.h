#pragma once

/*
* class utililitaire representant les vecteurs, points dans un espace 3D.
*/
template<typename T>
class Vec3
{
public:
	T x, y, z;
	
	Vec3(){};

	Vec3(T n) :x(n), y(n), z(n){};
	Vec3(T x_, T y_, T z_) :x(x_), y(y_), z(z_){};
	Vec3(const Vec3<T> & p) :x(p.x), y(p.y), z(p.z){};

	/*
	* Surcharge des operateurs arithmetiques
	*/

	inline const Vec3 & operator=(const Vec3<T> & p)
	{
		x = p.x;
		y = p.y;
		z = p.z;
		return *this;
	}

	inline const Vec3 & operator+=(const Vec3<T> & p)
	{
		x += p.x;
		y += p.y;
		z += p.z;
		return *this;
	}

	inline const Vec3 & operator*=(const Vec3<T> & p)
	{
		x *= p.x;
		y *= p.y;
		z *= p.z;
		return *this;
	}

	inline const Vec3 & operator*=(T k)
	{
		x *= k;
		y *= k;
		z *= k;
		return *this;
	}

	inline bool operator==(Vec3<T> v)
	{
		return v.x == x && v.y == y && v.z == z;
	}

	inline bool operator!=(Vec3<T> v)
	{
		return v.x != x && v.y = y && v.z != z;
	}

	inline Vec3<T> normalized ( ) { return *this * ( 1 / sqrt ( x * x + y * y + z * z ) ); }

	inline Vec3 operator-()
	{
		return Vec3<T>(-x,-y,-z);
	}

	inline Vec3<T> minv(Vec3<T> v)
	{
		return Vec3<T>(std::min(x, v.x), std::min(y, v.y), std::min(z, v.z));
	}

	inline Vec3<T> maxv(Vec3<T> v)
	{
		return Vec3<T>(std::max(x, v.x), std::max(y, v.y), std::max(z, v.z));
	}
	// Produit Vectoriel
	static inline Vec3<T> crossProduct(const Vec3<T> & v1, const Vec3<T> & v2) { return Vec3<T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x *v2.y - v1.y * v2.x); }

	// Produit Scalaire
	static inline float dotProduct(const Vec3<T> & v1, const Vec3<T> & v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
};

template <typename T>
inline const Vec3<T> operator * (const Vec3<T> & v, const T & k) { return Vec3<T>(v.x * k, v.y * k, v.z * k); }

template <typename T>
inline const Vec3<T> operator * (const T & k, const Vec3<T> & v) { return Vec3<T>(v.x * k, v.y * k, v.z * k); }

template <typename T>
inline const Vec3<T> operator * (const Vec3<T> & v1, const Vec3<T> & v2) { return Vec3<T>(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z); }

template <typename T>
inline const Vec3<T> operator / (const Vec3<T> & v, const T & k) { return Vec3<T>(v.x / k, v.y / k, v.z / k); }

template <typename T>
inline const Vec3<T> operator + (const Vec3<T> & v1, const Vec3<T> & v2) { return Vec3<T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }

template <typename T>
inline const Vec3<T> operator - (const Vec3<T> & v1, const Vec3<T> & v2) { return Vec3<T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); }

template <typename T>
inline float distanceSquared(const Vec3<T> & v1, const Vec3<T> & v2) { return (v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y) + (v2.z - v1.z) * (v2.z - v1.z); }

template <typename T>
inline float distance(const Vec3<T> & v1, const Vec3<T> & v2) { return sqrt(distanceSquared(v1, v2)); }