#include "db.h"
#include "libsql.h"
#include <iostream>
#include <sstream>

void setupDatabase() {
    const char* url = getenv("TURSO_DATABASE_URL");
    const char* auth_token = getenv("TURSO_AUTH_TOKEN");

    if (!url || !auth_token) {
        throw std::runtime_error("Environment variables TURSO_DATABASE_URL and TURSO_AUTH_TOKEN must be set.");
    }

    libsql_database_t db = libsql_database_init((libsql_database_desc_t){.url = url, .auth_token = auth_token});
    if (db.err) {
        throw std::runtime_error(std::string("Error initializing database: ") + libsql_error_message(db.err));
    }

    libsql_connection_t conn = libsql_database_connect(db);
    if (conn.err) {
        throw std::runtime_error(std::string("Error connecting to database: ") + libsql_error_message(conn.err));
    }

    const char* setup_sql =
        "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT);";
    libsql_batch_t batch = libsql_connection_batch(conn, setup_sql);
    if (batch.err) {
        throw std::runtime_error(std::string("Error setting up database: ") + libsql_error_message(batch.err));
    }

    libsql_connection_deinit(conn);
    libsql_database_deinit(db);
}

std::string insertUser(const std::string& name) {
    const char* url = getenv("TURSO_DATABASE_URL");
    const char* auth_token = getenv("TURSO_AUTH_TOKEN");

    libsql_database_t db = libsql_database_init((libsql_database_desc_t){.url = url, .auth_token = auth_token});
    libsql_connection_t conn = libsql_database_connect(db);

    libsql_statement_t stmt = libsql_connection_prepare(conn, "INSERT INTO users (name) VALUES (?);");
    if (stmt.err) {
        throw std::runtime_error(std::string("Error preparing statement: ") + libsql_error_message(stmt.err));
    }

    libsql_statement_bind_value(stmt, libsql_text(name.c_str(), name.size()));
    libsql_execute_t result = libsql_statement_execute(stmt);
    if (result.err) {
        throw std::runtime_error(std::string("Error inserting user: ") + libsql_error_message(result.err));
    }

    libsql_statement_deinit(stmt);
    libsql_connection_deinit(conn);
    libsql_database_deinit(db);

    return "User inserted successfully";
}

std::string getAllUsers() {
    const char* url = getenv("TURSO_DATABASE_URL");
    const char* auth_token = getenv("TURSO_AUTH_TOKEN");

    libsql_database_t db = libsql_database_init((libsql_database_desc_t){.url = url, .auth_token = auth_token});
    libsql_connection_t conn = libsql_database_connect(db);

    libsql_statement_t stmt = libsql_connection_prepare(conn, "SELECT * FROM users;");
    if (stmt.err) {
        throw std::runtime_error(std::string("Error preparing query: ") + libsql_error_message(stmt.err));
    }

    libsql_rows_t rows = libsql_statement_query(stmt);
    if (rows.err) {
        throw std::runtime_error(std::string("Error executing query: ") + libsql_error_message(rows.err));
    }

    std::ostringstream result;
    libsql_row_t row;
    while (!(row = libsql_rows_next(rows)).err && !libsql_row_empty(row)) {
        libsql_result_value_t id = libsql_row_value(row, 0);
        libsql_result_value_t name = libsql_row_value(row, 1);

        if (id.err || name.err) {
            continue;
        }

        result << id.ok.value.integer << ": " << (char*)name.ok.value.text.ptr << "\n";
        libsql_row_deinit(row);
    }

    libsql_rows_deinit(rows);
    libsql_statement_deinit(stmt);
    libsql_connection_deinit(conn);
    libsql_database_deinit(db);

    return result.str();
}
