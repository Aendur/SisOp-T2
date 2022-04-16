#if defined(SERVER)
#include "server.h"

int main (int argc, char ** argv) {
	char path[256];
	if (argc > 1) { snprintf(path, 256, argv[1]); }
	else { snprintf(path, 256, "settings_server.ini"); }

	Server server(path);
	server.Run();
	printf("main return 0\n");
	return 0;
}
#elif defined(CLIENT)
#include "client.h"

int main (int argc, char ** argv) {
	char path[256];
	if (argc > 1) { snprintf(path, 256, argv[1]); }
	else { snprintf(path, 256, "settings_client.ini"); }
	
	Client client(path);
	client.Connect();
	client.Run();
	printf("main return 0\n");
	return 0;
}
#endif

