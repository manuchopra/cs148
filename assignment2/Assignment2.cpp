//using namespace std;

#include "assignment2/Assignment2.h"
#include "common/core.h" // <-- haha.
#include "common/Utility/Mesh/Simple/PrimitiveCreator.h"
#include "common/Utility/Mesh/Loading/MeshLoader.h"
#include <cmath>
#include <iostream>
#include <fstream>

namespace
{
    const int SHADER_ERROR_LOG_SIZE = 500;
    
    bool VerifyShaderCompile(GLuint newShaderObject)
    {
        GLint compileStatus;
        OGL_CALL(glGetShaderiv(newShaderObject, GL_COMPILE_STATUS, &compileStatus));
        if (compileStatus != GL_TRUE) {
            char errorLogBuffer[SHADER_ERROR_LOG_SIZE];
            OGL_CALL(glGetShaderInfoLog(newShaderObject, SHADER_ERROR_LOG_SIZE, NULL, errorLogBuffer));
            std::cerr << "ERROR: Shader compilation failure -- " << std::endl << errorLogBuffer << std::endl;
            return false;
        }
        return true;
    }
    
    bool VerifyProgramLink(GLuint shaderProgram)
    {
        GLint linkStatus;
        OGL_CALL(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus));
        if (linkStatus != GL_TRUE) {
            char errorLogBuffer[SHADER_ERROR_LOG_SIZE];
            OGL_CALL(glGetProgramInfoLog(shaderProgram, SHADER_ERROR_LOG_SIZE, NULL, errorLogBuffer));
            std::cerr << "ERROR: Program link compilation failure -- " << std::endl << errorLogBuffer << std::endl;
            return false;
        }
        return true;
    }
}

Assignment2::Assignment2(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera):
Application(std::move(inputScene), std::move(inputCamera))
{
    vertexPositions =
    std::initializer_list<glm::vec4>({
        // Triangle 1
        {0.f, 0.f, 0.f, 1.f},
        {1.f, 0.f, 0.f, 1.f},
        {1.f, 1.f, 0.f, 1.f},
        // Triangle 2
        {0.f, 0.f, 0.f, 1.f},
        {-1.f, 1.f, 0.f, 1.f},
        {-1.f, 0.f, 0.f, 1.f}
    });
    
    time = 0;
}

std::unique_ptr<Application> Assignment2::CreateApplication(std::shared_ptr<class Scene> scene, std::shared_ptr<class Camera> camera)
{
    return make_unique<Assignment2>(std::move(scene), std::move(camera));
}

std::shared_ptr<class Camera> Assignment2::CreateCamera()
{
    // Specify any old aspect ratio for now, we'll update it later once the window gets made!
    // Read more about Field of View: http://rg3.name/201210281829.html!
    // Note that our field of view is the VERTICAL field of view (in degrees).
    return std::make_shared<PerspectiveCamera>(75.f, 1280.f / 720.f);
}

glm::vec2 Assignment2::GetWindowSize() const
{
    return glm::vec2(1280.f, 720.f);
}

void Assignment2::SetupScene()
{
    SetupExample1();
}

void Assignment2::SetupCamera()
{
}

void Assignment2::HandleInput(SDL_Keysym key, Uint32 state, Uint8 repeat, double timestamp, double deltaTime)
{
    Application::HandleInput(key, state, repeat, timestamp, deltaTime);
}

void Assignment2::HandleWindowResize(float x, float y)
{
    Application::HandleWindowResize(x, y);
}

void Assignment2::SetupExample1()
{
    // Insert "Load and Compile Shaders" code here.
    
    //--------------------------------3-SHADERS--------------------------------//
    //Filepaths for vert and frag shaders
    const std::string vertFilePath = std::string(STRINGIFY(SHADER_PATH)) + "/hw2/hw2.vert";
    const std::string fragFilePath = std::string(STRINGIFY(SHADER_PATH)) + "/hw2/hw2.frag";
    
    //Create ifstreams
    std::ifstream vertFileStream;
    std::ifstream fragFileStream;
    
    std::string vertData;
    std::string fragData;
    std::string line;
    
    
    //Read data from vert and frag shaders into 2 strings
    vertFileStream.open (vertFilePath);
    while(!vertFileStream.eof()) // To get you all the lines.
    {
        getline(vertFileStream,line); // Saves the line in STRING.
        vertData += line + "\n";
    }
    vertFileStream.close();
    fragFileStream.open (fragFilePath);
    while(!fragFileStream.eof()) // To get you all the lines.
    {
        getline(fragFileStream,line); // Saves the line in STRING.
        fragData += line + "\n";
    }
    fragFileStream.close();
    
    //Print
    // std::cout<<"------VERT DATA HERE-----"<<std::endl;
    // std::cout<<vertData<<std::endl; // Prints our STRING.
    // std::cout<<"------FRAG DATA HERE-----"<<std::endl;
    // std::cout<<fragData<<std::endl; // Prints our STRING.
    
    //Use glCreateShader to create the appropriate typed shader
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    //Use glShaderSource to tell OpenGL what the contents of each shader is
    const char *c_vertStr = vertData.c_str();
    const char *c_fragStr = fragData.c_str();
    glShaderSource(vertShader, 1, &c_vertStr, NULL);
    glShaderSource(fragShader, 1, &c_fragStr, NULL);
    
    //Use glCompileShader to compile both your vertex and fragment shader
    glCompileShader(vertShader);
    glCompileShader(fragShader);
    this->shaderProgram = glCreateProgram();
    glAttachShader(this->shaderProgram, vertShader);
    glAttachShader(this->shaderProgram, fragShader);
    glLinkProgram(this->shaderProgram);
    
    // Checkpoint 1.
    // Modify this part to contain your vertex shader ID, fragment shader ID, and shader program ID.
    const GLuint vertexShaderId = vertShader;
    const GLuint fragmentShaderId = fragShader;
    const GLuint shaderProgramId = this->shaderProgram;
    
    //-----------------------------------4-GEOMETRY-----------------------------//
    
    //Generate ONE vertex array object ID
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    this->vertexArrayObject = vertexArrayObject;
    
    //Bind the vertex array object using glBindVertexArray
    glBindVertexArray(this->vertexArrayObject);
    
    //Generate a buffer ID for the vertex positions using glGenBuffers
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    
    uint bufferSize = sizeof(glm::vec4)*vertexPositions.size();
    
    std::cout << bufferSize << std::endl;
    
    //Pass the vertex position data to the buffer using glBufferData
    glBufferData(GL_ARRAY_BUFFER, bufferSize, &vertexPositions[0], GL_STATIC_DRAW);
    
    //Use glVertexAttribPointer to let OpenGL how the currently bound buffer’s data should be used
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    
    //Use glEnableVertexAttribArray to make the current buffer object be passed to OpenGL along with the current vertex array object
    glEnableVertexAttribArray(0);
    
    //--------------------------------DO-NOT-CHANGE------------------------//
    
    // DO NOT EDIT OR REMOVE THE CODE IN THIS SECTION
    if (!VerifyShaderCompile(vertexShaderId) || !VerifyShaderCompile(fragmentShaderId) || !VerifyProgramLink(shaderProgramId)) {
        std::cout << "FAILURE: Checkpoint 1 failed." << std::endl;
    } else {
        std::cout << "SUCCESS: Checkpoint 1 completed." << std::endl;
    }
    
    OGL_CALL(glDetachShader(shaderProgramId, vertexShaderId));
    OGL_CALL(glDeleteShader(vertexShaderId));
    OGL_CALL(glDetachShader(shaderProgramId, fragmentShaderId));
    OGL_CALL(glDeleteShader(fragmentShaderId));
    // FINISH DO NOT EDIT OR REMOVE THE CODE IN THIS SECTION
    
    // Insert "Setup Buffers" code here.
}

void Assignment2::Tick(double deltaTime)
{
    // Insert "Send Buffers to the GPU" and "Slightly-More Advanced Shaders" code here.
    glUseProgram(this->shaderProgram);
    glBindVertexArray(this->vertexArrayObject);
    glDrawArrays(GL_TRIANGLES, 0, vertexPositions.size());
    
    time += deltaTime;
    // std::cout << time << std::endl;
    printf("%f\n", time);
    
    GLint location = glGetUniformLocation(this->shaderProgram, "inputTime");
    glUniform1f(location, time);
}
