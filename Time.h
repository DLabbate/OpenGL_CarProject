#pragma once
class Time
{
public:
	Time();
	~Time();
	float getDeltaTime();

private:
	float DeltaTime = 0.0f;
	float current_frame_time = 0.0f;
	float last_frame_time = 0.0f;
};

