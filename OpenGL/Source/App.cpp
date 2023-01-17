#include "pch.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// test scenes
#include "Tests/CubeTest.h"
#include "Tests/PhongTest.h"
#include "Tests/LightingMapTest.h"
#include "Tests/ModelTest.h"
#include "Tests/StencilTest.h"

int main()
{
	Sandbox* scene = new ModelTest();

	scene->Run();

	delete scene;


	return 0;
}