// Shape tests
#include "Shape.hpp"
#include <GL/gl.h>
#include <iostream>

using namespace gfx::shape;

int main()
{
	// Create hash table
	unordered_map<gfxShape, GLuint, gfxShapeHash> shapes;

	// Make some shapes
	gfxShape cube(DrawableShapes::CUBE, vec3(1.0f, 1.0f, 1.0f));
	gfxShape none(DrawableShapes::NONE, vec3(2.0f, 1.0f, 1.0f));
	gfxShape cube2(DrawableShapes::CUBE, vec3(1.0f, 2.0f, 3.0f));

	shapes.emplace(cube, (GLuint)0);
	shapes.emplace(none, (GLuint)1);

	if (shapes.find(cube) == shapes.end()) {
		cout << "Not found in map" << endl;
	}
	else if (shapes.find(cube) != shapes.end()) {
		shapes.emplace(cube2, GLuint(2));
		cout << "GLuint : " << &shapes[cube2] << endl;
	}
}

