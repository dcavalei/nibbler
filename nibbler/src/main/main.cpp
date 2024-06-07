#include "game/App.h"

int main(int ac, char const *av[]) {
    auto app = game::App(ac, av);

    app.init();
    app.run();
    return EXIT_SUCCESS;
}
