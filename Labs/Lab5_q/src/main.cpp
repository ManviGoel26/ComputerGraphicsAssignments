#include "utils.h"

int width = 640, height=640;
#define DIVISIONS 100
#define PI 3.14
#define DELTA_ANGLE 3
#define VERTEX_OFFSET_Z 3
GLint vModel_uniform, vView_uniform, vProjection_uniform;

glm::mat4 modelT, viewT, projectionT;//The model, view and projection transformations

double oldX, oldY, currentX, currentY;
bool isDragging=false;
int size;
glm::vec3 getTrackBallVector(double x, double y);
bool raySphereIntersection(glm::vec3 p, glm::vec3 d, glm::vec3 c);
int random(int min, int max) //range : [min, max]
{
   static bool first = true;
   if (first) 
   {  
      srand( time(NULL) ); //seeding for the first time only!
      first = false;
   }
   return min + rand() % (( max + 1 ) - min);
}

int main(int, char**)
{
    // Setup window
    GLFWwindow *window = setupWindow(width, height);
    ImGuiIO& io = ImGui::GetIO(); // Create IO object

    ImVec4 clearColor = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);

    unsigned int shaderProgram = createProgram("./shaders/vshader1.vs", "./shaders/fshader3.fs");
    unsigned int shaderProgram2 = createProgram("./shaders/vshader.vs", "./shaders/fshader2.fs");
    unsigned int shaderProgram3 = createProgram("./shaders/vshader.vs", "./shaders/fshader.fs");
    glUseProgram(shaderProgram);

    

    setupModelTransformation(shaderProgram);
    setupViewTransformation(shaderProgram);
    setupProjectionTransformation(shaderProgram, width , height);
    
    int vVertex_attrib = glGetAttribLocation(shaderProgram, "vVertex");
    if(vVertex_attrib == -1) {
        std::cout << "Could not bind location: vVertex\n" ;
        exit(0);
    }else{
        std::cout << "vVertex found at location " << vVertex_attrib << std::endl;
    }

    // int vNormal_attrib = glGetAttribLocation(shaderProgram, "vertex_norm");
    // if(vNormal_attrib == -1) {
    //     std::cout << "Could not bind location: vertex_norm\n" ;
    //     exit(0);
    // }else{
    //     std::cout << "aNormal found at location " << vNormal_attrib << std::endl;
    // }
/////////////////////////////////////////////////////////////////////
/// 

GLuint sphere_VAO, indices_IBO;
    int nTheta = 180/DELTA_ANGLE + 1;
    int nPhi = 360/DELTA_ANGLE + 1;

    float *vertices = new float[nTheta*nPhi*3];
    float *normals = new float[nTheta*nPhi*3];
    float *uv = new float[nTheta*nPhi*2];
    // float *v = new float[nTheta*nPhi];
    float theta, phi, x, y, z, x2, y2, z2;
    float radius = 0.5f;
    for (int j = 0; j<nTheta; j++)
        for(int i=0; i<nPhi; i++)
        {
            theta = float(j*DELTA_ANGLE)*M_PI/180.0;
            phi = float(i*DELTA_ANGLE)*M_PI/180.0;
            x = radius * sinf(theta)*cosf(phi);
            y = radius * sinf(theta)*sinf(phi);
            z = radius * cos(theta); 
            normals[(i + j*nPhi)*3 + 0] = x; normals[(i + j*nPhi)*3 + 1] = y; normals[(i + j*nPhi)*3 + 2] = z; 
            vertices[(i + j*nPhi)*3 + 0] = radius*x; vertices[(i + j*nPhi)*3 + 1] = radius*y; vertices[(i + j*nPhi)*3 + 2] = radius*z - VERTEX_OFFSET_Z; 
     

        }
    float ptsx[nTheta * nPhi], ptsy[nTheta * nPhi],ptsz[nTheta * nPhi];
    float radius2 = 0.85f;
    for (int j = 0; j<nTheta; j++)
        for(int i=0; i<nPhi; i++)
        {
            theta = float(j*DELTA_ANGLE)*M_PI/180.0;
            phi = float(i*DELTA_ANGLE)*M_PI/180.0;
            x2 = radius2 * sinf(theta)*cosf(phi);
            y2 = radius2 * sinf(theta)*sinf(phi);
            z2 = radius2 * cos(theta); 

            ptsx[j*nPhi + i] = x2;
            ptsy[j*nPhi + i] = y2;
            ptsz[j*nPhi + i] = z2;
        }   


    int origin_index = random(0, nTheta * nPhi);
    int direction_index = random(0, nTheta * nPhi);    

    glm::vec3 point1 = glm::vec3(ptsx[origin_index], ptsy[origin_index], ptsz[origin_index]);
    glm::vec3 point2 = glm::vec3(ptsx[direction_index], ptsy[direction_index], ptsz[direction_index]);
    glm::vec3 direction  = point2 - point1;

    //Generate index array
    GLushort *indices = new GLushort[2*(nTheta-1)*(nPhi-1)*3];
    for(int j=0; j<(nTheta-1); j++)
        for(int i=0; i<(nPhi-1); i++)
        {
            //Upper triangle
            indices[(i + j*(nPhi-1))*6 + 0] = i + j*nPhi;
            indices[(i + j*(nPhi-1))*6 + 1] = i + (j+1)*nPhi;
            indices[(i + j*(nPhi-1))*6 + 2] = i + 1 + j*nPhi;

            //Lower triangle
            indices[(i + j*(nPhi-1))*6 + 3] = i + 1 + j*nPhi;
            indices[(i + j*(nPhi-1))*6 + 4] = i + (j+1)*nPhi;
            indices[(i + j*(nPhi-1))*6 + 5] = i + 1 + (j+1)*nPhi;
        }

    //Generate vertex buffer and index buffer

    glGenVertexArrays(1, &sphere_VAO);
    glBindVertexArray(sphere_VAO);

    GLuint vertex_VBO, normal_VBO;
    glGenBuffers(1, &vertex_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
    glBufferData(GL_ARRAY_BUFFER, nTheta*nPhi*3*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vVertex_attrib);
    glVertexAttribPointer(vVertex_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0); 


    
    glGenBuffers(1, &indices_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (nTheta-1)*(nPhi-1)*6*sizeof(GLushort), indices, GL_STATIC_DRAW);



    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_IBO);
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


   float anglex=0.0f, angley=0.0f, anglez= 0.0f;
   GLfloat matrix[] = {
    1.0f, 0.0f, 0.0f, 0.0f, // first column
    0.0f, 1.0f, 0.0f, 0.0f, // second column
    0.0f, 0.0f, 1.0f, 0.0f,// third column
     0.0f, 0.0f, 0.0f, 1.0f
    };

    GLfloat matrix2[] = {
    1.0f, 0.0f, 0.0f, 0.0f, // first column
    0.0f, 1.0f, 0.0f, 0.0f, // second column
    0.0f, 0.0f, 1.0f, 0.0f,// third column
     0.0f, 0.0f, 0.0f, 1.0f
    };

   
    float vertices_line[] = {
        point1.x , point1.y, point1.z,
        point2.x, point2.y, point2.z
    };
    unsigned int VAO3, VBO3;

    glGenVertexArrays(1, &VAO3);
    glGenBuffers(1, &VBO3);
    // glGenBuffers(1, &EBO);
    glBindVertexArray(VAO3);

    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_line), vertices_line, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glBindVertexArray(0);  


    oldX = oldY = currentX = currentY = 0.0;
    int prevLeftButtonState = GLFW_RELEASE;



    // calculate t
    // check intersection;

    // set_point to display

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

      

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glUseProgram(shaderProgram);

        if( ImGui::IsKeyPressed('O')) {
            std::cout << "o" << std::endl;

             int origin_index = random(0, nTheta * nPhi);
            int direction_index = random(0, nTheta * nPhi);    

            glm::vec3 point1 = glm::vec3(ptsx[origin_index], ptsy[origin_index], ptsz[origin_index]);
            glm::vec3 point2 = glm::vec3(ptsx[direction_index], ptsy[direction_index], ptsz[direction_index]);
            // glm::vec3 direction  = point2 - point1;  

             float vertices_line[] = {
                point1.x , point1.y, point1.z,
                point2.x, point2.y, point2.z
            };

            glBindBuffer(GL_ARRAY_BUFFER, VBO3);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_line), vertices_line, GL_STATIC_DRAW);


        }

        {
            static float f = 0.0f;
            static int counter = 0;

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

        glUseProgram(shaderProgram);
        glBindVertexArray(sphere_VAO); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_IBO);
        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
        glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);


        glUseProgram(shaderProgram);
        glBindVertexArray(VAO3);
        glDrawArrays(GL_LINES, 0, 2);

        glUseProgram(shaderProgram2);
        glBindVertexArray(VAO3);
        glDrawArrays(GL_POINTS, 0, 2);


        // if (intersection){
        //     glUseProgram(shaderProgram3);
        //     glBindVertexArray(VAO4);
        //     glDrawArrays(GL_POINTS, 0 , num_points);
        // }
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

    }

    // Cleanup
    cleanup(window);

    return 0;
}

glm::vec3 getTrackBallVector(double x, double y)
{
    glm::vec3 p = glm::vec3(2.0*x/width - 1.0, 2.0*y/height - 1.0, 0.0); //Normalize to [-1, +1]
    p.y = -p.y; //Invert Y since screen coordinate and OpenGL coordinates have different Y directions.

    float mag2 = p.x*p.x + p.y*p.y;
    if(mag2 <= 1.0f)
        p.z = sqrtf(1.0f - mag2);
    else
        p = glm::normalize(p); //Nearest point, close to the sides of the trackball
    return p;
}

// float calculate_t(glm::vec3 p, glm::vec3 d, glm::vec3 c){


//     return t;

// }
// bool raySphereIntersection(glm::vec3 p, glm::vec3 d, glm::vec3 c){

//     t = calculate_t(p, d, c);
//     check conditions

//     return Flag;
// }


