// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <math.h> 

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;
static std::vector < GLfloat > data;
static int vertexAmount = 0; 
static int size;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "shader.hpp"
#include "controls.hpp"

void resize()
{
        GLfloat minC = 10000, maxC = -10000, minX = 10000, maxX = -10000, minY = 10000, maxY = -10000, scale, scaleC, moveX, moveY;
        for(int i = 0; i < data.size();i++)
        {
                if(data[i] < minX)
                        minX = data[i];
                if(data[i] > maxX)
                        maxX = data[i];
                if(data[++i] < minY)
                        minY = data[i];
                if(data[i] > maxY)
                        maxY = data[i]; 
                if(data[++i] < minC)
                        minC = data[i];
                if(data[i] > maxC)
                        maxC = data[i];
        }
		std::cout << "maxX" << maxX << " minX " << minX <<std::endl;
		std::cout << "maxY" << maxY << " minY " << minY <<std::endl;
        scale = max( (maxY - minY)/2, (maxX - minX)*cos(maxY-minY)/2);
        scaleC = (maxC - minC);
        moveX = (maxX + minX)/2;
        moveY = (maxY + minY)/2;
		std::cout << "scale: " << scale << std::endl;
        for(int i = 0; i < data.size();i++)
        {
                data[i] = (data[i] -moveX) / scale;
                data[++i] = (data[i] -moveY) / scale;
                data[++i] = (data[i] -minC) / scaleC;  
        }       
}

int loadVertex(int argc, char * argv[])
{
        std::cout << argc << std::endl;
        for(int f = 0; f<argc-1; f++)
        {
                FILE* file = fopen (argv[f+1] , "r");
                char wiersz[1000];
                char latlon=0;
                std::cout << "laduje\n";
                while (fgets(wiersz, 1000, file) != NULL){
                        //std::cout << wiersz << std::endl;
                        float lat,lon,ele;
                        char *p;
                        if (strstr(wiersz, "<trkpt ")){
                                 sscanf( strstr(wiersz, "lat=\"")+5, "%f", &lat);
                                 sscanf( strstr(wiersz, "lon=\"")+5, "%f", &lon);
                                 data.push_back(lat);
                                 data.push_back(lon);

                                 latlon = 1;
                        }
                        else if (p=strstr(wiersz, "<ele>")){
                                 sscanf( p+5, "%f</ele>", &ele); 
                                 if (latlon == 1) {
                                            data.push_back(ele);
                                 }
                        }
                }

                std::cout << "done" << std::endl;
        }
        //for(int i = 0; i < data.size(); i+=3)
          //      std::cout << i<< " color: " << data[i+2] << " "<< data[i] << " " << data[i+1] << std::endl;
        resize();
		std::cout << data.size() << std::endl;
        for(int i = 0; i < data.size(); i+=3)
                ;//std::cout << i<< " color: " << data[i+2] << " "<< data[i] << " " << data[i+1] << std::endl;
}

int main(int argc, char * argv[] )
{
        loadVertex(argc, argv);
		/*data.push_back(-0.5);
		data.push_back(-0.5);
		data.push_back(0);
		data.push_back(-0.5);
		data.push_back(0.5);
		data.push_back(0);
		data.push_back(0.5);
		data.push_back(0.5);
		data.push_back(0);
		data.push_back(0.5);
		data.push_back(-0.5);
		data.push_back(0);*/
        if( !glfwInit() )
        {
                fprintf( stderr, "Failed to initialize GLFW\n" );
                return -1;
        }

        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Open a window and create its OpenGL context
        window = glfwCreateWindow( 1024, 768, "Tutorial 03 - Matrices", NULL, NULL);
        if( window == NULL ){
                fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
                glfwTerminate();
                return -1;
        }
        glfwMakeContextCurrent(window);

        // Initialize GLEW
        glewExperimental = true; // Needed for core profile
        if (glewInit() != GLEW_OK) {
                fprintf(stderr, "Failed to initialize GLEW\n");
                return -1;
        }

        // Ensure we can capture the escape key being pressed below
        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

        // Dark blue background
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        // Create and compile our GLSL program from the shaders
        GLuint programID = LoadShaders( "SimpleTransform.vertexshader", "SingleColor.fragmentshader" );

        // Get a handle for our "MVP" uniform
        GLuint MatrixID = glGetUniformLocation(programID, "MVP");

        // Model matrix : an identity matrix (model will be at the origin)
        glm::mat4 Model      = glm::mat4(1.0f);
        // Our ModelViewProjection : multiplication of our 3 matrices


        GLuint vertexbuffer;
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, data.size()* sizeof(GLfloat), &data.front(), GL_STATIC_DRAW);
        GLuint vertexPositionID = glGetAttribLocation(programID, "vertexPosition_modelspace");

        do{
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glUseProgram(programID);
                computeMatricesFromInputs();
                glm::mat4 ProjectionMatrix = getProjectionMatrix();
                glm::mat4 ViewMatrix = getViewMatrix();
                glm::mat4 MVP = ProjectionMatrix * ViewMatrix * Model;


                glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

                // 1rst attribute buffer : vertices
                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
                glVertexAttribPointer(
                        vertexPositionID,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                        3,                  // size
                        GL_FLOAT,           // type
                        GL_FALSE,           // normalized?
                        0,                  // stride
                        (void*)0            // array buffer offset
                );

                // Draw the triangle !
                glDrawArrays(GL_LINE_STRIP, 0, data.size()/3); // 3 indices starting at 0 -> 1 triangle

                glDisableVertexAttribArray(0);

                // Swap buffers
                glfwSwapBuffers(window);
                glfwPollEvents();

        } // Check if the ESC key was pressed or the window was closed
        while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
                   glfwWindowShouldClose(window) == 0 );

        // Cleanup VBO and shader
        glDeleteBuffers(1, &vertexbuffer);
        glDeleteProgram(programID);
        glDeleteVertexArrays(1, &VertexArrayID);

        // Close OpenGL window and terminate GLFW
        glfwTerminate();

        return 0;
}


