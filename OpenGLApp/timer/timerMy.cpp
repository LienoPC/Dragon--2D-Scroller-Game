#include "timerMy.h"

double Timer::start_time;
int Timer::FPS;
double Timer::actual_time;

Timer::Timer() {

}


void Timer::setChrono() {

	start_time = glfwGetTime();
}

double Timer::getElapsedSeconds() {
		double elapsed;
		actual_time = glfwGetTime();
		elapsed = actual_time - start_time;
		return elapsed;
}

void Timer::forceChrono(double startTime) {
	Timer::start_time = startTime;
}

