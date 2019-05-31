#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

struct vertex {
	float pos, vel;
	float radius;
	vector<object*> links;
};

bool collides(vertex const&a, vertex const&b)
{
	return fabs(a.pos - b.pos) <= a.radius + b.radius;
}

void do_collide(vertex const&a, vertex const&b)
{
}

vector<vertex> os;

int main()
{
	for (int i = 0; i < os.size(); i++) {
		for (int j = i + 1; j < os.size(); j++) {
			if (collides(os[i], os[j]))
				do_collision(os[i], os[j]);
		}
	}
}
