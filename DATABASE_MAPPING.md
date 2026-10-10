# FixSphere — Database Mapping Document

**Application:** FixSphere — Mobile Phone Repair Center Management System  
**Database Name:** `fixsphere` (MySQL Community Server 8.0)  
**Host / Port:** `localhost:3306`

---

## 1. Current Database State & Existing Data

The live MySQL server was inspected on the local machine. The database currently contains two tables with live records:

### Table 1: `employees`
Stores employee profiles, roles, credentials, and timestamps.
* `emp_id` INT AUTO_INCREMENT PRIMARY KEY
* `name` VARCHAR(100) NOT NULL
* `nic` VARCHAR(12) NOT NULL UNIQUE
* `contact` VARCHAR(10) NOT NULL
* `address` VARCHAR(255) NOT NULL
* `role` VARCHAR(10) NOT NULL (`Admin`, `Checker`, `Repairer`)
* `username` VARCHAR(50) NOT NULL UNIQUE
* `password_hash` CHAR(64) NOT NULL (SHA-256 encrypted hash)
* `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP

**Existing Verified Accounts:**
1. **Admin:** Username: `admin` | Default password: `admin123` | Role: `Admin`
2. **Checker:** Username: `checker` | Default password: `check123` | Role: `Checker`
3. **Repairer:** Username: `repairer` | Default password: `repair123` | Role: `Repairer`

---

### Table 2: `repair_jobs`
Stores repair requests, device details, problem descriptions, and financial data.
* `job_id` INT AUTO_INCREMENT PRIMARY KEY
* `customer_name` VARCHAR(100) NOT NULL
* `customer_contact` VARCHAR(10) NOT NULL
* `customer_nic` VARCHAR(12) NOT NULL
* `customer_address` VARCHAR(255) NOT NULL
* `phone_model` VARCHAR(100) NOT NULL
* `details` TEXT NOT NULL
* `status` VARCHAR(20) NOT NULL DEFAULT 'Checked' (`Checked` / `Repairing` / `Finished` / `Couldn't Repair`)
* `cost` DECIMAL(10,2) NOT NULL DEFAULT 0.00
* `created_by` INT (FK referencing `employees.emp_id`)
* `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP
* `finished_at` DATETIME NULL (used for monthly income reports)

**Existing Verified Jobs (4 sample records):**
* Job #1: iPhone 13 | Status: `Checked` | Cost: $0.00 | Customer: Kamal Perera
* Job #2: Samsung A54 | Status: `Repairing` | Cost: $0.00 | Customer: Nimal Silva
* Job #3: Google Pixel 7 | Status: `Finished` | Cost: $8,500.00 | Customer: Amali Fernando | Finished At: 2026-10-10
* Job #4: Redmi Note 12 | Status: `Couldn't Repair` | Cost: $0.00 | Customer: Sahan Jayasuriya

---

## 2. Feature-to-Database Mapping

| Feature Module | Application Requirement | Database Table & Columns | Status |
| :--- | :--- | :--- | :--- |
| **Authentication (Phase 5)** | Username + password verification | `employees.username`, `employees.password_hash` (SHA-256) | Ready |
| **Role Authorization (Phase 5)** | Enforce permissions for Admin, Checker, Repairer | `employees.role` | Ready |
| **Account Activation (Phase 9)** | Admin can activate/deactivate accounts | `employees.is_active` (Proposed addition: TINYINT(1) DEFAULT 1) | Needs Migration |
| **Customer Management (Phase 7)** | Register customer, view list, search by NIC/phone/name | Dedicated `customers` table (Proposed migration) | Needs Migration |
| **Job Management (Phase 8)** | Register job, assign repairer, record problem | `repair_jobs.customer_id`, `repair_jobs.repairer_id`, `repair_jobs.details` | Needs Migration |
| **Workflow Statuses (Phase 8)** | Checked -> Repairing -> Finished / Couldn't Repair | `repair_jobs.status` | Ready |
| **Cost Validation (Phase 8)** | Cost = 0 for Couldn't Repair, Cost >= 0 for Finished | `repair_jobs.cost` | Ready |
| **Dashboard Metrics (Phase 6)** | Counts of Checked, Repairing, Finished, Couldn't Repair | `SELECT status, COUNT(*) FROM repair_jobs GROUP BY status` | Ready |
| **Monthly Income (Phase 10)** | Sum of costs of Finished jobs in selected month/year | `SELECT SUM(cost) FROM repair_jobs WHERE status='Finished' AND MONTH(finished_at)=? AND YEAR(finished_at)=?` | Ready |

---

## 3. Proposed Safe Schema Migration (Preserving All Data)

To support dedicated **Customer Management** (Phase 7) and **Repairer Assignment** without breaking or deleting any existing jobs or employees:

```sql
-- Step 1: Create dedicated customers table
CREATE TABLE IF NOT EXISTS fixsphere.customers (
    customer_id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    contact VARCHAR(10) NOT NULL,
    nic VARCHAR(12) NOT NULL UNIQUE,
    address VARCHAR(255) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Step 2: Safely populate customers from existing jobs (no data loss)
INSERT IGNORE INTO fixsphere.customers (name, contact, nic, address)
SELECT DISTINCT customer_name, customer_contact, customer_nic, customer_address
FROM fixsphere.repair_jobs;

-- Step 3: Add customer_id and repairer_id references to repair_jobs
ALTER TABLE fixsphere.repair_jobs 
    ADD COLUMN customer_id INT NULL AFTER job_id,
    ADD COLUMN repairer_id INT NULL AFTER created_by;

-- Step 4: Backfill customer_id in repair_jobs
UPDATE fixsphere.repair_jobs r
JOIN fixsphere.customers c ON r.customer_nic = c.nic
SET r.customer_id = c.customer_id;

-- Step 5: Add account activation flag to employees
ALTER TABLE fixsphere.employees
    ADD COLUMN is_active TINYINT(1) NOT NULL DEFAULT 1 AFTER role;
```

> **Safety Notice:** This migration will NOT drop any table or column, will NOT alter any password, and will preserve all 4 existing jobs and all 3 existing employee accounts.
