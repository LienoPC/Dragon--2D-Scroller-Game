#include "timerMy.h"

double Timer::start_time;
int Timer::FPS;
double Timer::actual_time;
double Timer::coolDownStart;
double Timer::coolDownActual;

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

double Timer::getCoolDown() {
	double elapsed;
	coolDownActual = glfwGetTime();
	elapsed = coolDownActual - coolDownStart;
	return elapsed;
}

void Timer::setCoolDown() {
	coolDownStart = glfwGetTime();
}

void Timer::forceChrono(double startTime) {
	Timer::start_time = startTime;
}

