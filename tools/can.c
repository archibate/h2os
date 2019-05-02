#include "svpng.inc"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define TWO_PI 6.28318530718f

typedef struct { float sd, emi, refl; } Result;


Result uni(Result a, Result b)
{
	return a.sd < b.sd ? a : b;
}

Result inter(Result a, Result b)
{
	Result r = a.sd > b.sd ? b : a;
	r.sd = a.sd > b.sd ? a.sd : b.sd;
	return r;
}

Result subtract(Result a, Result b)
{
	Result r = a;
	r.sd = (a.sd > -b.sd) ? a.sd : -b.sd;
	return r;
}

Result complement(Result a)
{
	a.sd = -a.sd;
	return a;
}


float circle(float x, float y, float cx, float cy, float r)
{
	float ux = x - cx, uy = y - cy;
	return sqrtf(ux * ux + uy * uy) - r;
}

float plane(float x, float y, float px, float py, float nx, float ny)
{
	return (x - px) * nx + (y - py) * ny;
}

float segment(float x, float y, float ax, float ay, float bx, float by)
{
	float vx = x - ax, vy = y - ay, ux = bx - ax, uy = by - ay;
	float t = fmaxf(fminf((vx * ux + vy * uy) / (ux * ux + uy * uy), 1.0f), 0.0f);
	float dx = vx - ux * t, dy = vy - uy * t;
	return sqrtf(dx * dx + dy * dy);
}

float capsule(float x, float y, float ax, float ay, float bx, float by, float r)
{
	return segment(x, y, ax, ay, bx, by) - r;
}

float box(float x, float y, float cx, float cy, float theta, float sx, float sy)
{
	float costheta = cosf(theta), sintheta = sinf(theta);
	float dx = fabs((x - cx) * costheta + (y - cy) * sintheta) - sx;
	float dy = fabs((y - cy) * costheta - (x - cx) * sintheta) - sy;
	float ax = fmaxf(dx, 0.0f), ay = fmaxf(dy, 0.0f);
	return fminf(fmaxf(dx, dy), 0.0f) + sqrtf(ax * ax + ay * ay);
}

float triangle(float x, float y, float ax, float ay, float bx, float by, float cx, float cy)
{
	float d = fminf(fminf(
				segment(x, y, ax, ay, bx, by),
				segment(x, y, bx, by, cx, cy)),
			segment(x, y, cx, cy, ax, ay));
	return (bx - ax) * (y - ay) > (by - ay) * (x - ax) && 
		(cx - bx) * (y - by) > (cy - by) * (x - bx) && 
		(ax - cx) * (y - cy) > (ay - cy) * (x - cx) ? -d : d;
}


#if 1
Result scene(float x, float y)
{
	Result a = {  circle(x, y, 0.4f,  0.2f, 0.1f), 2.0f, 0.0f };
	Result b = {     box(x, y, 0.5f,  0.8f, TWO_PI / 16.0f, 0.1f, 0.1f), 0.0f, 0.9f };
	Result c = {     box(x, y, 0.8f,  0.5f, TWO_PI / 16.0f, 0.1f, 0.1f), 0.0f, 0.9f };
	return uni(uni(a, b), c);
}
#else
Result scene(float x, float y)
{
	x = fabsf(x - 0.5f) + 0.5f;
	Result a = { capsule(x, y, 0.75f, 0.25f, 0.75f, 0.75f, 0.05f), 1.0f };
	Result b = { capsule(x, y, 0.75f, 0.25f, 0.50f, 0.75f, 0.05f), 1.0f };
	return a.sd < b.sd ? a : b;
}
#endif


void reflect(float ix, float iy, float nx, float ny, float* rx, float* ry)
{
	float idotn2 = (ix * nx + iy * ny) * 2.0f;
	*rx = ix - idotn2 * nx;
	*ry = iy - idotn2 * ny;
}


#define MAX_STEP 64
#define MAX_DEPTH 3
#define MAX_DISTANCE 2.0f
#define EPSILON 1e-6f
#define BIAS 1e-4f

void gradient(float x, float y, float* nx, float* ny) {
	*nx = (scene(x + EPSILON, y).sd - scene(x - EPSILON, y).sd) * (0.5f / EPSILON);
	*ny = (scene(x, y + EPSILON).sd - scene(x, y - EPSILON).sd) * (0.5f / EPSILON);
}

float trace(float ox, float oy, float dx, float dy, int depth)
{
	float t = 0.0f;
	for (int i = 0; i < MAX_STEP && t < MAX_DISTANCE; i++) {
		float x = ox + dx * t, y = oy + dy * t;
		Result r = scene(x, y);
		if (r.sd < EPSILON) {
			float sum = r.emi;
			if (depth < MAX_DEPTH && r.refl > 0.0f) {
				float nx, ny, rx, ry;
				gradient(x, y, &nx, &ny);
				reflect(dx, dy, nx, ny, &rx, &ry);
				sum += r.refl * trace(x + nx * BIAS, y + ny * BIAS, rx, ry, depth + 1);
			}
			return sum;
		}
		t += r.sd;
	}
	return 0.0f;
}


#define N 64

float sample(float x, float y)
{
	float sum = 0.0f;
	for (int i = 0; i < N; i++) {
		float a = TWO_PI * (i + (float)rand() / RAND_MAX) / N;
		sum += trace(x, y, cosf(a), sinf(a), 0);
	}
	return sum / N;
}


#define W 400
#define H 400

unsigned char img[W * H * 3];

int main(void)
{
	unsigned char *p = img;
	for (int y = 0; y < H; y++) {
		fprintf(stderr, "Rendering %3d%%\r", (y*100)/H);
		for (int x = 0; x < W; x++, p += 3) {
			p[0] = p[1] = p[2] = (int)(fminf(
						sample((float)x / W, (float)y / H)
						* 255.0f, 255.0f));
		}
	}
	svpng(fopen("basic.png", "wb"), W, H, img, 0);
}
