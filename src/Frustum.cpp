#include "Frustum.h"

Frustum::Frustum()
{
}

Frustum::~Frustum()
{
}

void Frustum::update(double* projection, double*  modelview)
{
	float a, b, c, d;
	a = mvp[3] + mvp[2];
	b = mvp[7] + mvp[6];
	c = mvp[11] + mvp[10];
	d = mvp[15] + mvp[14];
	planes[0].setAndNormalize(a, b, c, d); //near

	a = mvp[3] - mvp[2];
	b = mvp[7] - mvp[6];
	c = mvp[11] - mvp[10];
	d = mvp[15] - mvp[14];
	planes[1].setAndNormalize(a, b, c, d); //far

	a = mvp[3] + mvp[0];
	b = mvp[7] + mvp[4];
	c = mvp[11] + mvp[8];
	d = mvp[15] + mvp[12];
	planes[2].setAndNormalize(a, b, c, d); //left

	a = mvp[3] - mvp[0];
	b = mvp[7] - mvp[4];
	c = mvp[11] - mvp[8];
	d = mvp[15] - mvp[12];
	planes[3].setAndNormalize(a, b, c, d); //right

	a = mvp[3] + mvp[1];
	b = mvp[7] + mvp[5];
	c = mvp[11] + mvp[9];
	d = mvp[15] + mvp[13];
	planes[4].setAndNormalize(a, b, c, d); // bottom

	a = mvp[3] - mvp[1];
	b = mvp[7] - mvp[5];
	c = mvp[11] - mvp[9];
	d = mvp[15] - mvp[13];
	planes[5].setAndNormalize(a, b, c, d); // top
}


bool Frustum::testPoint(float px, float py, float pz){
	for (int i = 0; i<6; i++){
		if (planes[i].a*px + planes[i].b*py + planes[i].c*pz + planes[i].d < 0){
			return false;
		}
	}
	return true;
}

	
