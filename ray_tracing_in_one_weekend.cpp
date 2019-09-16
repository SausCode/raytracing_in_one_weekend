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
#include "material.hpp"

#include <iostream>
#include <fstream>
#include <limits>
#include <atomic>
#include <thread>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define EPS 0.001f
#define OUTPUT_FILE_NAME "output.png"
#define WIDTH 800
#define HEIGHT 400
#define NS 10

using namespace std;

vec3 color(const ray& r, hittable* world, int depth) {
	hit_record rec;
	if (world->hit(r, EPS, std::numeric_limits<float>::max(), rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return vec3(0, 0, 0);
		}
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

void draw_scene()
{
	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);
	unsigned char* imageBuffer = new unsigned char[WIDTH * HEIGHT * 4]();

	hittable * list[4];
	list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
	list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0));
	list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
	hittable * world = new hittable_list(list, 4);
	camera cam;

	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < NS; s++)
			{
				float u = float(x + random_float()) / float(WIDTH);
				float v = float(y + random_float()) / float(HEIGHT);
				ray r = cam.get_ray(u, v);
				col += color(r, world, 0);
			}
			col /= float(NS);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

			unsigned char ir = unsigned char(255.99 * col[0]);
			unsigned char ig = unsigned char(255.99 * col[1]);
			unsigned char ib = unsigned char(255.99 * col[2]);
			imageBuffer[x * 4 + (HEIGHT - 1 - y) * 4 * WIDTH + 0] = ir;
			imageBuffer[x * 4 + (HEIGHT - 1 - y) * 4 * WIDTH + 1] = ig;
			imageBuffer[x * 4 + (HEIGHT - 1 - y) * 4 * WIDTH + 2] = ib;
			imageBuffer[x * 4 + (HEIGHT - 1 - y) * 4 * WIDTH + 3] = 255;
		}
		std::cout << "x = " << x << "out of : " << WIDTH << std::endl;

	}

	stbi_write_png(OUTPUT_FILE_NAME, WIDTH, HEIGHT, 4, imageBuffer, WIDTH * 4);
	delete[] imageBuffer;
	delete list[0];
	delete list[1];
	delete list[2];
	delete list[3];
	delete world;
}


int main() {
	draw_scene();
	return 0;
}
