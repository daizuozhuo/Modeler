#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "vec.h"
#include <FL/gl.h>

#include "modelerglobals.h"
typedef Vec3<double> v3;

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
	void drawRightleg();
	void drawLeftleg();
	void drawSword();
};


v3 crossProduct(v3 a, v3 b) {
	return v3(a[1]*b[2] - a[2]*b[1], a[2]*b[0] - a[0]*b[2], a[0]*b[1] - a[1]*b[0]);
}

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

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createGundan(int x, int y, int w, int h, char *label)
{ 
    return new Gundan(x,y,w,h,label); 
}

void Gundan::drawRightleg()
{
	double kneel = VAL(RKNEEL);
	//draw the right leg
	glPushMatrix();
	//draw upper joint
	glTranslated(0.1, -1.4, 0);
	glRotated(kneel, -1.0, 0.0, 0.0); 
	glRotated(VAL(RLEGX),0.0, 0.0, 1.0); 
	glPushMatrix();
	{
		glRotated(90, 0.0, 1.0, 0.0);
		setDiffuseColor(0.6f, 0.6f, 0.6f);
		drawCylinder(1.2, 0.3, 0.3);
	}
	glPopMatrix();

	//draw upper leg
	double ulen=-1.5;
	{
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
	}

	//draw middle joint
	ulen += -0.1;
	glTranslated(0.3, ulen, 0);
	glRotated(2 * kneel, 1.0, 0.0, 0.0); 
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

	glPopMatrix();
}

void Gundan::drawLeftleg()
{
	double kneel = VAL(LKNEEL);
	glPushMatrix();
	//draw upper joint
	glTranslated(-0.1, -1.4, 0);
	glRotated(kneel, -1.0, 0.0, 0.0); 
	glRotated(VAL(LLEGX),0.0, 0.0, -1.0); 
	glPushMatrix();
	{
		glRotated(-90, 0.0, 1.0, 0.0);
		setDiffuseColor(0.6f, 0.6f, 0.6f);
		drawCylinder(1.2, 0.3, 0.3);
	}
	glPopMatrix();

	//draw upper leg
	double ulen=-1.5;
	{
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
	}

	//draw middle joint
	ulen += -0.1;
	glTranslated(-0.3, ulen, 0);
	glRotated(2 * kneel, 1.0, 0.0, 0.0); 
	glPushMatrix();
	{
		glRotated(-90, 0.0, 1.0, 0.0);
		setDiffuseColor(0.6f, 0.6f, 0.6f);
		drawCylinder(0.6, 0.3, 0.3);
	}
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

	glPopMatrix();
}

void Gundan::drawBody()
{
	glPushMatrix();
	//draw GN driver
	glTranslated(0, 0, -0.5-0.8);
	setDiffuseColor(1.0f, 1.0f, 1.0f);
	drawCylinder(0.8, 0.01, 0.4);
	//draw Body
	glTranslated(-1, -1, 0.8);
	setDiffuseColor(COLOR_BLUE);
	glScaled(2, 2.5, 1); 
	drawBox(1, 1, 1);
	glPopMatrix();
}

void Gundan::drawHead()
{	
	glPushMatrix();
	glTranslated(-0.5, 2, -0.5);
	setDiffuseColor(1.0f, 1.0f, 1.0f);
	drawBox(1, 1, 1);
	glTranslated(0.3, 0.6, 1);
	setDiffuseColor(COLOR_RED);
	drawBox(0.4, 0.8, 0.1);
	setDiffuseColor(1.0f, 1.0f, 0);
	drawPrism(0, 0, 0, 0, 0.4, 0, -1.6, 1.6, 0, 0.1);
	drawPrism(0.4, 0, 0, 2.0, 1.6, 0, 0.4, 0.4, 0, 0.1);
	glPopMatrix();
}

void Gundan::drawLefthand()
{
	double armLength = 4.0;
	glPushMatrix(); 
	setDiffuseColor(COLOR_BLUE);
	v3 a(-1.1, 1.5, -0.5);
	v3 b(-1.1, 1.0, -0.5);
	v3 c(-2.1, 1.5, -0.5);
	v3 d(-1.5, 1.2, -0.5);
	v3 e(-2.1, 1.2, -0.5);
	drawPrism(a, c, b, 1.0);
	drawPrism(d, c, e, 1.0);
	glTranslated(d[0], d[1]-armLength, -0.5); 
	setDiffuseColor(1.0f, 1.0f, 1.0f);
	drawBox(e[0]-d[0], armLength, 1.0);
	if(VAL(SWORD)) {
		glTranslated((e[0]-d[0])/2, 0.0, 1.0);
		drawSword();
	}
	glPopMatrix();

}

void Gundan::drawRighthand()
{
	double armLength = 4.0;
	glPushMatrix();
	setDiffuseColor(COLOR_BLUE);
	v3 a(1.1, 1.5, -0.5);
	v3 b(1.1, 1.0, -0.5);
	v3 c(2.1, 1.5, -0.5);
	v3 d(1.5, 1.2, -0.5);
	v3 e(2.1, 1.2, -0.5);
	drawPrism(a, b, c, 1.0);
	drawPrism(d, e, c, 1.0);
	glTranslated(d[0], d[1]-armLength, -0.5); 
	setDiffuseColor(1.0f, 1.0f, 1.0f);
	drawBox(e[0]-d[0], armLength, 1.0);
	glPopMatrix();
}

void Gundan::drawHip()
{
	glPushMatrix();
	setDiffuseColor(1.0f, 1.0f, 1.0f);
	v3 a(1.2, -1.1, -0.5);
	v3 b(-1.2, -1.1, -0.5);
	v3 c(0, -1.6, -0.5);
	drawPrism(a, b, c, 1.0);
	glPopMatrix();
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

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out Gundan
void Gundan::draw()
{
    ModelerView::draw();
	setAmbientColor(.1f,.1f,.1f);
	int level = VAL(LEVEL);
	if(level >= 1) drawHead();
	if(level >= 2) drawBody();
    if(level >= 3){
		drawRighthand();
		drawLefthand();
		drawHip();
	}
	if(level >=4) {	
		drawRightleg();
		drawLeftleg();
	}
}

int main()
{
    ModelerControl controls[NUMCONTROLS];
    controls[LEVEL] = ModelerControl("level", 1, 4, 1, 4);
    controls[LKNEEL] = ModelerControl("left kneel", 0, 70, 1, 0);
    controls[RKNEEL] = ModelerControl("right kneel", 0, 70, 1, 0);
    controls[LLEGX] = ModelerControl("left leg x", 0, 60, 1, 5);
    controls[RLEGX] = ModelerControl("right leg x", 0, 60, 1, 5);
	controls[SWORD] = ModelerControl("sword", 0, 1, 1, 1);
    ModelerApplication::Instance()->Init(&createGundan, controls, NUMCONTROLS);
    return ModelerApplication::Instance()->Run();
}
