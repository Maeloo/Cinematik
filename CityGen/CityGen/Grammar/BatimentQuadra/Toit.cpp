#include "Toit.h"


Toit::Toit(const Vec3<float>& a, const Vec3<float>& b, const Vec3<float>& c, const Vec3<float>& d, const float& _h,const float& _rotate,int _etage){
	q = Quadrangle(a, b, c, d);
	h = _h;
	rotate = _rotate;
	etages = _etage;
}


void Toit::G (Mesh& m ) const {

	Mesh m2;
	if (etages<2){
		int e = rand() % 100;
		if (e < 50)
			m2 = Mesh(Mesh::Toit(q.p1, q.p2, q.p3, q.p4, h));
		else
			m2 = Mesh(Mesh::ToitPyramide(q.p1, q.p2, q.p3, q.p4, h));
	}
	else if (etages>1 && etages < 5){
		int e = rand() % 100;
		if (e > 75){
			m2 = Mesh(Mesh::ToitPyramide(q.p1, q.p2, q.p3, q.p4, h));
		}
		else if (e < 75 && e>35){
			m2 = Mesh(Mesh::Toit(q.p1, q.p2, q.p3, q.p4, h));
		}
		else{

			Quadrangle q2 = q;
			q2.shrinkByDist(1.f);
			m2 = Mesh(Mesh::Box(q2.p1, q2.p2, q2.p3, q2.p4, h));
		}
	}
	else{
		Quadrangle q2 = q;
		q2.shrinkByDist(1.f);
		m2 = Mesh(Mesh::Box(q2.p1, q2.p2, q2.p3, q2.p4, h));
	}
	

	m2.transform(Transform::RotatelocalZ(rotate, m2.getPivot()));
	m.merge(m2);

}
