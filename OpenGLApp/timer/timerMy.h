#ifndef TIMER_H
#define GAME_H

#include "../glfw-3.3.8.bin.WIN64/include/GLFW/glfw3.h"

class Timer {

	public:
		static double start_time;
		static int FPS;
		static double actual_time;



		Timer();


		static double getElapsedSeconds();
		// ritorna il tempo passato in secondi dallo start_time al frame attuale
		static int getElapsedMilliS();

		static void setChrono();
		static void resetChrono();
		static void forceChrono(double startTime);
};













#endif // TIMER
