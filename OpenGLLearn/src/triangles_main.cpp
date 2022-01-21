/**
OPENGL��������������


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

	

	//��ʼ��
	glfwInit();
	//���ú�OPENGL�汾��ģʽ
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	//��������
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello!OpenGL", NULL, NULL);
	if (window == NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//glad ���أ��������OpenGL�ĳ�ʼ��������OpenGL�ĺ���ָ��
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/**
	������ɫ��������Ҫ��̬���봫��GPU
	*/

	//���嶥����ɫ������Ȼ���ַ���GLSL���봫�룬Ȼ����б���
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
	glCompileShader(vertexShader);
	//����Ƿ����ɹ�
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "vertexShaderSourceCompile Error��" << infoLog << std::endl;
	}

	/*
	Ƭ����ɫ������̬����
	*/

	//
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
	glCompileShader(fragmentShader);

	//����Ƿ����ɹ�
	
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "fragmentShaderSourceCompile Error��" << infoLog << std::endl;
	}

	/*
	���ӣ���ɫ��
	*/
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) 
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "��ɫ����������ʧ�ܣ�" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	//���嶥������
	float vertices[] = {
		-0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		0.0f,0.5f,0.0f
	};

	unsigned int VBO,VAO;
	glGenVertexArrays(1,&VAO);
	//����һ�����㻺�����
	glGenBuffers(1,&VBO);
	//
	glBindVertexArray(VAO);
	//ָ�����߰󶨡���������GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	//��CPU�����ݷ��͵�GPU�����һ���������Ǹ���GPU���ݲ���ı䡣
	//������ݻ�Ƶ���ı䣬��ʹ��GL_DYNAMIC_DRAW��GL_STREAM_DRAW,GPU�Ὣ���ݷ��ڸ���д����ڴ沿��
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
	//���ö���ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//�ȴ��رմ���
	while (!glfwWindowShouldClose(window))
	{

		//��Ⱦ
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