#include "PluginOpenGL.h"

#include <GL/glut.h>
#include <spdlog/spdlog.h>
#include <termios.h>

#include <sstream>

namespace interface {
    std::unordered_map<interface::Input, interface::signal> g_callbacks;  // TODO: check

    std::string PluginOpenGL::greet() { return "Hello from plugin!"; }
    void PluginOpenGL::register_callback(Input input, signal sig) {
        if (!sig && g_callbacks.contains(input)) {
            spdlog::debug("[register_callback] Unregister signal: {}", toString(input));
            g_callbacks.erase(input);
        } else if (!sig) {
            spdlog::warn("Cannot register Input {} with a non-callable signal! Skipping...", toString(input));
        } else {
            spdlog::debug("[register_callback] Register signal: {}", toString(input));
            g_callbacks[input] = sig;
        }
    }
    void display() {
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_POLYGON);
        glVertex3f(0.5, 0.0, 0.5);
        glVertex3f(0.5, 0.0, 0.0);
        glVertex3f(0.0, 0.5, 0.0);
        glVertex3f(0.0, 0.0, 0.5);
        glEnd();
        glFlush();
    }
    void timer(int value) {
        glutPostRedisplay();
        glutTimerFunc(100, timer, 0);  // 100 ms timer
    }
    void keyHandler(unsigned char key, int x, int y) {
        spdlog::debug("key: {}, x: {}, y: {}", key, x, y);
        if (key == 27) {
            g_callbacks[Input::ESC]();
        } else if (1) {
        }
    }
    void PluginOpenGL::display_menu(const interface::IMenu &) {}

    void PluginOpenGL::setup_completed() {
        static auto t = std::thread([]() {  // this is a bs for dev purposes
            static int ac = 1;
            static char const *av[] = {"nibbler", nullptr};
            glutInit(&ac, (char **)av);
            glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
            glutInitWindowSize(800, 600);
            glutCreateWindow("Snake Game");
            glutDisplayFunc(display);

            glutKeyboardFunc(keyHandler);
            glutTimerFunc(100, timer, 0);

            glEnable(GL_DEPTH_TEST);
            glutMainLoop();
        });
        spdlog::critical("OKKKKKKKKKKKKK");
    }

    extern "C" {
    IPlugin *create() { return new PluginOpenGL; }
    void destroy(IPlugin *p) { delete p; }
    }
}  // namespace interface
