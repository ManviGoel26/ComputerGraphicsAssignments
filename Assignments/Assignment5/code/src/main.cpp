// Assignment 03: Raytracing

#include "imgui_setup.h"
#include "camera.h"
#include "renderengine.h"
#include "world.h"
#include "material.h"
#include "object.h"
#include "sphere.h"
#include "lightsource.h"
#include "pointlightsource.h"
#include "triangle.h"
#include "cylinder.h"   
#include "transformed.h"
#include "implicitSurface.h"
#include <typeinfo>


#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../depends/stb/stb_image.h"
#include "../depends/stb/stb_image_write.h"

#define RENDER_BATCH_COLUMNS 20 // Number of columns to render in a single go. Increase to gain some display/render speed!

Camera *camera;
RenderEngine *engine;

int screen_width = 800, screen_height = 600; // This is window size, used to display scaled raytraced image.
int image_width = 1920, image_height = 1080; // This is raytraced image size. Change it if needed.
GLuint texImage;

bool load_image(std::vector<unsigned char>& image, const std::string& filename, int& x, int&y)
{
    int n;
    unsigned char* data = stbi_load(filename.c_str(), &x, &y, &n, 0);
    // std::cout << n << "\n";
    if (data != nullptr)
    {
        image = std::vector<unsigned char>(data, data + x*y*3);
    }
    stbi_image_free(data);
    return (data != nullptr);
}

int main(int, char**)
{
    // Setup window
    GLFWwindow *window = setupWindow(screen_width, screen_height);

    ImVec4 clearColor = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);

    // Setup raytracer camera. This is used to spawn rays.
    Vector3D camera_position(0, 0, 10);
    Vector3D camera_target(0, 0, 0); //Looking down -Z axis
    Vector3D camera_up(0, 1, 0);
    float camera_fovy =  45;
    camera = new Camera(camera_position, camera_target, camera_up, camera_fovy, image_width, image_height);

    //Create a world
    World *world = new World;
    world->setAmbient(Color(1));
    world->setBackground(Color(0.1, 0.3, 0.6));
    Material *m = new Material(world);
    m->color = Color(0.0, 1.0, 1.0);
    m->ka = 1;
    m->kd = 0.07;
    m->n = 4.0;
    m->ks = 0.6;
    // m->kt = 1.0;

    Material *m2 = new Material(world);
    m2->color = Color(0.1, 0.2, 0.6);
    // m2->ka = 0.5;
    // m2->kd = 0.07;
    // m2->n = 64.0;
    // m2->ks = 0.8;
    m2->kr = 0.6;
    // m2->kt = 1;
    // m2->eta = 1.52;


    // // triangle only rendered
    // Object *triangle1 = new Triangle(Vector3D(-1,0,2),Vector3D(0,3,1), Vector3D(5,0,-1),m2); 
    // world->addObject(triangle1);

    // Object *triangle2 = new Triangle(Vector3D(-5,-2,1),Vector3D(-3,4,-5), Vector3D(5,-3,-1),m); 
    // world->addObject(triangle2);


    
    // read_jpeg_header()
    // Implicit Surface 
    // Object *surface = new ImplicitSurface(m3);
    // world->addObject(surface);
    // LightSource *light1 = new PointLightSource(world, Vector3D(0, 0, 0), Color(9, 1, 1));
    // world->addLight(light1);
    std::string filename = "src/texMap.jpg";
    
    int width, height;
    std::vector<unsigned char> image;
    bool success = load_image(image, filename, width, height);
    if (!success)
    {
        std::cout << "Error loading image\n";
        return 1;
    }

    Material *m3 = new Material(world);
    m3->color = Color(0.2, 1.0, 1.0);
    m3->ka = 0.3;
    // m3->kt = 1;
    // m3->eta = 1.53;
    m3->kd = 0.2;
    m3->ks = 0.5;
    m3->image = image;
    m3->im_width = width;
    m3->im_height = height;
    
    // Sphere
    Object *sphere = new Sphere(Vector3D(-1, 0, 0), 1, m3);
    world->addObject(sphere);
    
    
    // std::cout << "Image width = " << width << '\n';
    // std::cout << "Image height = " << height << '\n';
    

    LightSource *light2 = new PointLightSource(world, Vector3D(5, 10, 10), Color(1, 1, 1));
    world->addLight(light2);

    engine = new RenderEngine(world, camera);

    //Initialise texture
    glGenTextures(1, &texImage);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, camera->getBitmap());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        bool render_status;
        for(int i=0; i<RENDER_BATCH_COLUMNS; i++) 
            render_status = engine->renderLoop(); // RenderLoop() raytraces 1 column of pixels at a time.
        if(!render_status)
        {
            // Update texture
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texImage);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image_width, image_height, GL_RGB, GL_UNSIGNED_BYTE, camera->getBitmap());
        } 

        ImGui::Begin("Lumina", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Size: %d x %d", image_width, image_height);
        if(ImGui::Button("Save")){
          char filename[] = "img.png";
          stbi_write_png(filename, image_width, image_height, 3, camera->getBitmap(),0);        
        }
        //Display render view - fit to width
        int win_w, win_h;
        glfwGetWindowSize(window, &win_w, &win_h);
        float image_aspect = (float)image_width/(float)image_height;
        float frac = 0.95; // ensure no horizontal scrolling
        ImGui::Image((void*)(intptr_t)texImage, ImVec2(frac*win_w, frac*win_w/image_aspect), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    glDeleteTextures(1, &texImage);

    cleanup(window);

    return 0;
}
