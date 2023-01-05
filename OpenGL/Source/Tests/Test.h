#pragma once


class Test
{
public:
	Test() {}
	virtual ~Test() {}

	virtual void OnUpdate() {}
	virtual void OnRender() {}
	virtual void OnImGuiRender() {}
};