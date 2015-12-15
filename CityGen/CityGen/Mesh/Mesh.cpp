#include "Mesh.h"


Mesh::Mesh ( ) : points ( std::vector<Vec3<float>> ( ) ), faces ( std::vector<Vec3<unsigned int>> ( ) ) { };

Mesh::Mesh ( std::vector<Vec3<float>> points_, std::vector<Vec3<unsigned int>> faces_, std::vector<Vec3<unsigned int>> facesTextures_, std::vector<Vec3<unsigned int>> facesNormales_, std::vector<Vec3<float>> textures_, std::vector<Vec3<float>> normales_ )
	: points ( points_ ), faces ( faces_ ), textures ( textures_ ), facesTextures ( facesTextures_ ), facesNormalesIndex ( facesNormales_ ), facesNormales ( normales_ ) {
	pivot = Vec3<float> ( 0.f );
	for ( unsigned int i = 0; i < points_.size ( ); ++i ) {
		pivot += points[i];
	}
	pivot = pivot / ( float ) points_.size ( );
}

Mesh::Mesh ( std::vector<Vec3<float>> points_, std::vector<Vec3<unsigned int>> faces_, std::vector<Vec3<unsigned int>> facesTextures_, std::vector<Vec3<float>> textures_ )
	: points ( points_ ), faces ( faces_ ), textures ( textures_ ), facesTextures ( facesTextures_ ) {
	pivot = Vec3<float> ( 0.f );
	for ( unsigned int i = 0; i < points_.size ( ); ++i ) {
		pivot += points[i];
	}
	pivot = pivot / ( float ) points_.size ( );

	calculateNormals ( );
}

Vec3<float> Mesh::getPivot ( ) {
	pivot = Vec3<float> ( 0.f );
	for ( unsigned int i = 0; i < points.size ( ); ++i ) {
		pivot += points[i];
	}
	pivot = pivot / ( float ) points.size ( );

	return pivot;
}

void Mesh::calculateNormals ( ) {
	// Calcule des normales par face
	std::cout << "Calculate face normals...\n";
	unsigned int facesCount = faces.capacity ( );
	facesNormalesIndex = std::vector<Vec3<unsigned int>> ( facesCount );
	facesNormales = std::vector<Vec3<float>> ( facesCount );

	for ( unsigned int i = 0; i < facesCount; ++i ) {
		Vec3<unsigned int> face = faces[i];
		Vec3<float> normal = Vec3<float>::crossProduct ( points[face.y] - points[face.x], points[face.z] - points[face.x] ).normalized ( );
		facesNormalesIndex[i] = face;
		facesNormales[i] = normal;
	}

	// Calcule des normales par vertex
	std::cout << "Calculate vertex normals...\n";
	unsigned int verticesCount = points.capacity ( );
	verticesNormales = std::vector<Vec3<float>> ( verticesCount );

	for ( unsigned int i = 0; i < verticesCount; ++i ) {
		Vec3<float> normal = Vec3<float>( .0f, .0f, .0f );

		for ( unsigned int j = 0; j < facesCount; ++j ) {
			Vec3<unsigned int> face = faces[j];

			for ( unsigned int k = 0; k < verticesCount; ++k ) { // Obligé d'avoir des faces triangulaires ...
				if ( ( k == 0 && face.x == i ) ||
					 ( k == 1 && face.y == i ) ||
					 ( k == 2 && face.z == i ) ) {
					normal += facesNormales[j];
				}
			}

			verticesNormales[i] = normal.normalized ( );
		}
	}
}


Mesh::Mesh(const Mesh & mesh) 
	: points(mesh.points), faces(mesh.faces), textures(mesh.textures), facesTextures(mesh.facesNormalesIndex), facesNormalesIndex(mesh.facesNormalesIndex), facesNormales(mesh.facesNormales), pivot(mesh.pivot){};

std::vector<Vec3<float>> Mesh::getPoints() const
{
	return points;
}

std::vector<Vec3<unsigned int>> Mesh::getFaces() const
{
	return faces;
}

std::vector<Vec3<float>> Mesh::getTextures() const
{
	return textures;
}

std::vector<Vec3<float>> Mesh::getFacesNormales() const
{
	return facesNormales;
}

std::vector<Vec3<unsigned int>> Mesh::getFacesTextures() const
{
	return facesTextures;
}

std::vector<Vec3<unsigned int>> Mesh::getFacesNormalesIndex() const
{
	return facesNormalesIndex;
}

std::vector<Vec3<float>> Mesh::getVerticesNormales ( ) const {
	return verticesNormales;
}

/* Application d'une transformation au mesh
* t : transformation
*/
void Mesh::transform(Transform t)
{
	for (unsigned int i = 0; i < points.size(); ++i) {
				points[i] = t.m * points[i];
			}
}

/* Merge du mesh courant avec un autre
* m : mesh a fusionner
*/
void Mesh::merge(const Mesh & m)
{
	std::vector<Vec3<unsigned int>> newFaces = m.getFaces();
	std::vector<Vec3<unsigned int>> newFacesTextures = m.getFacesTextures();
	std::vector<Vec3<unsigned int>> newFacesNormales = m.getFacesNormalesIndex();

	std::vector<Vec3<float>> newPoints = m.getPoints();
	std::vector<Vec3<float>> newTextures = m.getTextures();
	std::vector<Vec3<float>> newNormales = m.getFacesNormales();

	Vec3<unsigned int> offset(points.size());

	for (unsigned int i = 0; i < newFaces.size(); ++i)
		newFaces[i] += offset;
	for (unsigned int i = 0; i < newFacesTextures.size(); ++i)
		newFacesTextures[i] += offset;
	for (unsigned int i = 0; i < newFacesNormales.size(); ++i)
		newFacesNormales[i] += offset;

	faces.insert(faces.end(), newFaces.begin(), newFaces.end());
	facesTextures.insert(facesTextures.end(), newFacesTextures.begin(), newFacesTextures.end());
	facesNormalesIndex.insert(facesNormalesIndex.end(), newFacesNormales.begin(), newFacesNormales.end());
	
	points.insert(points.end(), newPoints.begin(), newPoints.end());
	textures.insert(textures.end(), newTextures.begin(), newTextures.end());
	facesNormales.insert(facesNormales.end(), newNormales.begin(), newNormales.end());
}

/*
* return : Mesh triangle
* p0, p1, p2 : sommet du triangle
*/
Mesh Mesh::Triangle(const Vec3<float>& p0, const Vec3<float>& p1, const Vec3<float>& p2)
{
	std::vector<Vec3<float>> points(3);
	points[0] = p0;
	points[1] = p1;
	points[2] = p2;

	 std::vector<Vec3<unsigned int>> faces(1);
	faces[0] = Vec3<unsigned int>(2, 1, 3);

	return Mesh(points, faces, std::vector<Vec3<unsigned int>>(), std::vector<Vec3<unsigned int>>(), std::vector<Vec3<float>>(), std::vector<Vec3<float>>());
}

/*
* return : Mesh quadrangle
* p0, p1, p2, p3 : sommet du quadrangle
*/
Mesh Mesh::Quadrangle(const Vec3<float>& p0, const Vec3<float>& p1, const Vec3<float>& p2, const Vec3<float>& p3)
{
	std::vector<Vec3<float>> points(4);
	points[0] = p0;
	points[1] = p1;
	points[2] = p2;
	points[3] = p3;

	std::vector<Vec3<unsigned int>> faces(2);
	faces[0] = Vec3<unsigned int>(2, 1, 3);
	faces[1] = Vec3<unsigned int>(1, 4, 3);

	return Mesh(points, faces, std::vector<Vec3<unsigned int>>(), std::vector<Vec3<unsigned int>>(), std::vector<Vec3<float>>(), std::vector<Vec3<float>>());
}

/*
* return : Mesh pentagone
* p0, p1, p2, p3, p4 : sommet du pentagone
*/
Mesh Mesh::Pentagone(const Vec3<float>& p0, const Vec3<float>& p1, const Vec3<float>& p2, const Vec3<float>& p3, const Vec3<float>& p4)
{
	std::vector<Vec3<float>> points = std::vector<Vec3<float>> ( 5 );
	points[0] = p0;
	points[1] = p1;
	points[2] = p2;
	points[3] = p3;
	points[4] = p4;

	std::vector<Vec3<unsigned int>> faces = std::vector<Vec3<unsigned int>> ( 3 );
	faces[0] = Vec3<unsigned int>(2, 1, 3);
	faces[1] = Vec3<unsigned int>(1, 4, 3);
	faces[2] = Vec3<unsigned int>(1, 5, 4);

	return Mesh(points, faces, std::vector<Vec3<unsigned int>>(), std::vector<Vec3<unsigned int>>(), std::vector<Vec3<float>>(), std::vector<Vec3<float>>());
}

/*
* return : Mesh hexagone
* p0, p1, p2, p3, p4, p5 : sommet du hexagone
*/
Mesh Mesh::Hexagone(const Vec3<float>& p0, const Vec3<float>& p1, const Vec3<float>& p2, const Vec3<float>& p3, const Vec3<float>& p4, const Vec3<float>& p5)
{
	std::vector<Vec3<float>> points(6);
	points[0] = p0;
	points[1] = p1;
	points[2] = p2;
	points[3] = p3;
	points[4] = p4;
	points[5] = p5;


	std::vector<Vec3<unsigned int>>faces = std::vector<Vec3<unsigned int>>(4);
	faces[0] = Vec3<unsigned int>(2, 1, 4);
	faces[1] = Vec3<unsigned int>(2, 4, 3);
	faces[2] = Vec3<unsigned int>(1, 5, 4);
	faces[3] = Vec3<unsigned int>(1, 6, 5);


	return Mesh(points, faces, std::vector<Vec3<unsigned int>>(), std::vector<Vec3<unsigned int>>(), std::vector<Vec3<float>>(), std::vector<Vec3<float>>());
}

/* Creation d'un mesh de prisme (Box)
* return : Mesh box
* p0, p1, p2, p3 : base de la box
* h : hauteur de la box
*/
Mesh Mesh::Box(const Vec3<float>& p1, const Vec3<float>& p2, const Vec3<float>& p3, const Vec3<float>& p4, const float& h){

	const unsigned int sizePoints = 8;
	const unsigned int sizeFaces = 12; //12 Triangles

	Vec3<float> Points[sizePoints] = { p1 + Vec3<float>(0., 0., h),
										p2 + Vec3<float>(0., 0., h),
										p3 + Vec3<float>(0., 0., h),
										p4 + Vec3<float>(0., 0., h),
										p1,
										p2,
										p3,
										p4
									
	};

	std::vector<Vec3<float>> points(Points, Points + sizePoints);


	Vec3<unsigned int> Faces[12] = { Vec3<unsigned int>(1, 5, 4),
									Vec3<unsigned int>(5, 8, 4),

									Vec3<unsigned int>(2, 1, 3),
									Vec3<unsigned int>(1, 4, 3),

									Vec3<unsigned int>(2, 6, 1),
									Vec3<unsigned int>(6, 5, 1),

									Vec3<unsigned int>(4, 8, 3),
									Vec3<unsigned int>(8, 7, 3),

									Vec3<unsigned int>(3, 7, 2),
									Vec3<unsigned int>(7, 6, 2),

									Vec3<unsigned int>(5, 6, 8),
									Vec3<unsigned int>(6, 7, 8)
	};

	std::vector<Vec3<unsigned int>> faces(Faces, Faces + sizeFaces);

	return  Mesh(points, faces, std::vector<Vec3<unsigned int>>(), std::vector<Vec3<unsigned int>>(), std::vector<Vec3<float>>(), std::vector<Vec3<float>>());

}

/*
* return : Mesh cercle
* o : origine du cercle
* r : rayon du cercle
* side : nombre de cote discretises du cercle
* direction : pour connaitre le sens du cercle
*/

Mesh Mesh::Circle(const Vec3<float>& o, const float & r, const unsigned int & side, const bool& direction)
{
	if (direction == true){
		const float dsize = 2.f * Constantes::PI / (float)side;
		unsigned int facesSize = side;
		std::vector<Vec3<float>> points;
		std::vector<Vec3<unsigned int>> faces;
		points.resize(side + 1);
		faces.resize(facesSize);
		points[0] = o;

		for (unsigned int i = 1; i <= side; ++i)
		{
			const float angle = (float)i * dsize;
			points[i] = Vec3<float>(o.x + r * cos(angle), o.y + r * sin(angle), o.z);

		}
		for (unsigned int i = 1; i < facesSize; ++i)
		{
			const unsigned int tmpi = i;// << 1;
			faces[i - 1] = Vec3<unsigned int>(1, tmpi + 1, tmpi + 2);
		}
		faces[facesSize - 1] = Vec3<unsigned int>(1, facesSize + 1, 2);
		return Mesh(points, faces, std::vector<Vec3<unsigned int>>(), std::vector<Vec3<unsigned int>>(), std::vector<Vec3<float>>(), std::vector<Vec3<float>>());
	}
	else{
		const float dsize = 2.f * Constantes::PI / (float)side;
		unsigned int facesSize = side;
		std::vector<Vec3<float>> points;
		std::vector<Vec3<unsigned int>> faces;
		points.resize(side + 1);
		faces.resize(facesSize);
		points[0] = o;

		for (unsigned int i = 1; i <= side; ++i)
		{
			const float angle = (float)i * dsize;
			points[i] = Vec3<float>(o.x + r * cos(angle), o.y + r * sin(angle), o.z);

		}
		for (unsigned int i = 1; i < facesSize; ++i)
		{
			const unsigned int tmpi = i;// << 1;
			faces[i - 1] = Vec3<unsigned int>(1, tmpi + 2,tmpi + 1);
		}
		faces[facesSize - 1] = Vec3<unsigned int>(1, 2, facesSize + 1);
		return Mesh(points, faces, std::vector<Vec3<unsigned int>>(), std::vector<Vec3<unsigned int>>(), std::vector<Vec3<float>>(), std::vector<Vec3<float>>());
	}
}

/*
* return : Mesh Cylinder
* o : origine du cylindre
* r : rayon du de la base du cylindre
* h : hauteur du cylindre
* side : nombre de cote discretises du cercle
*/
Mesh Mesh::Cylinder(const Vec3<float>& o, const double& r, const double& h,const unsigned int& side){
	
	Mesh CircleBase = Circle(o,r,side,false);
	Mesh CircleTop = Circle(o+Vec3<float>(0,0,h),r,side,true);
	
	std::vector<Vec3<float>> newPointsBase = CircleBase.getPoints();
	std::vector<Vec3<float>> newPointsTop = CircleTop.getPoints();

	for (unsigned int i = 1; i < side; ++i){
		Mesh FaceQuad = Quadrangle(newPointsTop[i], newPointsTop[i + 1], newPointsBase[i + 1], newPointsBase[i]);
		CircleBase.merge(FaceQuad);
	}

	Mesh FaceQuad = Quadrangle(newPointsBase[1], newPointsBase[side], newPointsTop[side], newPointsTop[1]);
	CircleBase.merge(FaceQuad);

	CircleBase.merge(CircleTop);

	return CircleBase;
}

/*
* return : Mesh cone
* o : origine du cone
* r : rayon de la base du cone
* h : hauteur du cone
* side : nombre de cote discretises du cercle
*/
Mesh Mesh::Cone(const Vec3<float>& o, const double& r, const double& h, const unsigned int& side){

	Mesh CircleBase = Circle(o, r, side,false);
	Vec3<float> pic = o + Vec3<float>(0, 0, h);

	std::vector<Vec3<float>> newPointsBase = CircleBase.getPoints();

	for (unsigned int i = 1; i < side; ++i){
		Mesh FaceTri = Triangle(newPointsBase[i + 1], newPointsBase[i], pic);
		CircleBase.merge(FaceTri);
	}

	Mesh FaceTri = Triangle(newPointsBase[1], newPointsBase[side - 1], pic);
	CircleBase.merge(FaceTri);


	return CircleBase;
}


/*
* return : Mesh Sphere
* o : origine de la sphere
* r : rayon de la sphere
*/
Mesh Mesh::Sphere(const Vec3<float>& o, const double& r , const int& nbmeridiens , const int& nbparalleles){

	Mesh sphere;

	float anglemeridiens = (2 * Constantes::PI) / nbmeridiens;
	float angleparalleles = Constantes::PI / nbparalleles;

	for (float i = 0; i<nbmeridiens; i++){
		for (float j = 0; j<nbparalleles; j++){

			Vec3<float> p0 = Vec3<float>(r*sin(angleparalleles*j)*cos(anglemeridiens*i), r*sin(angleparalleles*j)*sin(anglemeridiens*i), r*cos(angleparalleles*j));
			Vec3<float> p1 = Vec3<float>(r*sin(angleparalleles*j)*cos(anglemeridiens*(i + 1)), r*sin(angleparalleles*j)*sin(anglemeridiens*(i + 1)), r*cos(angleparalleles*j));
			Vec3<float> p2 = Vec3<float>(r*sin(angleparalleles*(j + 1))*cos(anglemeridiens*i), r*sin(angleparalleles*(j + 1))*sin(anglemeridiens*i), r*cos(angleparalleles*(j + 1)));

			Mesh FaceTriangle = Triangle(p0, p1,p2);
			sphere.merge(FaceTriangle);

			p0 = Vec3<float>(r*sin(angleparalleles*(j + 1))*cos(anglemeridiens*(i + 1)), r*sin(angleparalleles*(j + 1))*sin(anglemeridiens*(i + 1)), r*cos(angleparalleles*(j + 1)));

			FaceTriangle = Triangle(p2, p1, p0);
			sphere.merge(FaceTriangle);

		}
	}
	return sphere;
	
}


/*
* return : Mesh route
* p0, p1, p2, p3 : sommet du quadrangle
* sizePavement : taille d'un trottoir
* hpavement : hauteur trottoir
*/
Mesh Mesh::Route(const Vec3<float>& p0, const Vec3<float>& p1, const Vec3<float>& p2, const Vec3<float>& p3, const float& sizePavement, const float& hPavement)
{

	Vec3<float> shiftPavementZ(0.f, 0.f, hPavement);
	Vec3<float> shiftPavementY(sizePavement, 0.f, 0.f);
	Mesh res;
	res.merge(Mesh::Box(p0 - shiftPavementZ, p1 - shiftPavementZ, p1 - shiftPavementZ + (Vec3<float>(p2 - p1).normalized() * sizePavement), p0 - shiftPavementZ + (Vec3<float>(p3 - p0).normalized() * sizePavement), hPavement));
	res.merge(Mesh::Quadrangle(p0 - shiftPavementZ, p1 - shiftPavementZ, p2 - shiftPavementZ, p3 - shiftPavementZ));
	res.merge(Mesh::Box(p2 - shiftPavementZ, p3 - shiftPavementZ, p3 - shiftPavementZ + (Vec3<float>(p0 - p3).normalized() * sizePavement), p2 - shiftPavementZ + (Vec3<float>(p1 - p2).normalized() * sizePavement), hPavement));
	return res;
}

/*return : Mesh route
* p0, p1, p2, p3 : sommet du quadrangle
* sizePavement : taille d'un trottoir
* hpavement : hauteur trottoir
*/
Mesh Mesh::RouteL(const Vec3<float>& p0, const Vec3<float>& p1, const Vec3<float>& p2, const Vec3<float>& p3, const float& sizePavement, const float& hPavement)
{
	//	Vec3<float> shiftPavementZ(0.f, 0.f, hPavement);

	Vec3<float> shiftPavementZ(0.f, 0.f, hPavement);
	Vec3<float> shiftPavementY(sizePavement, 0.f, 0.f);
	Mesh res;
	//res.merge(Mesh::Box(p0 - shiftPavementZ, p1 - shiftPavementZ, p1 - shiftPavementZ + (Vec3<float>(p2 - p1).normalized() * sizePavement), p0 - shiftPavementZ + (Vec3<float>(p3 - p0).normalized() * sizePavement), hPavement));
	res.merge(Mesh::Quadrangle(p0 - shiftPavementZ, p1 - shiftPavementZ, p2 - shiftPavementZ, p3 - shiftPavementZ));
	res.merge(Mesh::Box(p2 - shiftPavementZ, p3 - shiftPavementZ, p3 - shiftPavementZ + (Vec3<float>(p0 - p3).normalized() * sizePavement), p2 - shiftPavementZ + (Vec3<float>(p1 - p2).normalized() * sizePavement), hPavement));
	return res;
}


/*return : Mesh route
* p0, p1, p2, p3 : sommet du quadrangle
* sizePavement : taille d'un trottoir
* hpavement : hauteur trottoir
*/
Mesh Mesh::RouteR(const Vec3<float>& p0, const Vec3<float>& p1, const Vec3<float>& p2, const Vec3<float>& p3, const float& sizePavement, const float& hPavement)
{

	Vec3<float> shiftPavementZ(0.f, 0.f, hPavement);
	Vec3<float> shiftPavementY(sizePavement, 0.f, 0.f);
	Mesh res;
	res.merge(Mesh::Box(p0 - shiftPavementZ, p1 - shiftPavementZ, p1 - shiftPavementZ + (Vec3<float>(p2 - p1).normalized() * sizePavement), p0 - shiftPavementZ + (Vec3<float>(p3 - p0).normalized() * sizePavement), hPavement));
	res.merge(Mesh::Quadrangle(p0 - shiftPavementZ, p1 - shiftPavementZ, p2 - shiftPavementZ, p3 - shiftPavementZ));
	//res.merge(Mesh::Box(p2 - shiftPavementZ, p3 - shiftPavementZ, p3 - shiftPavementZ + (Vec3<float>(p0 - p3).normalized() * sizePavement), p2 - shiftPavementZ + (Vec3<float>(p1 - p2).normalized() * sizePavement), hPavement));
	return res;
}

/*return : Mesh toit triangle
* h : hauteur du toit
*/
Mesh Mesh::Toit(const Vec3<float>& p1, const Vec3<float>& p2, const Vec3<float>& p3, const Vec3<float>& p4, float h)
{
	Mesh m;
	m = Mesh::Quadrangle(p1, p2, p3, p4);
	Vec3<float> p5((p1 + p2 + p3 + p4) / 4.f);
	float dist = distance(p1, p2);
	float dist1 = distance(p2, p3);
	if (dist < dist1)
	{
		Vec3<float> p6((p1 + p2) / 2.f);
		Vec3<float> p7((p3 + p4) / 2.f);
		Vec3<float> p8(((p5 + p6) / 2.f) + Vec3<float>(0, 0, h));
		Vec3<float> p9(((p5 + p7) / 2.f) + Vec3<float>(0, 0, h));
		Mesh m1;
		m1 = Mesh::Triangle(p1, p2, p8);
		Mesh m2;
		m2 = Mesh::Triangle(p4, p9, p3);
		m.merge(m1);
		m.merge(m2);
		Mesh m3;
		m3 = Mesh::Quadrangle(p3, p9, p8, p2);
		m.merge(m3);
		Mesh m4;
		m4 = Mesh::Quadrangle(p1, p8, p9, p4);
		m.merge(m4);
		return m;
	}
	else
	{
		Vec3<float> p6((p1 + p4) / 2.f);
		Vec3<float> p7((p2 + p3) / 2.f);
		Vec3<float> p8(((p5 + p6) / 2.f) + Vec3<float>(0, 0, h));
		Vec3<float> p9(((p5 + p7) / 2.f) + Vec3<float>(0, 0, h));
		Mesh m1;
		m1 = Mesh::Triangle(p1, p8, p4);
		Mesh m2;
		m2 = Mesh::Triangle(p3, p9, p2);
		m.merge(m1);
		m.merge(m2);
		Mesh m3;
		m3 = Mesh::Quadrangle(p4, p8, p9, p3);
		m.merge(m3);
		Mesh m4;
		m4 = Mesh::Quadrangle(p1, p2, p9, p8);
		m.merge(m4);
		return m;
	}

}

/*return : Mesh toit Pyramide
* h : hauteur du toit
*/
Mesh Mesh::ToitPyramide(const Vec3<float>& p1, const Vec3<float>& p2, const Vec3<float>& p3, const Vec3<float>& p4, float h)
{
	Mesh m;
	m = Mesh::Quadrangle(p1, p2, p3, p4);
	Vec3<float> p5(((p1 + p2 + p3 + p4) / 4.f) + Vec3<float>(0, 0, h));

	Mesh m1;
	m1 = Mesh::Triangle(p1, p2, p5);
	Mesh m2;
	m2 = Mesh::Triangle(p4, p5, p3);
	m.merge(m1);
	m.merge(m2);
	Mesh m3;
	m3 = Mesh::Triangle(p3, p5, p2);
	m.merge(m3);
	Mesh m4;
	m4 = Mesh::Triangle(p1, p5, p4);
	m.merge(m4);
	return m;
}

Mesh Mesh::Arbre(const Vec3<float>& o, const float& r){
	float h = r * 2;
	Mesh m;
	m = Mesh::Cylinder(o, r / 2, h, 50);
	Mesh m2;
	Vec3<float> c_sph = o + Vec3<float>{ 0, 0, h };
	m2 = Mesh::Sphere(c_sph, r , 50,50);
	m.merge(m2);
	return m;
}

Mesh Mesh::Sapin(const Vec3<float>& o, const float& r)
{
	const float MinHeight = 0.8f;
	float radius = r;

	// Hauteur min d'un arbre
	if (radius < MinHeight)
		radius = MinHeight;

	float h = radius * 2.f;

	Mesh m;
	m = Mesh::Cylinder(o, radius / 2.5, h / 2, 50);
	Mesh m2;
	Vec3<float> c_cone = o + Vec3<float>{ 0, 0, h / 2 };
	m2 = Mesh::Cone(c_cone, radius, h / 2, 50);
	m.merge(m2);
	c_cone = o + Vec3<float>{ 0, 0, h / 1.4f};
	m2 = Mesh::Cone(c_cone, radius / 1.3, h / 1.8, 50);
	m.merge(m2);
	c_cone = o + Vec3<float>{ 0, 0, h };
	m2 = Mesh::Cone(c_cone, radius / 2, h / 1.5, 50);
	m.merge(m2);
	return m;
}

Mesh::~Mesh()
{
}
