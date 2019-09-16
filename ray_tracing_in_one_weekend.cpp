// ray_tracing_in_one_weekend.cpp : Defines the entry point for the application.
//

#include "ray_tracing_in_one_weekend.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "random.hpp"

#include <iostream>
#include <fstream>
#include <limits>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define EPS 0.001f

using namespace std;

vec3 random_in_unit_sphere()
{
	vec3 p;
	do {
		p = 2.0f * vec3(random_float(), random_float(), random_float()) - vec3(1, 1, 1);
	} while (p.square_length() >= 1.0);
	return p;
}

vec3 color(const ray& r, hittable* world)
{
	hit_record rec;
	if (world->hit(r, EPS, std::numeric_limits<float>::max(), rec))
	{
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		return 0.5f* color(ray(rec.p, target - rec.p), world);
	}
	vec3 unit_direction = unit_vector(r.direction());
	float t = 0.5f * (unit_direction.y() + 1);
	return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
}


int main() {
	int nx = 800;
	int ny = 400;
	int ns = 100;
	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);
	unsigned char* imageBuffer = new unsigned char[nx * ny * 4]();

	hittable* list[2];
	list[0] = new sphere(vec3(0, 0, -1), 0.5f);
	list[1] = new sphere(vec3(0, -100.5, -1), 100);
	hittable* world = new hittable_list(list, 2);
	camera cam;

	for (int x = 0; x < nx; x++)
	{
		for (int y = 0; y < ny; y++)
		{
			vec3 col(0,0,0);
			for (int s = 0; s < ns; s++)
			{
				float u = float(x + random_float()) / float(nx);
				float v = float(y + random_float()) / float(ny);
				ray r = cam.get_ray(u, v);
				col += color(r, world);
			}
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

			unsigned char ir = unsigned char(255.99 * col[0]);
			unsigned char ig = unsigned char(255.99 * col[1]);
			unsigned char ib = unsigned char(255.99 * col[2]);
			imageBuffer[x * 4 + (ny - 1 - y) * 4 * nx + 0] = ir;
			imageBuffer[x * 4 + (ny - 1 - y) * 4 * nx + 1] = ig;
			imageBuffer[x * 4 + (ny - 1 - y) * 4 * nx + 2] = ib;
			imageBuffer[x * 4 + (ny - 1 - y) * 4 * nx + 3] = 255;
		}
		std::cout << "x = " << x << "out of : " << nx << std::endl;
	}

	stbi_write_png("output.png", nx, ny, 4, imageBuffer, nx * 4);
	delete[] imageBuffer;
	delete list[0];
	delete list[1];
	delete world;

	return 0;
}
