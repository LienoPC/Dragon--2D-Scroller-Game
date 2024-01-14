#ifndef TIMER_H
#define GAME_H

#include <__msvc_chrono.hpp>

class Timer {

	public:
		static std::chrono::time_point<std::chrono::system_clock, std::chrono::system_clock::duration> start_time;
		static int FPS;
		static std::chrono::time_point<std::chrono::system_clock> actual_time;



		Timer();


		static double getElapsedSeconds() {
			std::chrono::duration<double> elapsed(0);
			elapsed = actual_time - start_time;
			return elapsed.count();

		} // ritorna il tempo passato in secondi dallo start_time al frame attuale
		static int getElapsedMilliS();

		static void setChrono(){
			start_time = std::chrono::system_clock::now();
		}
		static void resetChrono();
};













#endif // TIMER
