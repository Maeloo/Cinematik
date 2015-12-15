#pragma once

#include <algorithm>
#include "../Utils/Vec3.h"
#include "../Transform/Transform.h"
#include "Geometry.h"
#include "Triangle.h"
#include "../Utils/Utils.h"


class Quadrangle : public Geometry {

public:
	Vec3<float> p1, p2, p3, p4;
	Quadrangle ( );
	Quadrangle ( Vec3<float> p1_, Vec3<float> p2_, Vec3<float> p3_, Vec3<float> p4_ );

	/*
	*return : aire du quad courant
	*/
	float area ( ) const;
	/*
	*return : perimetre du quad courant
	*/
	float perimeter ( ) const;
	/* mise a l'echelle du quad
	* t : pourcentage de mise a l'echelle
	*/
	void shrink ( float t );
	/* mise a l'echelle du quad
	* distance : pourcentage de mise a l'echelle
	*/
	void shrinkByDist ( float distance );
	/* verifie si un des cotes d'un quad coupe un des cotes du quadrangle courant
	* q : quad
	* return : vrai si un cote de q coupe un cote du quadrangle courant
	*/
	bool intersect(Quadrangle q);
	/* verifie si un point est a l'interieur du quadrangle
	* p : point
	* return vrai si p est dans le quad
	*/
	bool isIn(Vec3<float> p);
	/* verifie si un quad est a l'interieur du quadrangle
	* q : quad
	* return vrai si q est dans le quad
	*/
	bool isIn(Quadrangle);
	/* verifie si un quad touche le quad courant
	* q : quad
	* return : vrai si q touche le quad courant
	*/ 
	bool overlap(Quadrangle);
	/* renvoi le min des points du quad
	* return min des points du quad
	*/
	Vec3<float> getMinPoint();
	/* renvoi le max des points du quad
	* return max des points du quad
	*/
	Vec3<float> getMaxPoint();
	/* verifie si le quadrangle une normale correcte ppour l'affichage
	* renvoi vrai si le normale est correcte
	*/
	bool hasGoodNormal();
	/* Genere un Quad rectangle a partir des points de sa diagonale
	*/
	static Quadrangle GenerateRectangle(Vec3<float> p0, Vec3<float> p1, float width, float height);
	~Quadrangle ( );
};

