#include <iostream>
#include <vector>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <time.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>


#include "ModelManager.h"
#include "Camera.h"
#include "Terrain.h"



/// <summary>
/// Main Function to launch application
/// </summary>
/// <returns></returns>
int main()
{
	// Initialise GLFW
	if (!glfwInit())
	{		
		std::cout << "Failed to initialize GLFW" << std::endl;
		std::cin.get();
		return -1;
	}
	
	// Open a window and create its OpenGL context	
	//glfwGetPrimaryMonitor()
	GLFWwindow* window = glfwCreateWindow(1024, 768, "ModelViewer", NULL, NULL);
	if (window == NULL) {		
		std::cout << "Failed to Create GLFW Window." << std::endl;
		std::cin.get();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	// Initialize GLEW
	glewExperimental = true; 
	if (glewInit() != GLEW_OK) {		
		std::cout << "Failed to GLEW." << std::endl;
		std::cin.get();
		glfwTerminate();
		return -1;
	}	
	
	//Init Camera
	printf("\n\nInit Camera... ");
	auto pCamera = new Camera();
	pCamera->SetPos(glm::vec3(0, 292, -745));

	//Set Projection Matrix
	int count = 0;
	const GLFWvidmode *pScreen = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);
	float fAspectRatio = (float)pScreen->width / (float)pScreen->height;	
	glm::mat4 matProj = glm::perspective(glm::radians(60.0f), fAspectRatio, 0.1f, 10000.0f);
		

	//Init Model Manager
	printf("\n\nInit Models... ");
	ModelManager* pModelManager = new ModelManager();
	if(pModelManager->Initialize() == false)
		return 0;

	//Init Terrain
	printf("\n\nInit Terrain... ");
	Terrain* pTerrain = new Terrain(100, 100);
	if (pTerrain->InitTerrain() == false)
		return 0;

	printf("\n\nRendering Start... ");
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	srand(static_cast<unsigned int>( time(NULL)));
	double lastTime = glfwGetTime(), duration = 0.0f;	
	do
	{
		double currentTime = glfwGetTime();
		float Tick = (float)(currentTime - lastTime);
		lastTime = currentTime;

		//Wireframe
		if((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS))
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				
		pCamera->Process(window, Tick);
		pTerrain->Process(Tick);
		pModelManager->Process(window, Tick);


		pTerrain->Render(pCamera->GetCameraMatrix(), matProj, pCamera->GetPos());		
		pModelManager->Render(pCamera->GetCameraMatrix(), matProj, pCamera->GetPos());
			
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	while ((glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) && (glfwWindowShouldClose(window) == 0));
	
	delete pModelManager;
	delete pCamera;

	// Close Render window and terminate GLFW
	glfwTerminate();
	return 0;
}