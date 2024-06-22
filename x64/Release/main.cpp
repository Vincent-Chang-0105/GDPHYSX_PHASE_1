#include <iostream>
#include <string>
#include <chrono>
#include <limits>
#include <vector>
#include <iomanip>
#include <cmath>
#include "GameObject.h"

using namespace std::chrono_literals;
constexpr std::chrono::nanoseconds timestep(16ms);

static std::string ordinals(int num) {
    switch (num % 10) {
    case 1:
        return "st";

    case 2:
        return "nd";

    case 3:
        return "rd";

    default:
        return "th";
    }
}

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

    window = glfwCreateWindow(window_width, window_height, "Abegail Laureen R. Magaling", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    GameObject* green_sphere = new GameObject("3D/sphere.obj", "Shaders/sample.vert", "Shaders/sample.frag", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    GameObject* red_sphere = new GameObject("3D/sphere.obj", "Shaders/sample.vert", "Shaders/sample.frag", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    GameObject* blue_sphere = new GameObject("3D/sphere.obj", "Shaders/sample.vert", "Shaders/sample.frag", glm::vec4(0.f, 0.f, 1.f, 1.0f));
    GameObject* yellow_sphere = new GameObject("3D/sphere.obj", "Shaders/sample.vert", "Shaders/sample.frag", glm::vec4(1.f, 0.7f, 0.f, 1.0f));

    glViewport(0, 0, 700, 700);

    glm::mat4 identity_matrix = glm::mat4(1.0f);

    glm::mat4 projection_matrix = glm::ortho(
        -350.f, //L
        350.f,//R
        -350.f,//B
        350.f,//T
        -350.f,//Znear
        350.f);//Zfar

    physics::MyVector scale(50, 50, 50);

    std::vector<physics::PhysicsParticle> particles = { physics::PhysicsParticle(), physics::PhysicsParticle(), physics::PhysicsParticle(), physics::PhysicsParticle() };

    particles[0].Position = physics::MyVector(200, 200, 173);
    particles[0].Velocity = particles[0].Position.direction().ScalarMultiplication(90.f).ScalarMultiplication(-1.f);
    particles[0].Acceleration = particles[0].Position.direction().ScalarMultiplication(8.f).ScalarMultiplication(-1.f);

    particles[1].Position = physics::MyVector(-200, 200, 201);
    particles[1].Velocity = particles[1].Position.direction().ScalarMultiplication(80.f).ScalarMultiplication(-1.f);
    particles[1].Acceleration = particles[1].Position.direction().ScalarMultiplication(14.5f).ScalarMultiplication(-1.f);

    particles[2].Position = physics::MyVector(200, -200, -300);
    particles[2].Velocity = particles[2].Position.direction().ScalarMultiplication(130.f).ScalarMultiplication(-1.f);
    particles[2].Acceleration = particles[2].Position.direction().ScalarMultiplication(1.f).ScalarMultiplication(-1.f);

    particles[3].Position = physics::MyVector(-200, -200, -150);
    particles[3].Velocity = particles[3].Position.direction().ScalarMultiplication(110.f).ScalarMultiplication(-1.f);
    particles[3].Acceleration = particles[3].Position.direction().ScalarMultiplication(3.f).ScalarMultiplication(-1.f);

    std::vector<std::chrono::time_point<std::chrono::high_resolution_clock>> startTime(4, clock::now());
    std::vector<std::chrono::time_point<std::chrono::high_resolution_clock>> endTime(4);

    std::vector<bool> finished(4, false);
    std::vector<float> MagVelocity(4, 0.0f);

    std::vector<physics::MyVector> initialVelocities = {
    particles[0].Velocity,
    particles[1].Velocity,
    particles[2].Velocity,
    particles[3].Velocity
    };

    std::vector<GameObject*> gameObjects = { green_sphere, red_sphere, blue_sphere, yellow_sphere };

    auto start_time = clock::now();

    bool resultPrinted = false;
    bool all_finished = false;

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
            all_finished = true;

            for (int i = 0; i < particles.size(); i++) {
                if (!finished[i]) {

                    particles[i].update((float)ms.count() / 1000);
                    int pos_x = particles[i].Position.x;
                    int pos_y = particles[i].Position.y;
                    int pos_z = particles[i].Position.z;

                    if (pos_x == 0 && pos_y == 0 && pos_z == 0) {
                        finished[i] = true;
                        endTime[i] = clock::now();
                        MagVelocity[i] = particles[i].Velocity.Magnitude();
                    }
                    else {
                        all_finished = false;
                    }
                }
            }

            if (all_finished && !resultPrinted) {
                std::vector<int> index = { 0, 1, 2, 3 };

                for (int i = 0; i < index.size() - 1; i++) {
                    for (int j = 0; j < index.size() - i - 1; j++) {
                        auto time_diff_a = std::chrono::duration_cast<std::chrono::milliseconds>(endTime[index[j]] - startTime[index[j]]);
                        auto time_diff_b = std::chrono::duration_cast<std::chrono::milliseconds>(endTime[index[j + 1]] - startTime[index[j + 1]]);

                        if (time_diff_a.count() > time_diff_b.count()) {
                            std::swap(index[j], index[j + 1]);
                        }
                    }
                }

                int rank = 1;
                for (int i : index) {
                    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime[i] - startTime[i]).count();

                    float avgVelocityX = (initialVelocities[i].x + particles[i].Velocity.x) / 2.0f;
                    float avgVelocityY = (initialVelocities[i].y + particles[i].Velocity.y) / 2.0f;
                    float avgVelocityZ = (initialVelocities[i].z + particles[i].Velocity.z) / 2.0f;

                    std::cout << rank << ordinals(rank) << " : ";
                    if (gameObjects[i]->color == glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)) {
                        std::cout << "Green" << std::endl;
                    }
                    else if(gameObjects[i]->color == glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)) {
                        std::cout << "Red" << std::endl;
                    }
                    else if (gameObjects[i]->color == glm::vec4(0.f, 0.f, 1.f, 1.0f)) {
                        std::cout << "Blue" << std::endl;
                    }
                    else if (gameObjects[i]->color == glm::vec4(1.f, 0.7f, 0.f, 1.0f)) {
                        std::cout << "Yellow" << std::endl;
                    }

                    std::cout << "Mag. of Velocity: " << std::fixed << std::setprecision(2) << MagVelocity[i] << " m/s" << std::endl;
                    std::cout << "Average Velocity: (" << std::fixed << std::setprecision(2) << avgVelocityX << ", " << avgVelocityY << ", " << avgVelocityZ << ") m/s" << std::endl;
                    std::cout << static_cast<float>(elapsed / 1000.f) << " secs" << std::endl;
                    std::cout << "\n";
                    rank++;
                }
                resultPrinted = true;
            }

        }

        for (int i = 0; i < particles.size(); i++) {
            gameObjects[i]->Draw(identity_matrix, projection_matrix, particles[i].Position);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}