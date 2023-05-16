#pragma once

class Application
{
public:

	void Start();

protected:

	virtual void OnCreate();
	virtual void OnUpdate(float a_DeltaTime);
};