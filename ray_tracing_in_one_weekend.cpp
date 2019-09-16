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
#define WIDTH 1200
#define HEIGHT 800
#define NS 10

using namespace std;

hittable* random_scene() {
	int n = 500;
	hittable** list = new hittable * [n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = random_double();
			vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.8) {  // diffuse
					list[i++] = new sphere(center, 0.2,
						new lambertian(vec3(random_double() * random_double(),
							random_double() * random_double(),
							random_double() * random_double())
						)
					);
				}
				else if (choose_mat < 0.95) { // metal
					list[i++] = new sphere(center, 0.2,
						new metal(vec3(0.5 * (1 + random_double()),
							0.5 * (1 + random_double()),
							0.5 * (1 + random_double())),
							0.5 * random_double()));
				}
				else {  // glass
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

	return new hittable_list(list, i);
}

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

	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.1;

	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(WIDTH) / float(HEIGHT), aperture, dist_to_focus);

	hittable* world = random_scene();

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
	delete world;
}


int main() {
	draw_scene();
	return 0;
}
