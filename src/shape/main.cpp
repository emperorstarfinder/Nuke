// Shape tests
#include "shape.hpp"
#include <GL/gl.h>
#include <iostream>

using namespace nuke::shape;

int main()
{
	// Create hash table
	unordered_map<Shape, GLuint, ShapeHash> shapes;

	// Make some shapes
	Shape cube(DrawableShape::CUBE, vec3(1.0f, 1.0f, 1.0f));
	Shape none(DrawableShape::NONE, vec3(2.0f, 1.0f, 1.0f));
	Shape cube2(DrawableShape::CUBE, vec3(1.0f, 2.0f, 3.0f));

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

