#include "pch.h"
#include "Time.h"
#include <GLFW/glfw3.h>


Time::Time()
{
	current_frame_time = 0.0f;
	last_frame_time = 0.0f;
	DeltaTime = 0.0f;
}


Time::~Time()
{
}

float Time::getDeltaTime()
{
	current_frame_time = glfwGetTime();
	DeltaTime = current_frame_time - last_frame_time;
	last_frame_time = last_frame_time + DeltaTime;		//Update last_frame_time every iteration of main loop
	return DeltaTime;
}
