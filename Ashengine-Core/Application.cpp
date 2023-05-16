#include "Application.hpp"
#include "Time.hpp"
#include "Input.hpp"

void Application::Start()
{
	// Call OnCreate so that the application has a chance to start up.
	OnCreate();

	// Store the time at the end of each loop
	TimePoint StartTime = Time::Now();

	// Store somewhere the deltatime of each frame for use when updating things.
	double DeltaTime = TargetDeltaTime;

	// Run a game loop.
	while ( !ShouldExit )
	{
		// Tick things that need to happen each frame, like input.
		Input::Tick();

		// Run the update function
		OnUpdate( DeltaTime );
		
		// Let's figure out how long it took for this whole loop to run
		TimePoint EndTime = Time::Now();
		TimeSpan LoopDuration = EndTime - StartTime;
		StartTime = EndTime;

		// Let's now sleep for however long we need to to pad out the time to 30fps.
		// 30 fps means that each loop should take 33.333... milliseconds.
		// 30 frames per seconds means 1/30 seconds per frame = 33.333 milliseconds
		double Milliseconds = ( double )LoopDuration.Milliseconds();
		DeltaTime = Milliseconds / 1000;

		// Lets pad out the deltatime if it needs to take longer to hit the right fps.
		// Note that if it took longer than the target delta time, there is nothing we can do.
		if ( Milliseconds < TargetDeltaTime )
		{
			// Time left to pad it out to 30fps
			double PadTime = ( double )TargetDeltaTime - Milliseconds;

			// Let's sleep for that long to make the whole loop take 33.333 milliseconds
			Sleep( PadTime );
		}
	}
}

void Application::Exit()
{
	ShouldExit = true;
}

void Application::SetTargetFPS( float a_TargetFPS )
{
	TargetDeltaTime = 1.0f / a_TargetFPS;
}