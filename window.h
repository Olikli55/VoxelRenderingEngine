#ifndef WINDOW_CLASS_H //  just so to prevent compiler to compiling it twice
#define WINDOW_CLASS_H




class Window
{
public:
	unsigned int w;
	unsigned int h;
	GLFWwindow* window;
	Window(unsigned short h, unsigned short w);
};

#endif

