#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <cmath>
#include <learnopengl/shader_s.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader zprogram
	// ------------------------------------
	Shader ourShader("4.1.shader.vs", "4.1.shader.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float x1 = 0.1, x2 = 0.1, x3 = -0.1, x4 = -0.1, y1 = 0.8, y2 = 0.6, y3 = 0.6, y4 = 0.8,
		x5 = 1.0f, x6 = 1.0f, x7 = 0.7f, x8 = 0.7f, y5 = -0.8f, y6 = -1.0f, y7 = -0.8f, y8 = -1.0f;
	unsigned int VBO, VAO, EBO;

	// load and create a texture 
	// -------------------------
	unsigned int texture, texture1;
	glGenTextures(1, &texture);
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	//unsigned char *data = stbi_load(FileSystem:getPath("C:\\Users\\Soha\\source\\repos\\Project1\\Project1\\n.jpg").c_str(), &width, &height, &nrChannels, 0);

	unsigned char *data = stbi_load("6.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	// render loop
	// -----------
l:while (!glfwWindowShouldClose(window))
{
	// input
	// -----
	processInput(window);

	// render
	// ------
	float vertices[] = {
		// positions          // colors           // texture coords
		x1,  y1, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		x2, y2, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		x3, y3, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		x4,  y4, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // top left 

		x5, y5, 0.0f,    1.0f,0.0f,0.0f,     0.6f,0.55f,
		x6, y6, 0.0f,   0.0f, 1.0f, 0.0f,   0.6f, 0.4f,
		x7, y7, 0.0f,   0.0f, 0.0f, 1.0f,   0.35f, 0.55f,
		x8,  y8, 0.0f,   1.0f, 1.0f, 0.0f,   0.35f, 0.4f

	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3,  // second triangle
		4, 5, 7,
		5, 6, 7
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	stbi_set_flip_vertically_on_load(true);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// bind Texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// render container
	ourShader.use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 4, 3);
	glDrawArrays(GL_TRIANGLES, 5, 3);
	y1 -= 0.0005, y2 -= 0.0005, y3 -= 0.0005, y4 -= 0.0005;
	if (y2 <= -1)
	{
		y1 = 0.8, y2 = 0.6, y3 = 0.6, y4 = 0.8;
	}
	static int oldState3 = GLFW_RELEASE;
	int newState3 = glfwGetKey(window, GLFW_KEY_LEFT);
	if (newState3 == GLFW_RELEASE && oldState3 == GLFW_PRESS) {
		x5 -= 0.1, x6 -= 0.1, x7 -= 0.1, x8 -= 0.1;
	}
	oldState3 = newState3;

	static int oldState4 = GLFW_RELEASE;
	int newState4 = glfwGetKey(window, GLFW_KEY_RIGHT);
	if (newState4 == GLFW_RELEASE && oldState4 == GLFW_PRESS) {
		x5 += 0.1, x6 += 0.1, x7 += 0.1, x8 += 0.1;
	}
	oldState4 = newState4;


	float x = rand() % 100;
	x = x / 100.0;
	float y = rand() % 100;
	y = y / 100.0;
	y -= x;
	float a = (x5 + x7) / 2;
	if (y2 <= y5&&abs(x1 - x5) <= 0.3)
	{
		x1 = y;
		x2 = y;
		x3 = y - 0.2;
		x4 = y - 0.2;
		y1 = 0.8;
		y2 = 0.6;
		y3 = 0.6;
		y4 = 0.8;
	}
	else if (y2 <= -0.9)
	{
	   if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
		{
			Sleep(100);
			if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
			{
				goto l;
			}
		}

	}
	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	// -------------------------------------------------------------------------------
	glfwSwapBuffers(window);
	glfwPollEvents();
}

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}