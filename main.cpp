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


using namespace std::chrono_literals;
constexpr std::chrono::nanoseconds timestep(16ms);

int main(void)
{

    GLFWwindow* window;
    float window_width = 700;
    float window_height = 700;

    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(window_width, window_height, "PCO1 Abegail Laureen R. Magaling", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    //OrthoCamera
    auto ortho_camera = new OrthoCamera();
    ortho_camera->setCameraPosition(glm::vec3(0.0f, 0.0f, 30.0f));
    ortho_camera->setCenter(glm::vec3(0.0f, 0.0f, 0.0f));

    //PerspectiveCamera
    auto pers_camera = new PerspectiveCamera();
    pers_camera->setCameraPosition(glm::vec3(0, 0.f, 30.f));
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
    std::chrono::nanoseconds spawnInterval = 20ms; // Time interval between spawns
    std::chrono::nanoseconds spawnTimer = 0ms;     // Timer to control when to spawn next particle


    physics::PhysicsParticle p1 = physics::PhysicsParticle();
    p1.Position = physics::MyVector(0, 0, 0);
    p1.mass = 1;
    p1.AddForce(physics::MyVector(0, 50, 0));
    pWorld.AddParticle(&p1);

    physics::PhysicsParticle p2 = physics::PhysicsParticle();
    p2.Position = physics::MyVector(-200, 200, 201);
    p2.Velocity = p2.Position.direction().ScalarMultiplication(-80.f);
    p2.Acceleration = p2.Position.direction().ScalarMultiplication(-14.f);
    pWorld.AddParticle(&p2);

    /*RenderParticle rp1 = RenderParticle(&p1, &sphere, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

    RenderParticles.push_back(&rp1);*/

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
            spawnTimer += dur;
            std::cout << spawnTimer.count() << std::endl;
            // Spawn new RenderParticle if spawn interval has passed
            if (spawnTimer >= spawnInterval)
            {
                physics::PhysicsParticle* particle = new physics::PhysicsParticle();
                particle->Position = physics::MyVector(0, 0, 0);
                particle->mass = 1;
                particle->AddForce(physics::MyVector(0, 1000.f, 0));
                pWorld.AddParticle(particle);

                RenderParticle* renderParticle = new RenderParticle(particle, &sphere, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
                RenderParticles.push_back(renderParticle);

                // Reset spawn timer
                spawnTimer = 0ms;
            }
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