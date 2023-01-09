#include "pch.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// test scenes
#include "Tests/CubeTest.h"
#include "Tests/PhongTest.h"
#include "Tests/LightingMapTest.h"

int main()
{
	Sandbox* scene = new LightingMapTest();

	scene->Run();

	delete scene;


	return 0;
}