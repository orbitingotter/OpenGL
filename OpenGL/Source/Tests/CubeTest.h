#pragma once

#include "Test.h"

class CubeTest : public Test
{
public:
	CubeTest();
	~CubeTest();

	void OnUpdate() override;
	void OnRender() override;
	void OnImGuiRender() override;

private:
};