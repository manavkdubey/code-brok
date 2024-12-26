#include "routes.h"
#include "../core/add.h"
#include "../core/db.h"

void setupRoutes(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/add/<int>/<int>")
    ([](int a, int b) {
        int result = add(a, b); 
        return crow::json::wvalue{{"result", result}}; 
    });

    CROW_ROUTE(app, "/hello")
    ([]() {
        return "Hello from routes!";
    });
    CROW_ROUTE(app, "/db/setup")
    ([]() {
        try {
            setupDatabase();
            return crow::response(200, "Database setup complete");
        } catch (const std::exception& e) {
            return crow::response(500, e.what());
        }
    });

    // Insert a new user
    CROW_ROUTE(app, "/db/add_user/<string>")
    ([](const std::string& name) {
        try {
            return crow::response(200, insertUser(name));
        } catch (const std::exception& e) {
            return crow::response(500, e.what());
        }
    });

    // Get all users
    CROW_ROUTE(app, "/db/get_users")
    ([]() {
        try {
            return crow::response(200, getAllUsers());
        } catch (const std::exception& e) {
            return crow::response(500, e.what());
        }
    });
}
