#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>

// Optional: limit the frame rate
constexpr double TARGET_TIME_FOR_FRAME { 1.0 / 60.0 };

int main() {
    // Set an error callback to display glfw errors
    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "Error " << error << ": " << description << std::endl;
    });

    // Initialize glfw
    if (!glfwInit()) {
        return -1;
    }

#ifdef __APPLE__
    // We need to explicitly ask for a 3.3 context on Mac
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    // Create window
    GLFWwindow* window { glfwCreateWindow(1280, 720, "Window", nullptr, nullptr) };
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Intialize glad (loads the OpenGL functions)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {

        // Get time (in second) at loop beginning
		double startTime { glfwGetTime() };

        // Update and render here

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();

        // Optional: limit the frame rate
		double elapsedTime { glfwGetTime() - startTime };
        // wait the remaining time to match the target wanted frame rate
		if(elapsedTime < TARGET_TIME_FOR_FRAME)
		{
			glfwWaitEventsTimeout(TARGET_TIME_FOR_FRAME-elapsedTime);
		}
    }

    glfwTerminate();
    return 0;
}
