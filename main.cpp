#include <iostream>
#include <string>
#include <chrono>
#include <limits>
#include <vector>
#include <iomanip>
#include <cmath>
#include "GameObject.h"

#include "P6/PhysicsWorld.h"
#include "P6/RenderParticle.h"
#include "Camera/OrthoCamera.h"
#include "Camera/PerspectiveCamera.h"
#include "Randomization/RandomGen.h"


using namespace std::chrono_literals;
constexpr std::chrono::nanoseconds timestep(16ms);

int main(void)
{
    int numObj;
    std::cout << "Enter Number of Objects: ";
    std::cin >> numObj;

    GLFWwindow* window;
    float window_width = 700;
    float window_height = 700;

    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(window_width, window_height, "Phase 1 : Chang_Magaling", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    //OrthoCamera
    auto ortho_camera = new OrthoCamera();
    ortho_camera->setCameraPosition(glm::vec3(0.0f, 0.0f, 350.0f));
    ortho_camera->setCenter(glm::vec3(0.0f, 0.0f, 0.0f));

    //PerspectiveCamera
    auto pers_camera = new PerspectiveCamera();
    pers_camera->setCameraPosition(glm::vec3(0, 0.f, 350.f));
    //pers_camera->setCenter(glm::vec3(0.0f, 0.0f, 0.0f));

    auto pWorld = physics::PhysicsWorld();

    //white sphere
    auto sphere = GameObject("3D/sphere.obj", "Shaders/sample.vert", "Shaders/sample.frag");

    glViewport(0, 0, 700, 700);

    glm::mat4 identity_matrix = glm::mat4(1.0f);


    glm::mat4 projection_matrix = pers_camera->getViewProjection();
    //glm::mat4 projection_matrix = ortho_camera->getViewProjection();

    // Initialize RenderParticles
    std::list<RenderParticle*> RenderParticles;
    std::chrono::nanoseconds spawnInterval = 200ms; // Time interval between spawns
    std::chrono::nanoseconds spawnTimer = 0ms;     // Timer to control when to spawn next particle


    /*physics::PhysicsParticle p1 = physics::PhysicsParticle();
    p1.Position = physics::MyVector(0, 0, 0);
    p1.mass = 1;
    p1.AddForce(physics::MyVector(RandomGen().RandomizeForce(), RandomGen().RandomizeForce(), RandomGen().RandomizeForce()));
    pWorld.AddParticle(&p1);

    RenderParticle rp1 = RenderParticle(&p1, &sphere, glm::vec4(RandomGen().RandomizeColor(), RandomGen().RandomizeColor(), RandomGen().RandomizeColor(), 1.0f));

    RenderParticles.push_back(&rp1);*/

    for (int i = 0; i < numObj; i++) {
        physics::PhysicsParticle* p = new physics::PhysicsParticle();
        p->Position = physics::MyVector(0, -100, 0);
        p->mass = 1;
        p->AddForce(physics::MyVector(RandomGen().RandomizeForce(), RandomGen().RandomizeYForce(), RandomGen().RandomizeForce()));
        pWorld.AddParticle(p);

        RenderParticle* rp = new RenderParticle(p, &sphere, glm::vec4(RandomGen().RandomizeColor(), RandomGen().RandomizeColor(), RandomGen().RandomizeColor(), 1.0f));
        RenderParticles.push_back(rp);
    }

    std::vector<std::chrono::time_point<std::chrono::high_resolution_clock>> startTime(4, clock::now());
    std::vector<std::chrono::time_point<std::chrono::high_resolution_clock>> endTime(4);

    auto start_time = clock::now();

    bool resultPrinted = false;
    bool all_finished = false;

    glm::mat4 view_matrix = glm::mat4(1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        curr_time = clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds> (curr_time - prev_time);
        prev_time = curr_time;

        curr_ns += dur;

        if (curr_ns >= timestep) {
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_ns);
            curr_ns -= curr_ns;

            //glm::mat4 projection_matrix = pers_camera->getViewProjection();
            glm::mat4 projection_matrix = ortho_camera->getViewProjection();
            
            ortho_camera->Update(window, (float)ms.count() / 1000);

            pWorld.Update((float)ms.count() / 1000);

            // Update spawn timer
            ////spawnTimer += dur;
            // 
            // 
            //std::cout << spawnTimer.count() << std::endl;
            // 
            // Spawn new RenderParticle if spawn interval has passed
            //if (spawnTimer >= spawnInterval)
            //{
            //    physics::PhysicsParticle* particle = new physics::PhysicsParticle();
            //    particle->Position = physics::MyVector(0, 0, 0);
            //    particle->mass = 1;
            //    particle->AddForce(physics::MyVector(0, 1000.f, 0));
            //    particle->AddLifeSpan();
            //    pWorld.AddParticle(particle);

            //    RenderParticle* renderParticle = new RenderParticle(particle, &sphere, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
            //    RenderParticles.push_back(renderParticle);

            //    // Reset spawn timer
            //    spawnTimer = 0ms;
            //}
        }

        for (std::list<RenderParticle*>::iterator i = RenderParticles.begin(); i != RenderParticles.end(); i++) {
            (*i)->Draw(identity_matrix, projection_matrix, ortho_camera->GetViewMatrix());
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}