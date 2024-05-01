#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int NUM_BUBBLES = 20;
const float SPEED = 2.0f; // Speed of bubble movement

struct Bubble {
    float x, y;
    float radius;
    bool popped;
    bool hasColor;
    float r, g, b;
    float dx, dy; // Movement direction

    Bubble(float _x, float _y, float _radius, bool _hasColor = false, float _r = 1.0f, float _g = 1.0f, float _b = 1.0f)
        : x(_x), y(_y), radius(_radius), popped(false), hasColor(_hasColor), r(_r), g(_g), b(_b),
        dx((rand() % 100 - 50) / 50.0f), dy((rand() % 100 - 50) / 50.0f) {} // Random initial direction
};

std::vector<Bubble> bubbles;
int score = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        for (auto& bubble : bubbles) {
            float dx = mouseX - bubble.x;
            float dy = mouseY - bubble.y;
            float distance = sqrt(dx * dx + dy * dy);
            if (distance <= bubble.radius && !bubble.popped) {
                bubble.popped = true;
                if (bubble.hasColor) {
                    score += 5;
                }
                else {
                    score -= 1;
                }
            }
        }
    }
}

void init() {
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f); // Background color (dark blue)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, -1.0f, 1.0f);

    // Generate random bubbles
    srand(time(NULL));
    for (int i = 0; i < NUM_BUBBLES; ++i) {
        float radius = rand() % 30 + 20; // Random radius between 20 and 50
        int x = rand() % (int)(SCREEN_WIDTH - 2 * radius) + radius;
        // Random x-coordinate within screen bounds
        float y = rand() % (SCREEN_HEIGHT / 2) + radius; // Random y-coordinate within top half of the screen
        // Random color
        float r = (float)rand() / RAND_MAX;
        float g = (float)rand() / RAND_MAX;
        float b = (float)rand() / RAND_MAX;
        bool hasColor = rand() % 2 == 0; // Randomly assign whether bubble has color or not
        bubbles.push_back(Bubble(x, y, radius, hasColor, r, g, b));
    }
}

void update_bubbles() {
    for (auto& bubble : bubbles) {
        if (!bubble.popped) {
            bubble.x += bubble.dx * SPEED;
            bubble.y += bubble.dy * SPEED;
            // Boundary check
            if (bubble.x - bubble.radius < 0 || bubble.x + bubble.radius > SCREEN_WIDTH) {
                bubble.dx *= -1;
            }
            if (bubble.y - bubble.radius < 0 || bubble.y + bubble.radius > SCREEN_HEIGHT) {
                bubble.dy *= -1;
            }
        }
    }
}

void draw_bubble(float x, float y, float radius, float r, float g, float b) {
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(r, g, b); // Bubble color
    glVertex2f(x, y); // Center of the bubble
    for (int i = 0; i <= 360; i++) {
        float angle = 2 * 3.14159265359f * i / 360;
        float dx = radius * cos(angle);
        float dy = radius * sin(angle);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}

void draw_score(GLFWwindow* window, int score) {
    // Convert score to string
    std::string scoreStr = "Score: " + std::to_string(score);

    // Set position for drawing text
    float textX = 10.0f;
    float textY = SCREEN_HEIGHT - 20.0f;

    // Set the color for rendering text
    glColor3f(1.0f, 1.0f, 1.0f); // White color

    // Render text using GLFW's built-in functions
    glfwSetWindowTitle(window, scoreStr.c_str());
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    for (auto& bubble : bubbles) {
        if (!bubble.popped) {
            draw_bubble(bubble.x, bubble.y, bubble.radius, bubble.r, bubble.g, bubble.b);
        }
    }
    draw_score(glfwGetCurrentContext(), score);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bubble Pop Game", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Set the framebuffer size callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    init();

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Update bubbles' positions
        update_bubbles();

        // Render here
        draw();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
