#include <iostream>
//#include <ncurses.h>


//int main() {
//	initscr();
//	printw("hi");
//	refresh();
//	getch();
//	endwin();
//	return 0;
//}

#include <GLFW/glfw3.h>

int main(void) {
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
	return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(800, 800, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glViewport(0, 0, 800, 800);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f,
	};

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


/*

#include "Game.h"

#include <ctime>
#include <filesystem>
#include <iostream>

#ifndef UNITTEST
int main (int, char ** argv) {
	auto wdir = (std::filesystem::current_path()  / argv[0]).parent_path();
	std::cout << "working directory is " << wdir << std::endl;
	std::filesystem::current_path(wdir);
	
	std::srand(std::time(NULL));
	Game game = Game::GetInstance();
	game.Run();

	return 0;
}
#endif


*/