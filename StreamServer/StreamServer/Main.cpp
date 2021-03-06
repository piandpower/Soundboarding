#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Server.h"
#include "include\getopt.h"
#include <iostream>
#include "ThreadSafeQueue.h"
#include "SpotifyProvider.h"

int main(int argc, char** argv)
{
	const char *username = NULL;
	const char *password = NULL;
	const char* listname = NULL;
	int opt;

	while ((opt = getopt(argc, argv, "u:p:l:d")) != EOF) {
		switch (opt) {
		case 'u':
			username = optarg;
			break;

		case 'p':
			password = optarg;
			break;

		case 'l':
			listname = optarg;
			break;

		default:
			exit(1);
		}
	}

	if (!username || !password || !listname) {
		std::cout << "username passsowrd list name missing or something";
		exit(1);
	}

	std::string pipename = "\\\\.\\pipe\\mynamedpipe";

	ThreadSafeQueue<AudioChunk> queue;
	
	SpotifyProvider spotify(username, password, listname, &queue);
	spotify.StartThread();

	Server server(3666, &queue);

	server.Run();

	return 0;
}