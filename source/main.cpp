#if defined(SERVER)
#include "server.h"

int main (int, char **) {
	Server server("settings_server.ini");
	server.Run();
	printf("main return 0\n");
	return 0;
}
#elif defined(CLIENT)
#include "client.h"

int main (int, char **) {
	Client client("settings_client.ini");
	client.Connect();
	client.Run();
	printf("main return 0\n");
	return 0;
}
#endif

