#include "EtageCircle.h"


EtageCircle::EtageCircle(const Vec3<float>& a, const Vec3<float>& b , const Vec3<float>& c, const Vec3<float>& d, const float& _h, const float& _pourcentage, const int& _etages)
{
	q = Quadrangle(a, b, c, d);
	h = _h;
	pourcentage = _pourcentage;
	etages = _etages;
}


void EtageCircle::G(Mesh& m) const{

		double _h = h / 6;
		/*****************Inter Etage************************/
		Vec3<float> origine = Vec3<float>((q.p1.x + q.p2.x + q.p3.x + q.p4.x) / 4, (q.p1.y + q.p2.y + q.p3.y + q.p4.y) / 4, (q.p1.z + q.p2.z + q.p3.z + q.p4.z) / 4);
		
		float distance1 = distance(q.p1, q.p3);
		float distance2 = distance(q.p2, q.p4);
		double r = 0;

		if (distance1 < distance2)
			r = distance1 / 4;
		else
			r = distance2 / 4;

		Mesh m2(Mesh::Cylinder(origine, r, _h, 50));
		m.merge(m2);

		/****************************************************/
		Quadrangle q2 = Quadrangle(q.p1 + Vec3<float>(0.0, 0.0, _h), q.p2 + Vec3<float>(0.0, 0.0, _h), q.p3 + Vec3<float>(0.0, 0.0, _h), q.p4 + Vec3<float>(0.0, 0.0, _h));

		origine = Vec3<float>((q2.p1.x + q2.p2.x + q2.p3.x + q2.p4.x) / 4, (q2.p1.y + q2.p2.y + q2.p3.y + q2.p4.y) / 4, (q2.p1.z + q2.p2.z + q2.p3.z + q2.p4.z) / 4);

		distance1 = distance(q.p1, q.p3);
		distance2 = distance(q.p2, q.p4);
		if (distance1 < distance2)
			r = distance1 / 4;
		else
			r = distance2 / 4;

		Mesh m3(Mesh::Cylinder(origine, r+1, h, 50));
		m.merge(m3);

		// La grammaire commence ici
		int e = rand() % 100;
		if (e < pourcentage)
		{
			_h += h;
			EtageCircle(q.p1 + Vec3<float>(0.0f, 0.0f, _h), q.p2 + Vec3<float>(0.0f, 0.0f, _h), q.p3 + Vec3<float>(0.0f, 0.0f, _h), q.p4 + Vec3<float>(0.0f, 0.0f, _h), h, pourcentage - 2.5, etages + 1).G(m);
			
			
		}
	

	
}
