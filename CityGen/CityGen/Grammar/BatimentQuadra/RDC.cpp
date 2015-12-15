#include "RDC.h"


RDC::RDC(const Vec3<float>& a, const Vec3<float>& b, const Vec3<float>& c, const Vec3<float>& d, const float& _h,const float& _dif,const int& _type)
{
	q = Quadrangle(a, b, c, d);
	h = _h;
	dif = _dif;
	type = _type;
}

void RDC::G ( std::vector<Mesh>& m ) const
{
	Mesh m2(Mesh::Box(q.p1, q.p2, q.p3, q.p4, h));
	

	addDoor(m2, q.p1, q.p2, q.p3, q.p4, h);

	// La grammaire commence ici
	int e = rand()%100;
	if (e < dif)
	{
		int e = rand() % 100;
		if (e < 95){
			float window_size = 1.5f + (float)(rand() % 100) / 100.f;
			Etage(q.p1 + Vec3<float>(0.0, 0.0, h), q.p2 + Vec3<float>(0.0, 0.0, h), q.p3 + Vec3<float>(0.0, 0.0, h), q.p4 + Vec3<float>(0.0, 0.0, h), h, dif, type, 0.f, false, 0, window_size).G(m2);
		}
		else{
			EtageCircle(q.p1 + Vec3<float>(0.0, 0.0, h), q.p2 + Vec3<float>(0.0, 0.0, h), q.p3 + Vec3<float>(0.0, 0.0, h), q.p4 + Vec3<float>(0.0, 0.0, h), h, dif+20, 0).G(m2);
		}
		
		
	}
	else
	{
		Toit(q.p1 + Vec3<float>(0.0, 0.0, h), q.p2 + Vec3<float>(0.0, 0.0, h), q.p3 + Vec3<float>(0.0, 0.0, h), q.p4 + Vec3<float>(0.0, 0.0, h), h,0.f,0).G(m2);
	}

	m.push_back ( m2 );
}

void RDC::addDoor(Mesh& m, const Vec3<float>& p1, const Vec3<float>& p2, const Vec3<float>& p3, const Vec3<float>& p4, const float& h)
{
	int selected_side = rand() % 4;
	float s = 0.4f; // Shrink hauteur
	Vec3<float> p1_door, p2_door;

	if (selected_side == 0)
	{
		p1_door = p1;
		p2_door = p2;
		Etage::addWindowsSide(m, p2, p3, h, s, 0, -1.f);
		Etage::addWindowsSide(m, p3, p4, h, s, 0, -1.f);
		Etage::addWindowsSide(m, p4, p1, h, s, 0, -1.f);
	}
	else if (selected_side == 1)
	{
		p1_door = p2;
		p2_door = p3;
		Etage::addWindowsSide(m, p1, p2, h, s, 0, -1.f);
		Etage::addWindowsSide(m, p3, p4, h, s, 0, -1.f);
		Etage::addWindowsSide(m, p4, p1, h, s, 0, -1.f);
	}
	else if (selected_side == 2)
	{
		p1_door = p3;
		p2_door = p4;
		Etage::addWindowsSide(m, p1, p2, h, s, 0, -1.f);
		Etage::addWindowsSide(m, p2, p3, h, s, 0, -1.f);
		Etage::addWindowsSide(m, p4, p1, h, s, 0, -1.f);
	}
	else
	{
		p1_door = p4;
		p2_door = p1;
		Etage::addWindowsSide(m, p1, p2, h, s, 0, -1.f);
		Etage::addWindowsSide(m, p2, p3, h, s, 0, -1.f);
		Etage::addWindowsSide(m, p3, p4, h, s, 0, -1.f);
	}

	Etage::addWindowsSide(m, p1_door, p2_door, h * 0.6f, 1.f, -1, -1.f);
}