#pragma once

class Application
{
public:

	Application() = default;

	void Start();
	void Exit();
	void SetTargetFPS( float a_TargetFPS );

protected:

	virtual void OnCreate() = 0;
	virtual void OnUpdate( float a_DeltaTime ) = 0;

private:

	bool ShouldExit = false;
	float TargetDeltaTime = 1.0f / 30;
};