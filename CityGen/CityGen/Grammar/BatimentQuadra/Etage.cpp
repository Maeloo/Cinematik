#include "Etage.h"


Etage::Etage(const Vec3<float>& a, const Vec3<float>& b, const Vec3<float>& c, const Vec3<float>& d, const float& _h, const float& _pourcentage, const int& _type, const float& _rotate, const bool& _duo, const int& _etages, const float& _window_size){
	q = Quadrangle(a, b, c, d);
	h = _h;
	pourcentage = _pourcentage;
	type = _type;
	rotate = _rotate;
	duo = _duo;
	etages = _etages;
	window_size = _window_size;
}


void Etage::G(Mesh& m) const
{
	if (type == 0)
	{
		float _h = h / 6;
		/*****************Inter Etage************************/
		Quadrangle q2 = q;
		q2.shrinkByDist(2.f);
		Mesh m2(Mesh::Box(q2.p1, q2.p2, q2.p3, q2.p4, _h));
		m.merge(m2);
		/****************************************************/

		q2 = Quadrangle(q.p1 + Vec3<float>(0.0, 0.0, _h), q.p2 + Vec3<float>(0.0, 0.0, _h), q.p3 + Vec3<float>(0.0, 0.0, _h), q.p4 + Vec3<float>(0.0, 0.0, _h));
		q2.shrinkByDist(1.f);
		Mesh m3(Mesh::Box(q2.p1, q2.p2, q2.p3, q2.p4, h));
		addWindows(m3, q2.p1, q2.p2, q2.p3, q2.p4, h, etages, window_size);
		m.merge(m3);

		// La grammaire commence ici
		int e = rand() % 100;
		if (e < pourcentage)
		{
			
			if (e < 10 && duo!=true)
			{
				_h += h;
				float distance1 = distance(q.p1, q.p2);
				float distance2 = distance(q.p1, q.p4);

				if (distance1 < distance2)
				{
					distance1 = distance(q.p2, q.p3);
					Vec3<float> p3modif = q.p2 - (Vec3<float>(q.p2 - q.p3).normalized() * (distance1 / 2)) + Vec3<float>(0.0, 0.0, _h);
					Vec3<float> p4modif = q.p1 - (Vec3<float>(q.p1 - q.p4).normalized() * (distance2 / 2)) + Vec3<float>(0.0, 0.0, _h);

					Etage(q.p1 + Vec3<float>(0.0f, 0.0f, _h), q.p2 + Vec3<float>(0.0f, 0.0f, _h), p3modif, p4modif, h, pourcentage - 2.5f, 0, rotate, true, etages + 1, window_size).G(m);
					//Etage(q.p1 + Vec3<float>(0.0, 0.0, _h), q.p2 + Vec3<float>(0.0, 0.0, _h), q.p3 + Vec3<float>(0.0, 0.0, _h), q.p4 + Vec3<float>(0.0, 0.0, _h), h, pourcentage - 2.5, 0, rotate).G(m);
					Etage(p4modif, p3modif, q.p3 + Vec3<float>(0.0f, 0.0f, _h), q.p4 + Vec3<float>(0.0f, 0.0f, _h), h, pourcentage - 2.5f, 0, rotate, true, etages + 1, window_size).G(m);
				}
				else
				{
					distance2 = distance(q.p3, q.p4);
					Vec3<float> p2modif = q.p1 - (Vec3<float>(q.p1 - q.p2).normalized() * (distance1 / 2)) + Vec3<float>(0.0, 0.0, _h);
					Vec3<float> p3modif = q.p4 - (Vec3<float>(q.p4 - q.p3).normalized() * (distance2 / 2)) + Vec3<float>(0.0, 0.0, _h);

					Etage(q.p1 + Vec3<float>(0.0f, 0.0f, _h), p2modif, p3modif, q.p4 + Vec3<float>(0.0f, 0.0f, _h), h, pourcentage - 2.5f, 0, rotate, true, etages + 1, window_size).G(m);
					//Etage(q.p1 + Vec3<float>(0.0, 0.0, _h), q.p2 + Vec3<float>(0.0, 0.0, _h), q.p3 + Vec3<float>(0.0, 0.0, _h), q.p4 + Vec3<float>(0.0, 0.0, _h), h, pourcentage - 2.5, 0, rotate).G(m);
					Etage(p2modif, q.p2 + Vec3<float>(0.0f, 0.0f, _h), q.p3 + Vec3<float>(0.0f, 0.0f, _h), p3modif, h, pourcentage - 2.5f, 0, rotate, true, etages + 1, window_size).G(m);
				}
			}
			else
			{
				_h += h;
				Etage(q.p1 + Vec3<float>(0.0f, 0.0f, _h), q.p2 + Vec3<float>(0.0f, 0.0f, _h), q.p3 + Vec3<float>(0.0f, 0.0f, _h), q.p4 + Vec3<float>(0.0f, 0.0f, _h), h, pourcentage - 2.5f, 0, rotate, duo, etages + 1, window_size).G(m);
			}
		

		}
		else
		{
			_h += h;
			Toit(q.p1 + Vec3<float>(0.0, 0.0, _h), q.p2 + Vec3<float>(0.0, 0.0, _h), q.p3 + Vec3<float>(0.0, 0.0, _h), q.p4 + Vec3<float>(0.0, 0.0, _h), h, rotate, etages).G(m);

		}
	}

	//****************************************//
	//************Building Twist**************//
	//***************************************//
	else if (type == 1)
	{
		float _rotate = rotate + Constantes::PI * 0.025f;
		float _h = h / 6;

		/*****************Inter Etage************************/
		Quadrangle q2 = q;
		q2.shrinkByDist(2.f);
		Mesh m2(Mesh::Box(q2.p1, q2.p2, q2.p3, q2.p4, _h));
		m2.transform(Transform::RotatelocalZ(_rotate, m2.getPivot()));
		m.merge(m2);
		/****************************************************/

		q2 = Quadrangle(q.p1 + Vec3<float>(0.0, 0.0, _h), q.p2 + Vec3<float>(0.0, 0.0, _h), q.p3 + Vec3<float>(0.0, 0.0, _h), q.p4 + Vec3<float>(0.0, 0.0, _h));
		q2.shrinkByDist(1.f);
		Mesh m3(Mesh::Box(q2.p1, q2.p2, q2.p3, q2.p4, h));
		addWindows(m3, q2.p1, q2.p2, q2.p3, q2.p4, h, etages, window_size);
		m3.transform(Transform::RotatelocalZ(_rotate, m3.getPivot()));
		m.merge(m3);

		// La grammaire commence ici
		int e = rand() % 100;
		if (e < pourcentage)
		{
			_h += h;
			Etage(q.p1 + Vec3<float>(0.0f, 0.0f, _h), q.p2 + Vec3<float>(0.0f, 0.0f, _h), q.p3 + Vec3<float>(0.0f, 0.0f, _h), q.p4 + Vec3<float>(0.0f, 0.0f, _h), h, pourcentage - 2.5f, 1, _rotate, duo, etages + 1, window_size).G(m);
		}
		else
		{
			Toit(q.p1 + Vec3<float>(0.0f, 0.0f, h), q.p2 + Vec3<float>(0.0f, 0.0f, h), q.p3 + Vec3<float>(0.0f, 0.0f, h), q.p4 + Vec3<float>(0.0f, 0.0f, h), h, _rotate, etages).G(m);

		}
	}
}

// Ajoute des fenêtres sur les 4 côtés du quadrangle
// - m : Mesh à merger
// - p1, p2, p3, p3 : coordonnées du quadrangle
// - hauteur : hauteur de l'étage
// - etage : étage actuel
void Etage::addWindows(Mesh& m, const Vec3<float>& p1, const Vec3<float>& p2, const Vec3<float>& p3, const Vec3<float>& p4, const float& h, const int& etage, const float& win_size)
{
	float min_dist = 4.f;
	float s = 0.6f; // Shrink hauteur

	if (distance(p1, p2) > min_dist)
		addWindowsSide(m, p1, p2, h, s, etage, win_size);
	if (distance(p2, p3) > min_dist)
		addWindowsSide(m, p2, p3, h, s, etage, win_size);
	if (distance(p3, p4) > min_dist)
		addWindowsSide(m, p3, p4, h, s, etage, win_size);
	if (distance(p4, p1) > min_dist)
		addWindowsSide(m, p4, p1, h, s, etage, win_size);
}

// Ajoute un côté de fenêtre
// - m : Mesh à merger
// - p1 : point gauche du côté
// - p2 : point droite du côté
// - hauteur : hauteur de l'étage
// - shrink : pourcentage définissant la hauteur des fenêtres
// - etage : étage actuel
void Etage::addWindowsSide(Mesh& m, const Vec3<float>& p1, const Vec3<float>& p2, const float& hauteur, const float& shrink, const int& etage, const float& win_size)
{
	float n; // écart entre deux fenêtres
	float l; // largeur fenêtre
	float h; // hauteur fenêtre
	float p; // profondeur fenêtre

	Quadrangle q(p1, p2, p2 + Vec3<float>(0.f, 0.f, hauteur), p1 + Vec3<float>(0.f, 0.f, hauteur));
	q.shrink(shrink);

	if (etage >= 0)
		p = 0.1f;
	else
		p = 0.3f;

	l = 1.5f;
	h = hauteur * shrink;

	if (etage > 1)
		n = 0.5f;
	else
		n = 3.f;

	if (etage == 0 || etage == 1)
	{
		int lg = rand() % 100;
		l += (float)lg / 100.f;
	}
	else if (win_size > 0.f)
		l = win_size;
		
	Vec3<float> dir_p1(Vec3<float>::crossProduct(q.p4 - q.p1, q.p2 - q.p1));
	Vec3<float> dir_p12(q.p2 - q.p1);

	dir_p1 = dir_p1.normalized();
	dir_p12 = dir_p12.normalized();

	int nb_windows = (int)(distance(q.p1, q.p2) / (l + n));

	int tweak_porte = -1;
	if (etage < 0)
	{
		if (nb_windows > 1)
			tweak_porte = rand() % (nb_windows - 1) + 1;
		else
			tweak_porte = 1;
	}

	for (int i = 0; i <= nb_windows; i++)
	{
		bool ok = etage > 1;

		if (etage == 0 || etage == 1)
		{
			int e = rand() % 100;
			ok = e <= 50;
		}
		else if (etage < 0)
			ok = i == tweak_porte;

		if (ok)
		{
			Vec3<float> w1(q.p1 + (float)i * (l + n) * dir_p12);

			Vec3<float> w2(w1 + dir_p1 * p);
			Vec3<float> w3(w2 + dir_p12 * l);
			Vec3<float> w4(w1 + dir_p12 * l);

			if (distance(p1, p2) > distance(p1, w4))
			{
				Mesh win(Mesh::Box(w1, w2, w3, w4, h));
				m.merge(win);
			}
		}
	}
}


