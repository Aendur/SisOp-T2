#include "message.h"
#include <cstdio>

Message::Message(void) : type(0), text("") {}

Message::Message(long t, const char * txt) {
	this->type = t;
	snprintf(this->text, Size(), txt);
}


