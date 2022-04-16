#include "UI/UIEmpty.h"

#include <thread>
#include <chrono>

void UIEmpty::Refresh(int delay) {
	if (delay > 0) {
		std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(delay));
	}
}
