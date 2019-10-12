#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string.h>
#include <sstream>

#include "terminal.h"
#include "resources.h"

const GLint WIDTH = 1920, HEIGHT = 1080, DPP = 6;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void charCallback(GLFWwindow* window, unsigned int key);
void _stdcall glVertex2fToScreen(float x, float y);
void pixel(int x, int y);

bool** pixelMatrix = new bool*[0];

terminal *term;
//void (*termKeyHandler)(int, int, int, int, int);

int main()
{
	//initPixelMatrix(pixelMatrix);
	term = new terminal(pixelMatrix, WIDTH / DPP, HEIGHT / DPP, HEIGHT);
	//termKeyHandler = term->keyhandler;

	glfwInit();

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	GLFWwindow* window;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	glfwWindowHint(GLFW_DECORATED, GL_FALSE);

	//window = glfwCreateWindow(mode->width, mode->height, "LUL", NULL, NULL);
	window = glfwCreateWindow(WIDTH, HEIGHT, "LUL", NULL, NULL);
	if (!window)
    {
        glfwTerminate();
        return -1;
    }
	else
	{
		//Post-creatrion init
		glfwSetKeyCallback(window, keyCallback);
		glfwSetCharCallback(window, charCallback);
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	int frame = 0;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glColor3f(1, 1, 1);
		glBegin(GL_QUADS); {
			//glVertex2f(-1, -1);
			//glVertex2f(-1, 0);
			//glVertex2f(0, 0);
			//glVertex2f(0, -1);

			for (size_t j = 0; j < WIDTH / DPP; j++)
			{
				for (size_t i = 0; i < HEIGHT / DPP; i++)
				{
					if (pixelMatrix[i][j])
						pixel(j, i);
				}
			}
			
		}
		glEnd();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
	    /* Poll for and process events */ 
		glfwPollEvents();
		frame = (frame++ % 240);
	}

	glfwTerminate();
	return 0;
}

void charCallback(GLFWwindow* window, unsigned int key)
{
	term->keyHandler(key, 0, 0, 0, 1);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	term->keyHandler(key, scancode, action, mods, 0);
}

void _stdcall glVertex2fToScreen(float x, float y)
{
	// -1 -- 1 <= 0 - width
	//std::cout << "glVertex2f(" << 2 * (x / WIDTH) - 1 << ", " << 2 * (y / HEIGHT) - 1 << ")" << std::endl;
	glVertex2f( 2 * (x / WIDTH) - 1, 1 - 2 * (y / HEIGHT));
}

void pixel(int x, int y)
{
	glVertex2fToScreen((0 + x) * DPP, (0 + y) * DPP);
	glVertex2fToScreen((1 + x) * DPP, (0 + y) * DPP);
	glVertex2fToScreen((1 + x) * DPP, (1 + y) * DPP);
	glVertex2fToScreen((0 + x) * DPP, (1 + y) * DPP);
}