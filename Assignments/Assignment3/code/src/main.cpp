//Assignment 03: Lighting and shading

/*References
  Trackball: http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
*/

#include "utils.h"

#define  GLM_FORCE_RADIANS
#define  GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

//Globals
int screen_width = 640, screen_height=640;
GLint vModel_uniform, vView_uniform, vProjection_uniform;
GLint vColor_uniform;
glm::mat4 modelT, viewT, projectionT;//The model, view and projection transformations

double oldX, oldY, currentX, currentY;
bool isDragging=false;

void createCubeObject(unsigned int &, unsigned int &);
void createParametricObject(unsigned int &, unsigned int &);

void setupModelTransformation(unsigned int &);
void setupViewTransformation(unsigned int &);
void setupProjectionTransformation(unsigned int &);
glm::vec3 getTrackBallVector(double x, double y);

struct Light 
{
    glm::vec3 position;
    glm::vec3 intensities; //a.k.a. the color of the light
};

Light gLight;
int main(int, char**)
{
    // Setup window
    GLFWwindow *window = setupWindow(screen_width, screen_height);
    ImGuiIO& io = ImGui::GetIO(); // Create IO object

    ImVec4 clearColor = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);

    // unsigned int shaderProgram = createProgram("./shaders/vshader_g.vs", "./shaders/fshader_g.fs");
    // unsigned int shaderProgram = createProgram("./shaders/vshader_p.vs", "./shaders/fshader_p.fs");
    unsigned int shaderProgram = createProgram("./shaders/vshader_b.vs", "./shaders/fshader_b.fs");


    //Get handle to color variable in shader
    vColor_uniform = glGetUniformLocation(shaderProgram, "vColor");
    if(vColor_uniform == -1){
        fprintf(stderr, "Could not bind location: vColor\n");
        exit(0);
    }

    glUseProgram(shaderProgram);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    setupModelTransformation(shaderProgram);
    setupViewTransformation(shaderProgram);
    setupProjectionTransformation(shaderProgram);
    // glBindAttribLocation(shaderProgram, 1, "vNormal");

    // createCubeObject(shaderProgram, VAO);
    createParametricObject(shaderProgram, VAO);

    oldX = oldY = currentX = currentY = 0.0;
    int prevLeftButtonState = GLFW_RELEASE;

    gLight.position = glm::vec3(-32.0, 0.0, 0.0);
    gLight.intensities = glm::vec3(1.0, 1.0, 0.5);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Get current mouse position
        int leftButtonState = glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT);
        double x,y;
        
        // GLint originsLoc = glGetUniformLocation(shaderProgram, "light.position");
        // glUniform3f(originsLoc, origins[i].x, origins[i].y, li.z);
        GLuint lightPos = glGetUniformLocation(shaderProgram, "light.position");
        glUniform3f(lightPos, gLight.position.x, gLight.position.y, gLight.position.z);
        
        GLuint lightColor = glGetUniformLocation(shaderProgram, "light.intensities");
        glUniform3f(lightColor, gLight.intensities.x, gLight.intensities.y, gLight.intensities.z);
    
        
        glfwGetCursorPos(window,&x,&y);
        if(leftButtonState == GLFW_PRESS && prevLeftButtonState == GLFW_RELEASE){
            isDragging = true;
            currentX = oldX = x;
            currentY = oldY = y;
        }
        else if(leftButtonState == GLFW_PRESS && prevLeftButtonState == GLFW_PRESS){
            currentX = x;
            currentY = y;
        }
        else if(leftButtonState == GLFW_RELEASE && prevLeftButtonState == GLFW_PRESS){
            isDragging = false;
        }

        // Rotate based on mouse drag movement

        prevLeftButtonState = leftButtonState;
        if(isDragging && (currentX !=oldX || currentY != oldY))
        {
            glm::vec3 va = getTrackBallVector(oldX, oldY);
            glm::vec3 vb = getTrackBallVector(currentX, currentY);

            GLuint lightPos = glGetUniformLocation(shaderProgram, "light.position");
            std::cout << lightPos << "\n";
            glUniform3f(lightPos, gLight.position.x, gLight.position.y, gLight.position.z);
            
            GLuint lightColoe = glGetUniformLocation(shaderProgram, "light.intensities");
            glUniform3f(lightColoe, gLight.intensities.x, gLight.intensities.y, gLight.intensities.z);
        

            float angle = acos(std::min(1.0f, glm::dot(va,vb)));
            glm::vec3 axis_in_camera_coord = glm::cross(va, vb);
            glm::mat3 camera2object = glm::inverse(glm::mat3(viewT*modelT));
            glm::vec3 axis_in_object_coord = camera2object * axis_in_camera_coord;
            modelT = glm::rotate(modelT, angle, axis_in_object_coord);
            glUniformMatrix4fv(vModel_uniform, 1, GL_FALSE, glm::value_ptr(modelT));
            

            oldX = currentX;
            oldY = currentY;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glUseProgram(shaderProgram);

        {
            ImGui::Begin("Information");                          
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO); 
        
        glUniform3f(vColor_uniform, 0.5, 0.5, 0.5);
        // glDrawArrays(GL_TRIANGLES, 0, 6*2*3);
        glDrawArrays(GL_TRIANGLES, 0, 10*5*2*3); // 30 Longitudes * 10 Latitude * 2 Traingles * 3 Vertices
        // glUniform3f(vColor_uniform, 0.0, 0.0, 0.0);
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

    }

    // Cleanup
    cleanup(window);

    return 0;
}

void createCubeObject(unsigned int &program, unsigned int &cube_VAO)
{
    glUseProgram(program);

    //Bind shader variables
    int vVertex_attrib = glGetAttribLocation(program, "vVertex");
    if(vVertex_attrib == -1) {
        fprintf(stderr, "Could not bind location: vVertex\n");
        exit(0);
    }

    //Cube data
    GLfloat cube_vertices[] = {10, 10, -10, -10, 10, -10, -10, -10, -10, 10, -10, -10, //Front
                   10, 10, 10, -10, 10, 10, -10, -10, 10, 10, -10, 10}; //Back
    GLushort cube_indices[] = {
                0, 1, 2, 0, 2, 3, //Front
                4, 7, 5, 5, 7, 6, //Back
                1, 6, 2, 1, 5, 6, //Left
                0, 3, 4, 4, 7, 3, //Right
                0, 4, 1, 4, 5, 1, //Top
                2, 6, 3, 3, 6, 7 //Bottom
                };

    //Generate VAO object
    glGenVertexArrays(1, &cube_VAO);
    glBindVertexArray(cube_VAO);

    //Create VBOs for the VAO
    //Position information (data + format)
    int nVertices = (6*2)*3; //(6 faces) * (2 triangles each) * (3 vertices each)
    GLfloat *expanded_vertices = new GLfloat[nVertices*3];
    for(int i=0; i<nVertices; i++) {
        expanded_vertices[i*3] = cube_vertices[cube_indices[i]*3];
        expanded_vertices[i*3 + 1] = cube_vertices[cube_indices[i]*3+1];
        expanded_vertices[i*3 + 2] = cube_vertices[cube_indices[i]*3+2];
    }
    GLuint vertex_VBO;
    glGenBuffers(1, &vertex_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
    glBufferData(GL_ARRAY_BUFFER, nVertices*3*sizeof(GLfloat), expanded_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vVertex_attrib);
    glVertexAttribPointer(vVertex_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    delete []expanded_vertices;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); //Unbind the VAO to disable changes outside this function.
    
}

// start
// Change the equation for different surfaces. 
std::vector<GLfloat> parametricSurface(float u, float v)
{
    // Constant Radius
    int R = 5;

    GLfloat x = R*cos(u);
    GLfloat y = R*sin(u);
    GLfloat z = v;

    std::vector<GLfloat> ans = {x, y, z};
    return ans;

    // del f/ del u = (-R*sin(u), R*cos(u), 0)
    // del f/ del v = (0, 0, 1)
} 

std::vector<GLfloat> createNormals(float u, float v)
{
    // Constant Radius
    int R = 5;

    glm::vec3 dfbydu = glm::vec3(-R*sin(u), R*cos(u), 0);
    glm::vec3 dfbydv = glm::vec3(0, 0, 1);

    glm::vec3 cp = normalize(cross(dfbydu, dfbydv));
    std::vector<GLfloat> ans = {cp.x, cp.y, cp.z};
    return ans;

    // del f/ del u = (-R*sin(u), R*cos(u), 0)
    // del f/ del v = (0, 0, 1)
} 
// end


void createParametricObject(unsigned int &program, unsigned int &shape_VAO)
{
    glUseProgram(program);

    //Bind shader variables
    int vVertex_attrib = glGetAttribLocation(program, "vVertex");
    if(vVertex_attrib == -1) {
        fprintf(stderr, "Could not bind location: vVertex\n");
        exit(0);
    }

    // int vNormal_attrib = glGetAttribLocation(program, "vNormal");
    // if(vNormal_attrib == -1) {
    //     fprintf(stderr, "Could not bind location: vNormal\n");
    //     exit(0);
    // }

    //Shape data
    int res_u = 10, res_v = 5;
    
    size_t nVertices = res_u*res_v*2*3; // No. of vertices of the shape
    GLfloat *shape_vertices = new GLfloat[nVertices*3]; 
    GLfloat *normals = new GLfloat[nVertices*3]; 
    //TODO: Generate shape vertices and create trangles from those.
    //Note: In order to avoid generating an index array for triangles first and then expanding the coordinate array for triangles,
    // You can directly generate coordinates for successive triangles in two nested for loops to scan over the surface.

// Start
    // Specify the range of parameters
    float start_u = 0, start_v = 0, end_u = glm::pi<float>()*2, end_v = 10;

    // Calculate the step size to make uniform size of rectangles
    float step_u = (end_u - start_u)/res_u, step_v = (end_v - start_v)/res_v;
    
    int index = 0;
    
    // 2 Loops for 2 parameters.
    for (int j = 0; j < res_v; j++)
    {
        for (int i = 0; i < res_u; i++)
        {
            // Calculate the current and next value for both parameters. 
            float u = i*step_u + start_u;
            float v = j*step_v + start_v;
            float u_next = (i+1 == res_u) ? end_u : (i+1)*step_u + start_u;
            float v_next = (j+1 == res_v) ? end_v : (j+1)*step_v + start_v;

            // Order of Triangles
            std::vector<float> order = {u, v, u_next, v, u, v_next, u_next, v_next, u, v_next, u_next, v};

            // Calculate the coordinates of each triangles and add it to the array in correct order.
            for (int k = 0; k < order.size()-1; k += 2)
            {
                std::vector<GLfloat> ans = parametricSurface(order[k], order[k+1]);
                std::vector<GLfloat> ns = createNormals(order[k], order[k+1]);

                shape_vertices[index] = ans[0]; normals[index] = ns[0]; index++;
                shape_vertices[index] = ans[1]; normals[index] = ns[1]; index++;
                shape_vertices[index] = ans[2]; normals[index] = ns[2]; index++;
                
            }   
        }
    }
    
    //Generate VAO object
    glGenVertexArrays(1, &shape_VAO);
    glBindVertexArray(shape_VAO);

    //Create VBOs for the VAO
    GLuint vertex_VBO;
    glGenBuffers(1, &vertex_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
    glBufferData(GL_ARRAY_BUFFER, nVertices*3*sizeof(GLfloat), shape_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vVertex_attrib);
    glVertexAttribPointer(vVertex_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    delete []shape_vertices;

    GLuint normal_VBO;
    glGenBuffers(1, &normal_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, normal_VBO);
    glBufferData(GL_ARRAY_BUFFER, nVertices*3*sizeof(GLfloat), normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, 0);
    delete []normals;


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); //Unbind the VAO to disable changes outside this function.
}
    // End

void setupModelTransformation(unsigned int &program)
{
    //Modelling transformations (Model -> World coordinates)
    modelT = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));//Model coordinates are the world coordinates

    //Pass on the modelling matrix to the vertex shader
    glUseProgram(program);
    vModel_uniform = glGetUniformLocation(program, "vModel");
    if(vModel_uniform == -1){
        fprintf(stderr, "Could not bind location: vModel\n");
        exit(0);
    }
    glUniformMatrix4fv(vModel_uniform, 1, GL_FALSE, glm::value_ptr(modelT));
}


void setupViewTransformation(unsigned int &program)
{
    //Viewing transformations (World -> Camera coordinates
    //Camera at (0, 0, 100) looking down the negative Z-axis in a right handed coordinate system
    viewT = glm::lookAt(glm::vec3(40.0, -40.0, 40.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

    //Pass-on the viewing matrix to the vertex shader
    glUseProgram(program);
    vView_uniform = glGetUniformLocation(program, "vView");
    if(vView_uniform == -1){
        fprintf(stderr, "Could not bind location: vView\n");
        exit(0);
    }
    glUniformMatrix4fv(vView_uniform, 1, GL_FALSE, glm::value_ptr(viewT));
}

void setupProjectionTransformation(unsigned int &program)
{
    //Projection transformation
    projectionT = glm::perspective(45.0f, (GLfloat)screen_width/(GLfloat)screen_height, 0.1f, 1000.0f);

    //Pass on the projection matrix to the vertex shader
    glUseProgram(program);
    vProjection_uniform = glGetUniformLocation(program, "vProjection");
    if(vProjection_uniform == -1){
        fprintf(stderr, "Could not bind location: vProjection\n");
        exit(0);
    }
    glUniformMatrix4fv(vProjection_uniform, 1, GL_FALSE, glm::value_ptr(projectionT));
}

glm::vec3 getTrackBallVector(double x, double y)
{
	glm::vec3 p = glm::vec3(2.0*x/screen_width - 1.0, 2.0*y/screen_height - 1.0, 0.0); //Normalize to [-1, +1]
	p.y = -p.y; //Invert Y since screen coordinate and OpenGL coordinates have different Y directions.

	float mag2 = p.x*p.x + p.y*p.y;
	if(mag2 <= 1.0f)
		p.z = sqrtf(1.0f - mag2);
	else
		p = glm::normalize(p); //Nearest point, close to the sides of the trackball
	return p;
}

