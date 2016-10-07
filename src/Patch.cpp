

#include "Patch.h"
#include "Frustum.h"

#include <sstream>
#include <iostream>
#include <stdlib.h>

#include <QImage>
#include <QGLWidget>

#ifndef M_PI
	# define M_PI           3.14159265358979323846  /* pi */
#endif

#ifndef GL_BGR
	#define GL_BGR 0x80E0
#endif

#define radius 40.0f

#define steps 100

template <typename T>
std::string NumberToString ( T Number )
{
     std::ostringstream ss;
     ss << Number;
     return ss.str();
}

Patch::Patch(int x, int y, int nbTilesX, int nbTilesY) : m_x(x), m_y(y), requiresUpdate(true)
{
	//create Geometry
	createGeometry(nbTilesX, nbTilesY);
}

Patch::~Patch()
{
	pointcoordsx.clear();
	pointcoordsy.clear();
	pointcoordsz.clear();

	glDeleteLists(drawList,1);
	glDeleteTextures(1, &texture);
}

void Patch::initialise()
{
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set texture clamping method
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Patch::createGeometry(int nbTilesX, int nbTilesY)
{
	double step = steps;
	double width_size = 2.0f * M_PI / nbTilesX;
	double width_angle_start = M_PI + width_size * m_x;
	double width_step = width_size / step;
	
	double height_size = M_PI / nbTilesY;
	double height_angle_start = height_size * m_y;
	double height_step = height_size / step;

	double tex_step = 1.0 / (step);

	
	double x0, y0, z0, alpha, beta;
	for (int x = 0; x <= steps; x++)
	{
		for (int y = 0; y <= steps; y++)
		{
			alpha = width_angle_start + width_step * x;
			beta = height_angle_start + height_step * y;
			x0 = radius  * sin(beta) * sin(alpha);
			y0 = radius  * cos(beta);
			z0 = -radius  * sin(beta) * cos(alpha);
			pointcoordsx.push_back(x0);
			pointcoordsy.push_back(y0);
			pointcoordsz.push_back(z0);
		}
	}

	drawList = glGenLists(1);
	glNewList(drawList, GL_COMPILE);
	glColor3ub(255.0, 255.0, 255);
	glBegin(GL_QUADS);
	for (int x = 0; x < steps; x++)
	{
		for (int y = 0; y < steps; y++)
		{
			alpha = width_angle_start + width_step * x;
			beta = height_angle_start + height_step * y;
			x0 = radius  * sin(beta) * sin(alpha);
			y0 = radius  * cos(beta);
			z0 = - radius  * sin(beta) * cos(alpha);

			glTexCoord2d(tex_step * x, tex_step *y);
			glVertex3d(x0, y0, z0);

			alpha = width_angle_start + width_step * x;
			beta = height_angle_start + height_step * (y + 1);
			x0 = radius  * sin(beta) * sin(alpha);
			y0 = radius  * cos(beta);
			z0 = -radius  * sin(beta) * cos(alpha);

			glTexCoord2d(tex_step * x, tex_step *(y + 1));
			glVertex3d(x0, y0, z0);


			alpha = width_angle_start + width_step * (x + 1);
			beta = height_angle_start + height_step * (y + 1);
			x0 = radius  * sin(beta) * sin(alpha);
			y0 = radius  * cos(beta);
			z0 = -radius  * sin(beta) * cos(alpha);

			glTexCoord2d(tex_step * (x + 1), tex_step * (y + 1));
			glVertex3d(x0, y0, z0);

			alpha = width_angle_start + width_step * (x + 1);
			beta = height_angle_start + height_step * y;
			x0 = radius  * sin(beta) * sin(alpha);
			y0 = radius  * cos(beta);
			z0 = -radius  * sin(beta) * cos(alpha);

			glTexCoord2d(tex_step * (x + 1), tex_step * y);
			glVertex3d(x0, y0, z0);
		}
	}
	glEnd();
	glEndList();
}

bool Patch::getVisibilty(){
	return visibility;
}

bool Patch::testVisibility(Frustum * frustum)
{
	for (int i = 0; i < pointcoordsx.size(); i++){
		if (frustum->testPoint(pointcoordsx[i],pointcoordsy[i],pointcoordsz[i]))
		{
			visibility = true;
			return true;		
		}
	}
	visibility = false;
	return false;
}


void Patch::update()
{
	if (visibility && requiresUpdate){
		QImage im("data/tile" + QString::number(m_x) + "_" + QString::number(m_y) + ".jpg");
		QImage flipped = im.mirrored(false,true);

		QImage tex = QGLWidget::convertToGLFormat(flipped);

		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D,     //Always GL_TEXTURE_2D
			0,								//0 for now
			GL_RGBA,						//Format OpenGL uses for image
			512, 512,						//Width and height
			0,                              //The border of the image
			GL_RGBA,						//GL_RGB, because pixels are stored in RGB format
			GL_UNSIGNED_BYTE,				//GL_UNSIGNED_BYTE, because pixels are stored
			tex.bits());

		glBindTexture(GL_TEXTURE_2D, 0);
		requiresUpdate = false;
	}
}

void Patch::display()
{
	if (visibility){
		glBindTexture(GL_TEXTURE_2D, texture);
		glCallList(drawList);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
