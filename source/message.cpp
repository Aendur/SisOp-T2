#include "message.h"
#include <cstring>

Message::Message(void) : type(0), text("") {}
Message::Message(long t, const char * txt) {
	this->type = t;
	strncpy(this->text, txt, GM_MSG_SIZE - 1);
	this->text[GM_MSG_SIZE - 1] = 0;
}
