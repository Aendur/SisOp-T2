#ifndef MESSAGE_H
#define MESSAGE_H

#define GM_CONNECTION_REQ 1
#define GM_CONNECTION_ACK 2
#define GM_CMD_STOP "STOP"

struct Message {
	long type;     /* message type, must be > 0 */
	char text[16]; /* message data */

	Message(void);
	Message(long t, const char * txt);
	int Size(void) const { return sizeof(text); }
};

#endif
