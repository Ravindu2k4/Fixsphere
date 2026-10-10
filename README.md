# FixSphere — Mobile Phone Repair Center Management System

FixSphere is an enterprise-grade Windows desktop management system designed for mobile phone repair centers. Built with **C++/CLI (.NET Windows Forms)** and backed by **MySQL 8.0**, it provides secure role-based access control (RBAC), end-to-end repair job tracking, customer relationship management, and financial analytics.

---

## 🌟 Key Features

* **Role-Based Access Control (RBAC)**:
  * **Admin**: Complete system control — job creation, assigning repairers, updating statuses, deleting records, managing staff accounts, and financial reporting.
  * **Checker**: Front-desk operations — registering customers, creating repair intake jobs, searching jobs and viewing statuses. Restricted from updating statuses or assigning jobs.
  * **Repairer**: Technician workbench — view only jobs assigned to their account, update progress/statuses, and record technical problem details and notes.
* **Repair Lifecycle Tracking**:
  * Workflows: `Pending` $\rightarrow$ `In Progress` $\rightarrow$ `Finished` / `Couldn't Repair`.
  * Automatic business rule enforcement (e.g. `Couldn't Repair` enforces LKR 0.00 cost).
* **Currency Standardization**:
  * All monetary amounts throughout the entire system are standardized in **Sri Lankan Rupees (LKR)** (e.g. `LKR 1,500.00`).
* **Monthly Financial Reports & CSV Export**:
  * Monthly revenue aggregation of completed repairs.
  * Instant one-click CSV export with detailed metadata.
* **Security & Authentication**:
  * Cryptographic password storage using **SHA-256**.
  * Parameterized SQL queries preventing SQL injection across all modules.

---

## 🛠️ Technology Stack

* **Language**: C++/CLI (.NET Framework 4.7.2)
* **GUI Framework**: Windows Forms (WinForms)
* **Database**: MySQL Server 8.0
* **Data Connector**: `MySql.Data.dll` (v8.0.30)
* **IDE**: Microsoft Visual Studio 2022 / Community

---

## 🚀 Getting Started

### 1. Database Setup
1. Ensure **MySQL Server 8.0** is running on your machine.
2. Execute the provided SQL setup script:
   ```bash
   mysql -u root -p < setup_database.sql
   ```
   *This automatically creates the `fixsphere` database schema, tables (`employees`, `customers`, `repair_jobs`), foreign keys, and seeds default accounts.*

### 2. Configure Database Connection
Copy `FixSphere/db_config.ini.example` to `FixSphere/db_config.ini`:
```ini
[mysql]
server=localhost
port=3306
user=root
password=root
database=fixsphere
```

### 3. Build & Run
1. Open `FixSphere.slnx` in **Visual Studio 2022**.
2. Select **Release** (or **Debug**) and platform **x64**.
3. Press **Ctrl + Shift + B** to compile.
4. Press **F5** to start the application.

---

## 🔑 Default Credentials

| Role | Username | Password |
| :--- | :--- | :--- |
| **Admin** | `admin` | `admin123` |
| **Checker** | `checker` | `check123` |
| **Repairer** | `repairer` | `repair123` |

---

## 📄 License
This project is developed for educational and enterprise repair management purposes.
