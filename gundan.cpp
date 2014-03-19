#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "vec.h"
#include <FL/gl.h>

#include "modelerglobals.h"
typedef Vec3<double> v3;
#define PI 3.14159265

// To make a Gundan, we inherit off of ModelerView
class Gundan : public ModelerView 
{
public:
    Gundan(int x, int y, int w, int h, char *label) 
        : ModelerView(x,y,w,h,label) { }
    virtual void draw();
private:
	void drawHead();
	void drawBody();
	void drawRighthand();
	void drawLefthand();
	void drawHip();
	void drawLeftThign();
	void drawRightThign();
	void drawLeftShank();
	void drawRightShank();
	void drawSword();
};

void drawTriangle(v3 a, v3 b, v3 c) {
	drawTriangle(a[0], a[1], a[2], b[0], b[1], b[2], c[0], c[1], c[2]);
}

void drawQuad(v3 a, v3 b, v3 c, v3 d) {
	drawTriangle(a, b, c);
	drawTriangle(c, d, a);
}

void drawQuadruple(v3 a1, v3 b1, v3 c1, v3 d1,
				   v3 a2, v3 b2, v3 c2, v3 d2)
{
	//a1, b1, c1, d1 are belong to the bottom surface
	drawQuad(d1, c1, b1, a1);
	drawQuad(a1, a2, d2, d1);
	drawQuad(d1, d2, c2, c1);
	drawQuad(c1, c2, b2, b1);
	drawQuad(b1, b2, a2, a1);
	drawQuad(a2, b2, c2, d2);
}


void drawPrism(double x1, double y1, double z1,
			   double x2, double y2, double z2,
			   double x3, double y3, double z3, double h)
{
	v3 a1(x1, y1, z1);
	v3 b1(x2, y2, z2);
	v3 c1(x3, y3, z3);
	v3 normal = crossProduct((b1-a1), (c1-a1));
	normal.normalize();
	drawTriangle(c1, b1 , a1);
	v3 a2 = a1 + normal * h;
	v3 b2 = b1 + normal * h;
	v3 c2 = c1 + normal * h;
	drawQuad(a1, a2, c2, c1);
	drawQuad(b1, b2, a2, a1);
	drawQuad(c1, c2, b2, b1);
	drawTriangle(a2, b2, c2);
}

void drawPrism(v3 a, v3 b, v3 c, double h) {
	drawPrism(a[0], a[1], a[2], b[0], b[1], b[2], c[0], c[1], c[2], h);
}

void drawTorus(double r, double p)
{
	double a, b;
	double x, y, z;
	double step = 0.05;
	for(double d=0; d<2*PI; d+=step) {
		a = r * cos(d);
		b = r * sin(d);
		glBegin(GL_LINE_LOOP);
		for(double phi=0; phi<PI; phi+=step) {
			x = p * cos(d) * sin(phi);
			y = p * sin(d) * sin(phi);
			z = p * cos(phi);
			glNormal3d(x, y, z);
			glVertex3d(a+x, b+y, z);
		}
		glEnd();
		glBegin(GL_LINE_LOOP);
		for(double phi=0; phi<PI; phi+=step) {
			x = p * cos(d) * sin(phi);
			y = p * sin(d) * sin(phi);
			z = p * cos(phi);
			glNormal3d(x, y, z);
			glVertex3d(a-x/4, b-y/4, z);
		}
		glEnd();
	}
}

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createGundan(int x, int y, int w, int h, char *label)
{ 
    return new Gundan(x,y,w,h,label); 
}

void Gundan::drawRightThign()
{
	double kneel = VAL(RKNEEL);
	//draw upper joint
	glTranslated(0.1, -1.6, 0);
	glRotated(kneel+VAL(RLEGZ), -1.0, 0.0, 0.0); 
	glRotated(VAL(RLEGX),0.0, 0.0, 1.0); 

	//draw upper joint
	glPushMatrix();
	glRotated(90, 0.0, 1.0, 0.0);
	setDiffuseColor(0.6f, 0.6f, 0.6f);
	drawCylinder(1.2, 0.3, 0.3);
	glPopMatrix();

	//draw upper leg
	double ulen=-1.5;
	v3 a1(0.9, ulen, -0.3);
	v3 b1(0.3, ulen, -0.3);
	v3 c1(0.3, ulen, 0.3);
	v3 d1(0.9, ulen, 0.3);
	v3 a2(1.1, 0.0, -0.3);
	v3 b2(0.2, 0.0, -0.3);
	v3 c2(0.2, 0.0, 0.3);
	v3 d2(1.1, 0.0, 0.3);
	setDiffuseColor(1.0f, 1.0f, 1.0f);
	drawQuadruple(a1, b1, c1, d1, a2, b2, c2, d2);
	glTranslated(0.3, ulen-0.1, 0);
}

void Gundan::drawRightShank()
{
	double kneel = VAL(RKNEEL);
	//draw middle joint
	glRotated(2 * kneel+VAL(RSHANKZ), 1.0, 0.0, 0.0); 
	glPushMatrix();
	{
		glRotated(90, 0.0, 1.0, 0.0);
		setDiffuseColor(0.6f, 0.6f, 0.6f);
		drawCylinder(0.6, 0.3, 0.3);
	}
	glPopMatrix();

	//draw lower leg
	double llen=-2.0;
	glTranslated(0.0, llen, -0.25);
	setDiffuseColor(1.0f, 1.0f, 1.0f);
	drawBox(0.6, abs(llen), 0.5);

	//draw foot
	double footheight=0.5;
	glTranslated(-0.2, -footheight, 0);
	v3 a1(0.9, 0, -0.1);
	v3 b1(0, 0, -0.1);
	v3 c1(0.2, 0, 1.2);
	v3 d1(0.7, 0, 1.2);
	v3 a2(0.8, footheight, 0);
	v3 b2(0.2, footheight, 0);
	v3 c2(0.2, footheight, 0.6);
	v3 d2(0.8, footheight, 0.6);
	setDiffuseColor(COLOR_RED);
	drawQuadruple(a1, b1, c1, d1, a2, b2, c2, d2);
}

void Gundan::drawLeftThign()
{
	double kneel = VAL(LKNEEL);
	int level = VAL(LEVEL);
	glTranslated(-0.1, -1.6, 0);
	glRotated(kneel+VAL(LLEGZ), -1.0, 0.0, 0.0); 
	glRotated(VAL(LLEGX),0.0, 0.0, -1.0); 

	//draw upper join
	glPushMatrix();
	glRotated(-90, 0.0, 1.0, 0.0);
	setDiffuseColor(0.6f, 0.6f, 0.6f);
	drawCylinder(1.2, 0.3, 0.3);
	glPopMatrix();

	//draw upper leg
	double ulen=-1.5;
	v3 a1(-0.9, ulen, -0.3);
	v3 b1(-0.3, ulen, -0.3);
	v3 c1(-0.3, ulen, 0.3);
	v3 d1(-0.9, ulen, 0.3);
	v3 a2(-1.1, 0.0, -0.3);
	v3 b2(-0.2, 0.0, -0.3);
	v3 c2(-0.2, 0.0, 0.3);
	v3 d2(-1.1, 0.0, 0.3);
	setDiffuseColor(1.0f, 1.0f, 1.0f);
	drawQuadruple(d1, c1, b1, a1, d2, c2, b2, a2);
	glTranslated(-0.3, ulen-0.1, 0);
}

void Gundan::drawLeftShank()
{
	double kneel = VAL(LKNEEL);
	glRotated(2 * kneel +VAL(LSHANKZ), 1.0, 0.0, 0.0); 

	//draw middle joint
	glPushMatrix();
	glRotated(-90, 0.0, 1.0, 0.0);
	setDiffuseColor(0.6f, 0.6f, 0.6f);
	drawCylinder(0.6, 0.3, 0.3);
	glPopMatrix();

	//draw lower leg
	double llen=-2.0;
	glTranslated(0.0, llen, -0.25);
	glPushMatrix();
	glTranslated(-0.6, 0, 0);
	setDiffuseColor(1.0f, 1.0f, 1.0f);
	drawBox(0.6, abs(llen), 0.5);
	glPopMatrix();
	//draw foot
	double footheight=0.5;
	glTranslated(0.2, -footheight, 0);
	v3 a1(-0.9, 0, -0.1);
	v3 b1(-0, 0, -0.1);
	v3 c1(-0.2, 0, 1.2);
	v3 d1(-0.7, 0, 1.2);
	v3 a2(-0.8, footheight, 0);
	v3 b2(-0.2, footheight, 0);
	v3 c2(-0.2, footheight, 0.6);
	v3 d2(-0.8, footheight, 0.6);
	setDiffuseColor(COLOR_RED);
	drawQuadruple(d1, c1, b1, a1, d2, c2, b2, a2);
}

void drawTextureRect(double x1, double y1, double x2, double y2)
{ 
	/* remember which matrix mode OpenGL was in. */
	int savemode;
	glGetIntegerv( GL_MATRIX_MODE, &savemode );
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	//attach texture
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	const int w=15, h=15;
	float pixels[w][h][3];
	bool blue=true;
	for(int i=0; i<w; i++) {
		for(int j=0; j<w; j++) {
			if(blue) {
				pixels[i][j][0]=0.0;
				pixels[i][j][1]=1.0;
				pixels[i][j][2]=0.0;
			} else {
				pixels[i][j][0]=0.0;
				pixels[i][j][1]=0.0;
				pixels[i][j][2]=1.0;
			}
			blue = !blue;
		}
	};
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_FLOAT, pixels);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBegin( GL_QUADS );
	
	glNormal3d( 0.0, 0.0, 1.0 );
	glTexCoord2f(0.0, 0.0); glVertex3f(x1, y1, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(x1, y2, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(x2, y2, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(x2, y1, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	/* restore the model matrix stack, and switch back to the matrix
	mode we were in. */
	glPopMatrix();
	glMatrixMode( savemode );
}

void Gundan::drawBody()
{
	//draw GN driver
	glTranslated(0, 0.5, -0.5-0.8);
	setDiffuseColor(1.0f, 1.0f, 1.0f);
	drawCylinder(0.8, 0.01, 0.4);
	//draw Body
	glTranslated(-1, -1.5, 0.8);
	setDiffuseColor(0.5f, 0.5f, 0.5f);
	drawBox(2, 2.5, 1);
	//draw texture
	glTranslated(0, 0, 1.01);
	drawTextureRect(0,0, 2, 2.5);
	//draw torus
	glTranslated(1, 1.5, 0);
	setDiffuseColor(COLOR_GREEN);
	drawSphere(0.35);
	setDiffuseColor(1.0f, 1.0f, 1.0f);
	glLineWidth(4.0);
	drawTorus(0.4, 0.2);
}

void Gundan::drawHead()
{	
	glTranslated(-0.5, 1.5, -1.01);
	setDiffuseColor(1.0f, 1.0f, 1.0f);
	drawBox(1, 1, 1);
	glTranslated(0.3, 0.6, 1);
	setDiffuseColor(COLOR_RED);
	drawBox(0.4, 0.8, 0.1);
	setDiffuseColor(1.0f, 1.0f, 0);
	drawPrism(0, 0, 0, 0, 0.4, 0, -1.6, 1.6, 0, 0.1);
	drawPrism(0.4, 0, 0, 2.0, 1.6, 0, 0.4, 0.4, 0, 0.1);
}

void Gundan::drawLefthand()
{
	double armLength = 3.5;
	glTranslated(-1.5, 1.2, -0.5); 
	glRotated(VAL(LHANDX), 0, 0, -1);
	glRotated(VAL(LHANDZ), -1, 0, 0);
	glTranslated(0, -armLength, -0.5); 
	setDiffuseColor(COLOR_BLUE);
	v3 a(0.4, 0.3+armLength, 0);
	v3 b(0.4, -0.2+armLength, 0);
	v3 c(-0.6, 0.3+armLength, 0);
	v3 d(0, armLength, 0);
	v3 e(-0.6, armLength, 0);
	drawPrism(a, c, b, 1.0);
	drawPrism(d, c, e, 1.0);
	setDiffuseColor(1.0f, 1.0f, 1.0f);
	drawBox(e[0]-d[0], armLength, 1.0);
	if(VAL(SWORD)) {
		glTranslated((e[0]-d[0])/2, 0.0, 1.0);
		drawSword();
	}
}

void Gundan::drawRighthand()
{
	double armLength = 3.5;
	glTranslated(1.5, 1.2, -0.5); 
	glRotated(VAL(RHANDX), 0, 0, 1);
	glRotated(VAL(RHANDZ), 1, 0, 0);
	glTranslated(0, -armLength, -0.5); 
	setDiffuseColor(COLOR_BLUE);
	v3 a(-0.4, 0.3+armLength, 0);
	v3 b(-0.4, -0.2+armLength, 0);
	v3 c(0.6, 0.3+armLength, 0);
	v3 d(0, armLength, 0);
	v3 e(0.6, armLength, 0);
	drawPrism(a, b, c, 1.0);
	drawPrism(d, e, c, 1.0);
	setDiffuseColor(1.0f, 1.0f, 1.0f);
	drawBox(e[0]-d[0], armLength, 1.0);
}

void Gundan::drawHip()
{
	glTranslated(0, -0.5, -0.5);
	setDiffuseColor(1.0f, 1.0f, 1.0f);
	v3 a(1.2, -1.1, -0.5);
	v3 b(-1.2, -1.1, -0.5);
	v3 c(0, -1.6, -0.5);
	drawPrism(a, b, c, 1.0);
}

void Gundan::drawSword()
{
	glPushMatrix();
	glRotated(90, 0.0, 1.0, 0.0);
	glRotated(120, 0.0, 0.0, 1.0);
	glScaled(3.0, 2.0, 1.0);
	glTranslated(-0.05, -0.2, 0);
	glPushMatrix();
	{
		glTranslated(0.05, 0, 0);
		glRotated(90, -1.0, 0.0, 0.0);
		setDiffuseColor(0.7f, 0.7f, 0.7f);
		drawCylinder(0.5, 0.05, 0.05);
	}
	glPopMatrix();

	{//left handguard
		v3 a1(0, 0.2, -0.05);
		v3 b1(0, 1.0, -0.05);
		v3 c1(-0.1, 0.8, -0.05);
		v3 d1(-0.1, 0.4, -0.05);
		v3 a2(0, 0.2, 0.05);
		v3 b2(0, 1.0, 0.05);
		v3 c2(-0.1, 0.8, 0.05);
		v3 d2(-0.1, 0.4, 0.05);
		setDiffuseColor(0.7f, 0.7f, 0.7f);
		drawQuadruple(a1, b1, c1, d1, a2, b2, c2, d2);
	}
	{//right handguard
		v3 a1(0.2, 0.4, -0.05);
		v3 b1(0.2, 0.8, -0.05);
		v3 c1(0.1, 1.0, -0.05);
		v3 d1(0.1, 0.2, -0.05);
		v3 a2(0.2, 0.4, 0.05);
		v3 b2(0.2, 0.8, 0.05);
		v3 c2(0.1, 1.0, 0.05);
		v3 d2(0.1, 0.2, 0.05);
		setDiffuseColor(0.7f, 0.7f, 0.7f);
		drawQuadruple(a1, b1, c1, d1, a2, b2, c2, d2);
	}
	double slen = 2.5;	
	glTranslated(0.0, 0.5, -0.005);
	setDiffuseColor(COLOR_GREEN);
	drawBox(0.1, slen, 0.01);
	v3 a(0.0, slen, 0.0);
	v3 b(0.1, slen, 0.0);
	v3 c(0.04, slen+0.3, 0.0);
	drawPrism(a, b, c, 0.01);

	glPopMatrix();
}

void Gundan::draw()
{
    ModelerView::draw();
	setAmbientColor(.1f,.1f,.1f);
	int level = VAL(LEVEL);
	//level 1
	glPushMatrix();
	drawBody();
	if(level>=2) {
		glPushMatrix();
		drawHead();
		glPopMatrix();
		
		glPushMatrix();
		drawLefthand();
		glPopMatrix();

		glPushMatrix();
		drawRighthand();
		glPopMatrix();

		glPushMatrix();
		drawHip();
		if(level>=3) {
			glPushMatrix();
			drawLeftThign();
			if(level>=4) {
				glPushMatrix();
				drawLeftShank();
				glPopMatrix();
			}
			glPopMatrix();

			glPushMatrix();
			drawRightThign();
			if(level>=4) {
				glPushMatrix();
				drawRightShank();
				glPopMatrix();
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

int main()
{
    ModelerControl controls[NUMCONTROLS];
    controls[LEVEL] = ModelerControl("level", 1, 4, 1, 4);
    controls[LHANDX] = ModelerControl("left hand x", 0, 70, 1, 10);
    controls[RHANDX] = ModelerControl("right hand x", 0, 70, 1, 10);
    controls[LHANDZ] = ModelerControl("left hand z", -80, 80, 1, 0);
    controls[RHANDZ] = ModelerControl("right hand z", -80, 80, 1, 0);
    controls[LKNEEL] = ModelerControl("left kneel", 0, 70, 1, 0);
    controls[RKNEEL] = ModelerControl("right kneel", 0, 70, 1, 0);
    controls[LLEGX] = ModelerControl("left leg x", 0, 60, 1, 5);
    controls[RLEGX] = ModelerControl("right leg x", 0, 60, 1, 5);
    controls[LLEGZ] = ModelerControl("left leg z", -80, 80, 1, 0);
    controls[RLEGZ] = ModelerControl("right leg z", -80, 80, 1, 0);
    controls[LSHANKZ] = ModelerControl("left shank z", 0, 120, 1, 0);
    controls[RSHANKZ] = ModelerControl("right shank z", 0, 120, 1, 0);
	controls[SWORD] = ModelerControl("sword", 0, 1, 1, 1);
    ModelerApplication::Instance()->Init(&createGundan, controls, NUMCONTROLS);
    return ModelerApplication::Instance()->Run();
}
