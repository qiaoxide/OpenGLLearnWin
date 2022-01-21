/**
OPENGL基础绘制三角形


*/


#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>


void framebuffer_size_callback(GLFWwindow* window,int width,int height);

const char* vertexShaderSource = 
"#version 330 core\n"
"layout (location=0) in vec3 aPos;\n"
"void main() \n"
"{\n"
"	gl_Position=vec4(aPos.x,aPos.y,aPos.z,1.0);\n"
"}\0";

const char* fragmentShaderSource = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor=vec4(1.0f,0.5f,0.2f,1.0f);\n"
"}\0";


//int main() 
int triangles()
{

	

	//初始化
	glfwInit();
	//设置好OPENGL版本和模式
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	//创建窗口
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello!OpenGL", NULL, NULL);
	if (window == NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//glad 加载，里面包含OpenGL的初始化，加载OpenGL的函数指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/**
	顶点着色器程序，需要动态编译传给GPU
	*/

	//定义顶点着色器对象，然后将字符串GLSL代码传入，然后进行编译
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
	glCompileShader(vertexShader);
	//检查是否编译成功
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "vertexShaderSourceCompile Error：" << infoLog << std::endl;
	}

	/*
	片段着色器，动态编译
	*/

	//
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
	glCompileShader(fragmentShader);

	//检查是否编译成功
	
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "fragmentShaderSourceCompile Error：" << infoLog << std::endl;
	}

	/*
	链接，着色器
	*/
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) 
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "着色器程序链接失败：" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	//定义顶点数据
	float vertices[] = {
		-0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		0.0f,0.5f,0.0f
	};

	unsigned int VBO,VAO;
	glGenVertexArrays(1,&VAO);
	//生成一个顶点缓冲对象
	glGenBuffers(1,&VBO);
	//
	glBindVertexArray(VAO);
	//指定或者绑定。绑定类型是GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	//将CPU的数据发送到GPU，最后一个参数，是告诉GPU数据不会改变。
	//如果数据会频繁改变，就使用GL_DYNAMIC_DRAW或GL_STREAM_DRAW,GPU会将数据放在高速写入的内存部分
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
	//设置顶点指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//等待关闭窗口
	while (!glfwWindowShouldClose(window))
	{

		//渲染
		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1,&VAO);
	glDeleteBuffers(1,&VBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
	glViewport(0, 0, width, height);
}