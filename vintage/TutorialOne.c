#include <glew.h>
#include <glfw3.h>


float triangle[9] =
	{ 0.0f, 0.5f, 0.0f,
	  0.5f, -0.5f, 0.0f,
	  -0.5f, -0.5f, 0.0f };

int main()
{
	GLFWwindow *window;
	unsigned int vbo = 0;
	unsigned int vao = 0;
	const char *vertexShader;
	const char *fragmentShader;

	glfwInit();

	window = glfwCreateWindow( 800, 600, "A window", NULL, NULL  );
	glfwMakeContextCurrent( window );

	glewInit();


	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LESS );




	glGenBuffers( 1, &vbo );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, 9 * sizeof(float), &triangle, GL_STATIC_DRAW );

	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );
	glEnableVertexAttribArray( 0 );
	glBindBuffer( GL_VERTEX_ARRAY, vbo );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL );

	while( !glfwWindowShouldClose(window) )
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}