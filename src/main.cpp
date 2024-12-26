#include "crow.h"
#include "routes.h"
#include "utilities.h"

int main() {
    crow::SimpleApp app;

    setupRoutes(app);

    app.port(18080).run();
}
