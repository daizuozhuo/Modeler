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

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out Gundan
void Gundan::draw()
{
    ModelerView::draw();
	setAmbientColor(.1f,.1f,.1f);
	//draw the head
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

	//draw the body
	glPushMatrix();
		glTranslated(-1, -1, -0.5);
		setDiffuseColor(COLOR_BLUE);
		drawBox(2, 2.5, 1);
	glPopMatrix();

	double armLength = 4.0;
	//draw the right hand
	glPushMatrix();
	{
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
	}
	glPopMatrix();

	//draw the left hand
	glPushMatrix(); 
	{
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
	}
	glPopMatrix();

	glPushMatrix();
	{
		setDiffuseColor(1.0f, 1.0f, 1.0f);
		v3 a(1.2, -1.1, -0.5);
		v3 b(-1.2, -1.1, -0.5);
		v3 c(0, -1.6, -0.5);
		drawPrism(a, b, c, 1.0);
	}
	glPopMatrix();

	//draw the right leg
	glPushMatrix();
	{
		glTranslated(0.1, -1.4, 0);
		glRotated(90, 0.0, 1.0, 0.0);
		setDiffuseColor(0.6f, 0.6f, 0.6f);
		drawCylinder(1.2, 0.3, 0.3);
	}
	glPopMatrix();
}

int main()
{
    ModelerControl controls[NUMCONTROLS];
    controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0);
    controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
    controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0);
    controls[HEIGHT] = ModelerControl("Height", 1, 2.5, 0.1f, 1);
	controls[ROTATE] = ModelerControl("Rotate", -135, 135, 1, 0);

    ModelerApplication::Instance()->Init(&createGundan, controls, NUMCONTROLS);
    return ModelerApplication::Instance()->Run();
}
