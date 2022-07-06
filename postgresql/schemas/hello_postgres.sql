DROP SCHEMA IF EXISTS hello_postgres CASCADE;

CREATE SCHEMA IF NOT EXISTS hello_postgres;

CREATE TABLE IF NOT EXISTS hello_postgres.users (
    name TEXT PRIMARY KEY,
    count INTEGER DEFAULT(1),
);
