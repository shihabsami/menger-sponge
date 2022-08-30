#include "../App/App.h"

int main() {
    std::unique_ptr<App> app = std::make_unique<App>("Menger Sponge", true);
    if (app->init() != APP_SUCCESS) {
        fprintf(stderr, "ERROR: Failed to initialise application.\n");
        return EXIT_FAILURE;
    }

    app->run();
    app->done();

    return EXIT_SUCCESS;
}
