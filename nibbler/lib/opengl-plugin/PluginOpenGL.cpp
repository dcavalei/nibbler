#include "PluginOpenGL.h"

#include <GL/freeglut.h>
#include <spdlog/spdlog.h>

#include <future>
#include <sstream>

namespace interface {
    //=====================================================================================================================
    // Global variables
    //=====================================================================================================================

    std::unordered_map<interface::Input, interface::Callback> g_callbacks;  // TODO: check

    //=====================================================================================================================
    // PluginOpenGL
    //=====================================================================================================================
    std::string PluginOpenGL::greet() { return "Hello from plugin!"; }

    // --------------------------------------------------------------------------------------
    void PluginOpenGL::register_cb(Input input, Callback sig) {
        if (!sig && g_callbacks.contains(input)) {
            SPDLOG_DEBUG("Unregister signal: {}", toString(input));
            g_callbacks.erase(input);
        } else if (!sig) {
            SPDLOG_WARN("Cannot register Input {} with a non-callable signal! Skipping...", toString(input));
        } else {
            SPDLOG_DEBUG("Register signal: {}", toString(input));
            g_callbacks[input] = sig;
        }
    }

    // --------------------------------------------------------------------------------------
    void display() {
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_POLYGON);
        glVertex3f(0.5, 0.0, 0.5);
        glVertex3f(0.5, 0.0, 0.0);
        glVertex3f(0.0, 0.5, 0.0);
        glVertex3f(0.0, 0.0, 0.5);
        glEnd();
        glFlush();

        glClear(GL_COLOR_BUFFER_BIT);

        glutSwapBuffers();
    }

    // --------------------------------------------------------------------------------------
    void timer([[maybe_unused]] int value) {
        glutPostRedisplay();
        glutTimerFunc(100, timer, 0);  // 100 ms timer
    }

    // --------------------------------------------------------------------------------------
    void keyHandler(unsigned char key, int, int) {
        switch (key) {
            case 13: g_callbacks[Input::ENTER](); break;
            case 27: g_callbacks[Input::ESC](); break;
            case 49: g_callbacks[Input::ONE](); break;
            case 50: g_callbacks[Input::TWO](); break;
            case 51: g_callbacks[Input::THREE](); break;
            case 97: g_callbacks[Input::A](); break;
            case 115: g_callbacks[Input::S](); break;
            case 100: g_callbacks[Input::D](); break;
            case 119: g_callbacks[Input::W](); break;
            default: SPDLOG_DEBUG("Key number: {}", key);
        }
    }

    // --------------------------------------------------------------------------------------
    void PluginOpenGL::display_menu(const interface::Menu &) {}

    // --------------------------------------------------------------------------------------
    void PluginOpenGL::display_game(std::vector<ARGB> const &) {}

    //=====================================================================================================================
    // Static initialization
    //=====================================================================================================================
    static int ac = 1;
    static char const *av[] = {"nibbler", nullptr};

    // --------------------------------------------------------------------------------------
    void PluginOpenGL::entrypoint(std::future<void> future) {
        SPDLOG_INFO("PluginOpenGL starting...");
        glutInit(&ac, (char **)av);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        glutInitWindowSize(800, 600);
        glutCreateWindow("Snake Game");
        glutDisplayFunc(display);

        glutKeyboardFunc(keyHandler);
        glutTimerFunc(100, timer, 0);
        glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
        glEnable(GL_DEPTH_TEST);
        future.wait();  // wait for the main process
        glutMainLoop();
        SPDLOG_DEBUG("PluginOpenGL exiting loop...");
    }

    // --------------------------------------------------------------------------------------
    std::future<void> PluginOpenGL::request_shutdown() {
        SPDLOG_INFO("PluginOpenGL graceful shutdown...");
        std::promise<void> promise;
        auto future = promise.get_future();

        std::thread([p = std::move(promise)]() mutable {
            SPDLOG_DEBUG("PluginOpenGL starting shutdown worker...");
            glutLeaveMainLoop();
            std::this_thread::sleep_for(std::chrono::milliseconds(300));  // TODO: glut needs some time to exit...
            p.set_value_at_thread_exit();
            SPDLOG_DEBUG("PluginOpenGL exiting shutdown worker...");
        }).detach();
        return future;
    }

    //=====================================================================================================================
    // extern "C"
    //=====================================================================================================================
    extern "C" {

    // --------------------------------------------------------------------------------------
    IPlugin *create() { return new PluginOpenGL; }

    // --------------------------------------------------------------------------------------
    void destroy(IPlugin *p) { delete p; }
    }
}  // namespace interface
