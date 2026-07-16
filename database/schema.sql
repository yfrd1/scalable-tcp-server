CREATE DATABASE IF NOT EXISTS server_db
    CHARACTER SET utf8mb4
    COLLATE utf8mb4_unicode_ci;

USE server_db;

CREATE TABLE users
(
    id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) NOT NULL UNIQUE,
    password_hash VARCHAR(255) NOT NULL,
    role TINYINT UNSIGNED NOT NULL DEFAULT 1,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    last_login_at TIMESTAMP NULL DEFAULT NULL,
    is_active BOOLEAN NOT NULL DEFAULT TRUE
);

CREATE TABLE files
(
    id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    user_id BIGINT UNSIGNED NOT NULL,
    original_name VARCHAR(255) NOT NULL,
    stored_name VARCHAR(255) NOT NULL UNIQUE,
    file_size BIGINT UNSIGNED NOT NULL,
    mime_type VARCHAR(100) NOT NULL,
    upload_time TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,

    CONSTRAINT fk_files_user
        FOREIGN KEY (user_id)
        REFERENCES users(id)
        ON DELETE CASCADE
);

CREATE TABLE login_sessions
(
    session_id      CHAR(64) NOT NULL,
    user_id         BIGINT UNSIGNED NOT NULL,
    created_at      DATETIME NOT NULL,
    expires_at      DATETIME NOT NULL,
    last_access_at  DATETIME NOT NULL,
    ip_address      VARCHAR(45),

    PRIMARY KEY (session_id),

    INDEX idx_login_sessions_user_id (user_id),
    INDEX idx_login_sessions_expires_at (expires_at),

    CONSTRAINT fk_login_sessions_user
        FOREIGN KEY (user_id)
        REFERENCES users(id)
        ON DELETE CASCADE
)
ENGINE = InnoDB
DEFAULT CHARSET = utf8mb4
COLLATE = utf8mb4_unicode_ci;