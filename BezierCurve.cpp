#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <iostream>

struct Point {
    float x, y;
};

std::vector<Point> controlPoints = {
        {100, 100},
        {200, 300},
        {400, 300},
        {500, 100}
};

int selectedPoint = -1;
const float PICK_RADIUS = 10.0f;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Simple linear interpolation
Point lerp(const Point& p0, const Point& p1, float t) {
    return { (1 - t) * p0.x + t * p1.x, (1 - t) * p0.y + t * p1.y };
}

// De Casteljau's Algorithm for Bezier
Point deCasteljau(const std::vector<Point>& points, float t) {
    std::vector<Point> temp = points;
    while (temp.size() > 1) {
        std::vector<Point> next;
        for (size_t i = 0; i < temp.size() - 1; ++i) {
            next.push_back(lerp(temp[i], temp[i + 1], t));
        }
        temp = next;
    }
    return temp[0];
}

void drawCurve() {
    glColor3f(0, 0, 1);
    glBegin(GL_LINE_STRIP);
    for (float t = 0; t <= 1.001; t += 0.01) {
        Point p = deCasteljau(controlPoints, t);
        glVertex2f(p.x, p.y);
    }
    glEnd();
}

void drawControlPoints() {
    glPointSize(10);
    glBegin(GL_POINTS);
    for (const auto& p : controlPoints) {
        glColor3f(1, 0, 0);
        glVertex2f(p.x, p.y);
    }
    glEnd();
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    ypos = WINDOW_HEIGHT - ypos; // OpenGL has origin at bottom-left

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            for (size_t i = 0; i < controlPoints.size(); ++i) {
                float dx = xpos - controlPoints[i].x;
                float dy = ypos - controlPoints[i].y;
                if (dx * dx + dy * dy < PICK_RADIUS * PICK_RADIUS) {
                    selectedPoint = i;
                    break;
                }
            }
        } else if (action == GLFW_RELEASE) {
            selectedPoint = -1;
        }
    }
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    ypos = WINDOW_HEIGHT - ypos; // Flip Y axis

    if (selectedPoint != -1) {
        controlPoints[selectedPoint].x = xpos;
        controlPoints[selectedPoint].y = ypos;
    }
}

int main() {
    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Bezier Curve Editor", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);

        drawControlPoints();
        drawCurve();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
