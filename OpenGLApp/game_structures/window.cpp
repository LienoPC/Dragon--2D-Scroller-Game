#include "window.h"

ThrowWindow::ThrowWindow() {


}

ThrowWindow::ThrowWindow(glm::vec2 Position, glm::vec2 offsetInterval, int identificator, DirectionStart directionStart) {

	this->Position = Position;
	this->offsetInterval = offsetInterval;
	this->identificator = identificator;
	this->directionStart = directionStart;

}