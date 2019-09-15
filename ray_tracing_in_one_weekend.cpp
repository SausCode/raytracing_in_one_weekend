// ray_tracing_in_one_weekend.cpp : Defines the entry point for the application.
//

#include "ray_tracing_in_one_weekend.h"
#include "vec3.h"

#include <iostream>
#include <fstream>

using namespace std;

int main()
{

	ofstream imageFile;
	imageFile.open("test.ppm", ios::out | ios::trunc);

	int nx = 200;
	int ny = 100;
	imageFile << "P3\n" << nx << " " << ny << "\n255\n";
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(float(i) / float(nx), float(j) / float(ny), 0.2f);

			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			imageFile << ir << " " << ig << " " << ib << "\n";
		}
	}

	imageFile.close();

	return 0;
}
