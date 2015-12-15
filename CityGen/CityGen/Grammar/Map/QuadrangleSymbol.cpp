#include "QuadrangleSymbol.h"

QuadrangleSymbol::QuadrangleSymbol ( ) {
}


QuadrangleSymbol::QuadrangleSymbol(const Vec3<float>& p0_, const Vec3<float>& p1_, const Vec3<float>& p2_, const Vec3<float>& p3_, const Vec3<float>& _mid, const Vec3<float>& _loin) : p0(p0_), p1(p1_), p2(p2_), p3(p3_), mid(_mid), loin(_loin)
{

}

void QuadrangleSymbol::addTrees ( Quadrangle q, const Vec3<float>& minQuad, const Vec3<float>& maxQuad, std::vector<Mesh> & m, const std::list<Quadrangle>& rdcs, const int& nbTryTree, const float& dist = 1.f ) const
{
	std::list<Circle> trees;
	std::list<Quadrangle> qtrees;
	const float MinRadius = 0.5f; // Rayon minimum pour un arbre
	const float MaxRadius = 6.f; // Rayon maximum pour un arbre
	const float DeltaQuad = 2.f; // Delta box englobante
	
	float Delta = dist; // Nombre d'essais d'ajout d'abres

	// Si on est en centre ville on fait moins de tentatives d'ajout
	if(Delta > 1.f)
		Delta = Utils::randf(dist * 0.008f, dist * 0.01f);

	const int NbTry = (int)(nbTryTree * Delta);

	for (int i = 0; i < NbTry; ++i)
	{
		bool addTree = true;
		Vec3<float> center(Utils::randf(minQuad.x + MinRadius, maxQuad.x - MinRadius), Utils::randf(minQuad.y + MinRadius, maxQuad.y - MinRadius), 0.f);
	    float rayon = Utils::randf(1.0f, 4.0f);

		// Limite max de la largeur d'un arbre
		/*if (rayon > MaxRadius)
			rayon = Utils::randf(MinRadius, MaxRadius - 2.f);*/

		Circle circle(center, rayon);

		Quadrangle tmp(Vec3<float>(center.x - rayon / DeltaQuad, center.y - rayon / DeltaQuad, 0.f),
			Vec3<float>(center.x - rayon / DeltaQuad, center.y + rayon / DeltaQuad, 0.f),
			Vec3<float>(center.x + rayon / DeltaQuad, center.y + rayon / DeltaQuad, 0.f),
			Vec3<float>(center.x + rayon / DeltaQuad, center.y - rayon / DeltaQuad, 0.f));

		if (q.isIn(tmp))
		{
			for (Quadrangle quad : rdcs)
			{
				if (quad.overlap(tmp))
				{
					addTree = false; break;
				}
			}
			if (addTree)
			{
				for (Quadrangle quad : qtrees)
				{
					if (quad.overlap(tmp))
					{
						addTree = false; break;
					}
				}
			}
			if (addTree)
			{
				qtrees.push_back(tmp);
				trees.push_back(circle); 
			}
		}
	}
	for (Circle tree : trees)
		m.push_back(Mesh::Sapin(tree.center, tree.radius));
}


void QuadrangleSymbol::Generate ( std::vector<Mesh> & m, int compteur ) const
{
	const int nbTry = 10000, nbTryTree = 50;
	Quadrangle q = Quadrangle(p0, p1, p2, p3);
	int random = rand() % 2;

	if (q.area() <  1000.f)
	{
		
		q.shrinkByDist(10.f);

		//*******************Test Centre Ville***************/
		float dif = 100 - ((distance(mid, p0) / distance(mid, loin)) * 100);
		/************************************************/

		Mesh m1 = Mesh::Quadrangle(q.p1, q.p2, q.p3, q.p4);
		m1.merge(Mesh::RouteL(p0, p1, q.p2, q.p1, 3.f, 1.F));
		m1.merge(Mesh::RouteL(p2, p3, q.p4, q.p3, 3.f, 1.f));
		m1.merge(Mesh::RouteL(p3, p0 , q.p1, q.p4, 3.f, 1.f));
		m1.merge(Mesh::RouteL(p1, p2, q.p3, q.p2, 3.f, 1.f));
		m.push_back ( m1 );
		int e = rand() % 100;
		if (e<95)
		{
			Vec3<float> minQuad = q.getMinPoint();
			Vec3<float> maxQuad = q.getMaxPoint();
			std::list<Quadrangle> rdcs;
			const float maxArea = q.area() * 0.2f;
			for (int i = 0; i < nbTry; ++i)
			{
				bool addRdc = true;
				Quadrangle tmp = Quadrangle::GenerateRectangle(Vec3<float>(Utils::randf(minQuad.x, maxQuad.x), Utils::randf(minQuad.y, maxQuad.y), 0.f),
					Vec3<float>(Utils::randf(minQuad.x, maxQuad.x), Utils::randf(minQuad.y, maxQuad.y), 0.f), Utils::randf(2.f, std::abs(maxQuad.x - minQuad.x)), Utils::randf(2.f, std::abs(maxQuad.y - minQuad.y)));


		

				//tmp.sortPoint();
				const float angleDiag = std::acos(Vec3<float>::dotProduct(Vec3<float>(p2 - p0).normalized(), Vec3<float>(p3 - p1).normalized()));
				if (q.isIn(tmp) && tmp.area() >= maxArea )
				{
					for (Quadrangle quad : rdcs)
					{
						if (quad.overlap(tmp))
						{
							addRdc = false; break;
						}
					}
					if (addRdc) rdcs.push_back(tmp);
				}
			} 
			for (Quadrangle quad : rdcs){
				int e = rand() % 100;
				if (e<80)
					RDC(quad.p1, quad.p2, quad.p3, quad.p4, 5.f, dif, 0).G(m);
				else
					RDC(quad.p1, quad.p2, quad.p3, quad.p4, 5.f, dif, 1).G(m);
				
			}
			addTrees(q, minQuad, maxQuad, m, rdcs, nbTryTree );
		}
		else{
			/*Vec3<float> o = (q.p1 + q.p2 + q.p3 + q.p4)*Vec3<float>(0.25);
			m.merge(Mesh::Sapin(o, 10));*/
			addTrees(q, q.getMinPoint(), q.getMaxPoint(), m, std::list<Quadrangle>(), nbTryTree);
		}
		
	}
	
	else if (random < 1 && q.area() > 5000.f &&  q.area() < 10000.f) // Decoupe en "quartier basique"
	{

		q.shrinkByDist(1000.f);
	
		QuadrangleSymbol(p0, p1, q.p2, q.p1, mid, loin).Generate(m, compteur - 1);
		QuadrangleSymbol(p2, p3, q.p4, q.p3, mid, loin).Generate(m, compteur - 1);
		QuadrangleSymbol(p1, p2, q.p3, q.p2, mid, loin).Generate(m, compteur - 1);
		QuadrangleSymbol(p3, p0, q.p1, q.p4, mid, loin).Generate(m, compteur - 1);
		//m.merge(Mesh::Route(q.p1, q.p2, q.p3, q.p4));
		Quadrangle q2 = q;
		q2.shrinkByDist(10.f);
		//Mesh m1 = Mesh::Quadrangle(q.p1, q.p2, q.p3, q.p4);
		Mesh m_;
		m_.merge ( Mesh::RouteL ( q.p1, q.p2, q2.p2, q2.p1, 3.f, 1.F ) );
		m_.merge(Mesh::RouteL(q.p3, q.p4, q2.p4, q2.p3, 3.f, 1.f));
		m_.merge(Mesh::RouteL(q.p4, q.p1, q2.p1, q2.p4, 3.f, 1.f));
		m_.merge ( Mesh::RouteL ( q.p2, q.p3, q2.p3, q2.p2, 3.f, 1.f ) );
		m_.merge ( Mesh::Quadrangle ( q2.p1, q2.p2, q2.p3, q2.p4 ) );
		m.push_back ( m_ );

		addTrees(q2, q2.getMinPoint(), q2.getMaxPoint(), m, std::list<Quadrangle>(), nbTryTree);
		//q.shrinkByDist(10.f);
		//QuadrangleSymbol(q.p1, q.p2, q.p3, q.p4, mid, loin).Generate(m, compteur - 1); // centre du quad
		

	}
	else if ( random < 2 && q.area ( ) > 4000.f && q.area ( ) < 8000.f )
	{
		TriangleSymbol(p0, p2, p1, mid, loin).Generate(m, compteur - 1);
		TriangleSymbol(p0, p2, p3, mid, loin).Generate(m, compteur - 1);
	}
	else
	{
		float AB = distance(p0, p1);
		float BC = distance(p1, p2);
		float CD = distance(p2, p3);
		float DA = distance(p3, p0);
		float fact = 0.5f;
		float shiftCenter1 = (float)((rand() % 2) - 1) * 0.1f;
		float shiftCenter2 = (float)((rand() % 2) - 1) * 0.1F;
		if ((AB >= BC && AB >= DA) || (CD >= BC && CD >= DA))
		{
			Vec3<float> center1 = (p0 + p1) * fact;
			Vec3<float> center2 = (p2 + p3) * fact;
			center1 = ((p1 - p0) * shiftCenter1) + center1;
			center2 = ((p3 - p2) * shiftCenter2) + center2;
			QuadrangleSymbol(center1, p1, p2, center2,mid,loin).Generate(m, compteur - 1);
			QuadrangleSymbol(p0, center1, center2, p3, mid,loin).Generate(m, compteur - 1);
		}
		else
		{
			Vec3<float> center1 = (p1 + p2) * fact;
			Vec3<float> center2 = (p3 + p0) * fact;
			center1 = ((p2 - p1) * shiftCenter1) + center1;
			center2 = ((p3 - p0) * shiftCenter2) + center2;
			QuadrangleSymbol(center1, p2, p3, center2, mid,loin).Generate(m, compteur - 1);
			QuadrangleSymbol(p1, center1, center2, p0, mid,loin).Generate(m, compteur - 1);
		}
	}
}

/* Genere un quadrangle symbole et un mesh avec une bordure de route
* p1, p2, p3, p4 : coordonnées du quadrangle de base
* borderSize : largeur de la bordure
* sizePavement, hPavement : dimension du trottoir
* m : mesh auquel on rajoute la bordure
* mid : milieu pour definir le centre ville
*/
QuadrangleSymbol QuadrangleSymbol::genBorder ( const Vec3<float>& p1, const Vec3<float>& p2, const Vec3<float>&p3, const Vec3<float>& p4, const float& borderSize, const float& sizePavement, const float& hPavement, std::vector<Mesh>& m, const Vec3<float>& _mid, const Vec3<float>& _loin )
{
	Quadrangle q(p1, p2, p3, p4);
	Quadrangle q2(q);
	q.shrinkByDist(borderSize);
	QuadrangleSymbol qs(q.p1, q.p2, q.p3, q.p4, _mid,_loin);

	Mesh m2;
	m2.merge(Mesh::RouteR(q2.p1, q2.p2, q.p2, q.p1, sizePavement, hPavement));
	m2.merge(Mesh::RouteR(q2.p3, q2.p4, q.p4, q.p3, sizePavement, hPavement));
	m2.merge(Mesh::RouteR(q2.p4, q2.p1, q.p1, q.p4, sizePavement, hPavement));
	m2.merge(Mesh::RouteR(q2.p2, q2.p3, q.p3, q.p2, sizePavement, hPavement));
	m.push_back ( m2 );
	return qs;
}
QuadrangleSymbol::~QuadrangleSymbol ( ) {
}
