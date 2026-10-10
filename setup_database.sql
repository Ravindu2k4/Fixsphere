-- ==============================================================================
-- FixSphere — Mobile Phone Repair Center Management System
-- Standalone Database Setup Script
-- Compatible with: MySQL Server 8.0+ / MySQL Workbench
-- ==============================================================================

-- 1. Create and select the database
CREATE DATABASE IF NOT EXISTS fixsphere;
USE fixsphere;

-- 2. Drop existing tables in reverse dependency order (if fresh installation is desired)
-- Uncomment the following lines if you want to wipe and re-initialize completely:
-- DROP TABLE IF EXISTS repair_jobs;
-- DROP TABLE IF EXISTS customers;
-- DROP TABLE IF EXISTS employees;

-- ------------------------------------------------------------------------------
-- Table 1: employees
-- ------------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS employees (
    emp_id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    nic VARCHAR(12) NOT NULL UNIQUE,
    contact VARCHAR(10) NOT NULL,
    address VARCHAR(255) NOT NULL,
    role VARCHAR(10) NOT NULL, -- 'Admin', 'Checker', 'Repairer'
    is_active TINYINT(1) NOT NULL DEFAULT 1,
    username VARCHAR(50) NOT NULL UNIQUE,
    password_hash CHAR(64) NOT NULL, -- SHA-256 password hash
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ------------------------------------------------------------------------------
-- Table 2: customers
-- ------------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS customers (
    customer_id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    contact VARCHAR(10) NOT NULL,
    nic VARCHAR(12) NOT NULL UNIQUE,
    address VARCHAR(255) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ------------------------------------------------------------------------------
-- Table 3: repair_jobs
-- ------------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS repair_jobs (
    job_id INT AUTO_INCREMENT PRIMARY KEY,
    customer_id INT NULL,
    customer_name VARCHAR(100) NOT NULL,
    customer_contact VARCHAR(10) NOT NULL,
    customer_nic VARCHAR(12) NOT NULL,
    customer_address VARCHAR(255) NOT NULL,
    phone_model VARCHAR(100) NOT NULL,
    details TEXT NOT NULL,
    status VARCHAR(20) NOT NULL DEFAULT 'Pending', -- 'Pending', 'In Progress', 'Finished', 'Couldn''t Repair'
    cost DECIMAL(10,2) NOT NULL DEFAULT 0.00,
    created_by INT NULL,
    repairer_id INT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    finished_at DATETIME NULL,
    CONSTRAINT fk_jobs_customer FOREIGN KEY (customer_id) REFERENCES customers(customer_id) ON DELETE SET NULL,
    CONSTRAINT fk_jobs_created_by FOREIGN KEY (created_by) REFERENCES employees(emp_id) ON DELETE SET NULL,
    CONSTRAINT fk_jobs_repairer FOREIGN KEY (repairer_id) REFERENCES employees(emp_id) ON DELETE SET NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ------------------------------------------------------------------------------
-- Seed Default Employee Accounts (if not already existing)
-- Passwords are encrypted using SHA-256:
--   'admin123'  -> 240be518fabd2724ddb6f04eeb1da5967448d7e831c08c8fa822809f74c720a9
--   'check123'  -> c698a7a75166097919ec0be142eca8f074c6a6ee0ef767553613c07da8ea271c
--   'repair123' -> 91bda28a40ec2eadea4b9ef5d3d5c2f06d552117e3cdf32baebe2f92ff5f16fc
-- ------------------------------------------------------------------------------
INSERT IGNORE INTO employees (emp_id, name, nic, contact, address, role, is_active, username, password_hash)
VALUES
(1, 'Administrator', '199000000000', '0770000000', 'Anuradhapura', 'Admin', 1, 'admin', SHA2('admin123', 256)),
(2, 'Kamal Perera',   '200012345678', '0771234567', 'Anuradhapura', 'Checker', 1, 'checker', SHA2('check123', 256)),
(3, 'Nimal Silva',    '199912345678', '0712345678', 'Mihintale',    'Repairer', 1, 'repairer', SHA2('repair123', 256));

-- ------------------------------------------------------------------------------
-- Seed Sample Customer Records
-- ------------------------------------------------------------------------------
INSERT IGNORE INTO customers (customer_id, name, contact, nic, address)
VALUES
(1, 'Kamal Perera',     '0771234567', '200012345678', 'Anuradhapura'),
(2, 'Nimal Silva',      '0712345678', '199912345678', 'Mihintale'),
(3, 'Amali Fernando',   '0761234567', '199812345678', 'Kurunegala'),
(4, 'Sahan Jayasuriya', '0759876543', '200112345678', 'Dambulla');

-- ------------------------------------------------------------------------------
-- Seed Sample Repair Jobs (Covering each workflow status)
-- ------------------------------------------------------------------------------
INSERT IGNORE INTO repair_jobs (job_id, customer_id, customer_name, customer_contact, customer_nic, customer_address, phone_model, details, status, cost, created_by, repairer_id, created_at, finished_at)
VALUES
(1, 1, 'Kamal Perera',     '0771234567', '200012345678', 'Anuradhapura', 'iPhone 13',       'Screen touch unresponsive and cracked display', 'Pending', 0.00, 2, NULL, '2026-10-10 10:00:00', NULL),
(2, 2, 'Nimal Silva',      '0712345678', '199912345678', 'Mihintale',    'Samsung A54',     'Device not charging; loose USB-C port',         'In Progress', 0.00, 2, 3, '2026-10-10 11:30:00', NULL),
(3, 3, 'Amali Fernando',   '0761234567', '199812345678', 'Kurunegala',   'Google Pixel 7',  'Battery replacement and power IC diagnostic',   'Finished', 8500.00, 2, 3, '2026-10-10 09:15:00', '2026-10-10 14:30:00'),
(4, 4, 'Sahan Jayasuriya', '0759876543', '200112345678', 'Dambulla',     'Redmi Note 12',   'Severe liquid damage; motherboard short circuit', 'Couldn''t Repair', 0.00, 2, 3, '2026-10-10 12:00:00', NULL);

-- ==============================================================================
-- Verification Summary
-- ==============================================================================
SELECT 'FixSphere database setup completed successfully!' AS status;
SELECT COUNT(*) AS total_employees FROM employees;
SELECT COUNT(*) AS total_customers FROM customers;
SELECT COUNT(*) AS total_jobs FROM repair_jobs;
