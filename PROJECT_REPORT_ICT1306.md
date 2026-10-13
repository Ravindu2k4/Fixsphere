# RAJARATA UNIVERSITY OF SRI LANKA
## Faculty of Applied Sciences — Department of Computing
### ICT 1306 — Object Oriented Programming | Group Assignment Report

---

# FixSphere: Mobile Phone Repair Center Management System
**A Multi-Tier Desktop Management Application Built with C++/CLI Windows Forms & MySQL**

---

### Team & Scenario Details

* **Course Module:** ICT 1306 — Object Oriented Programming
* **Team Size:** 5 Members
* **Chosen Scenario:** Small Business / Service — Mobile Phone Repair & Hardware Servicing Center (FixSphere)
* **Target Operating System:** Windows 10 / 11 (x64)
* **Development Environment:** Microsoft Visual Studio 2022 (v143 / v145 toolset)
* **Programming Paradigm:** Object-Oriented Programming (C++/CLI with .NET Framework 4.7.2)
* **Database Management System:** MySQL Server 8.0+ / Connector/NET (MySql.Data v8.0.30)
* **GitHub Repository URL (Public):** [https://github.com/Ravindu2k4/Fixsphere](https://github.com/Ravindu2k4/Fixsphere)
* **Unlisted Demonstration Video URL:** `[INSERT_YOUR_YOUTUBE_UNLISTED_LINK_HERE]`

#### Team Members

| # | Student Full Name | Index Number | Registration Number | Primary Responsibility / Contribution |
|---|-------------------|--------------|---------------------|----------------------------------------|
| 1 | K. R. M. De Silva | 6675 | ICT/2024/063 | Project Lead, System Architecture, Database Schema Design |
| 2 | S. P. Abewickrama | 6618 | ICT/2024/003 | Windows Forms GUI Design, Navigation & Layout Styling |
| 3 | D. L. C. A. Liyanawaduge | 6685 | ICT/2024/078 | OOP Domain Modeling (`Person`, `Customer`, `Employee`) |
| 4 | K. W. R. Adithya | 6620 | ICT/2024/006 | Role-Based Authorization & Workflow Security Logic |
| 5 | M. J. Apeksha | 6628 | ICT/2024/014 | Financial Calculations (LKR), Reporting & Video Presentation |

---

## 1. Executive Summary & Problem Scenario

Small and medium-sized mobile repair centers handle multiple hardware and software service orders simultaneously. Traditionally, customer records, job allocations, diagnostic updates, and billing were managed using ad-hoc paper notes or basic spreadsheets. This leads to common business failures:
1. **Misplaced Devices & Unclear Responsibilities:** Technicians lose track of assigned phones and device statuses.
2. **Security & Permission Risks:** Front-desk receptionists ("Checkers") accidentally change technician diagnostic states or financial totals.
3. **Inaccurate Financial Accounting:** Cash flows, repair revenue, and parts costs are difficult to track accurately.

**FixSphere** is an enterprise-grade desktop solution addressing these challenges. It provides a multi-role Windows desktop application enforcing a strict three-tier authorization hierarchy:
* **Admin:** Full system governance (user management, job assignment, record deletion, executive reports).
* **Checker:** Front-desk intake officer (customer registration, ticket creation, read-only status tracking).
* **Repairer:** Technical specialist (updates diagnostic status, repair notes, and cost for assigned jobs only).

All financial records are standardized in **Sri Lankan Rupees (LKR)**, and all data persists in a relational **MySQL** database with encrypted credentials.

---

## 2. System Architecture & Class List with Responsibilities

The application architecture cleanly separates the User Interface (Presentation Layer), Domain Model Entities (Business Logic Layer), and Database Services (Data Persistence Layer).

| Class Name | File Location | Responsibilities & Architectural Role |
|------------|---------------|---------------------------------------|
| `Person` | `Models.h` | **Abstract Base Class**: Defines common personal identity attributes (`id`, `name`, `contact`, `nic`), encapsulates protected fields, and declares the abstract method `GetRoleDescription()`. |
| `Customer` | `Models.h` | **Derived Entity Class**: Inherits from `Person`. Encapsulates client residential address and overrides role descriptions and summary outputs. |
| `Employee` | `Models.h` | **Derived Entity Class**: Inherits from `Person`. Encapsulates staff roles (`Admin`, `Checker`, `Repairer`), account username, and active status. |
| `UserSession` | `UserSession.h` | **Session Management Class**: Maintains the globally authenticated user state, implements authorization helper functions (`CanAssignRepairer`, `CanUpdateJobStatus`, `CanDeleteJobs`). |
| `DatabaseService` | `DatabaseService.h` | **Data Access Layer**: Encapsulates MySQL database connections, dynamic configuration parsing (`db_config.ini`), query execution, parameter binding, and SHA-256 password hashing. |
| `ComboItem` | `MainForm.h` | **UI Helper Class**: Holds key-value pairs (ID and Display name) for ComboBox dropdowns; overrides `ToString()` for runtime polymorphism. |
| `LoginForm` | `LoginForm.h` | **GUI Presentation Class**: Handles employee authentication, SHA-256 password validation, and initiates user sessions. |
| `MainForm` | `MainForm.h` | **Primary Presentation Class**: Renders the multi-tab interface (Dashboard, Customers, Repair Jobs, Employees, Reports), applies role security, and binds data grids. |

---

## 3. Worked Examples of Each OOP Pillar from Own Code

FixSphere was purposefully architected to demonstrate all six core Object-Oriented Programming concepts directly within custom business logic:

### Pillar 1: Class
A **Class** is an extensible program-code-template defining properties and behaviors.
```cpp
// Defined in Models.h
public ref class Customer : public Person
{
private:
    String^ address;

public:
    Customer(int id, String^ name, String^ contact, String^ nic, String^ address)
        : Person(id, name, contact, nic)
    {
        this->address = address;
    }
    // Properties and methods...
};
```

### Pillar 2: Object
An **Object** is a concrete runtime instance of a class allocated in memory.
```cpp
// Concrete instances created during runtime in LoginForm.h and MainForm.h
UserSession^ session = gcnew UserSession(empId, fullName, username, role);
UserSession::CurrentUser = session;

Customer^ newClient = gcnew Customer(101, L"Sahan Perera", L"0771234567", L"200012345678", L"Anuradhapura");
```

### Pillar 3: Encapsulation
**Encapsulation** hides internal state from direct external manipulation, ensuring data integrity through controlled getters and setters.
```cpp
// Defined in Models.h
public ref class Person abstract
{
private:
    int id;
    String^ name;
    String^ contact;
    String^ nic;

public:
    property String^ Contact
    {
        String^ get() { return contact; }
        void set(String^ value) { contact = value; }
    }
};
```

### Pillar 4: Abstraction
**Abstraction** hides complex implementation details and exposes only essential operations through abstract interfaces.
```cpp
// Defined in Models.h
public ref class Person abstract
{
public:
    // Pure abstract method: enforces derived classes to implement their own role logic
    virtual String^ GetRoleDescription() abstract;
};

// Defined in DatabaseService.h - abstracts MySQL query execution and connection pooling
DataTable^ dt = DatabaseService::ExecuteQuery("SELECT * FROM repair_jobs WHERE status = @st", params);
```

### Pillar 5: Inheritance
**Inheritance** enables derived classes (`Customer`, `Employee`) to inherit fields, properties, and methods from a common parent (`Person`), eliminating code redundancy.
```cpp
// Customer inherits Person attributes: id, name, contact, nic
public ref class Customer : public Person
{
public:
    Customer(int id, String^ name, String^ contact, String^ nic, String^ address)
        : Person(id, name, contact, nic) // Calls base class constructor
    {
        this->address = address;
    }
};
```

### Pillar 6: Polymorphism
**Polymorphism** allows objects of different classes to respond uniquely to the same method invocation at runtime through virtual function overriding.
```cpp
// Derived class Customer overrides base method
virtual String^ Customer::GetRoleDescription() override
{
    return L"Client / Device Owner requiring phone hardware or software servicing.";
}

// Derived class Employee provides different behavior for the same method
virtual String^ Employee::GetRoleDescription() override
{
    return String::Format(L"Internal Staff Member operating under role: {0}", role);
}

// Runtime polymorphic call:
Person^ personRef = gcnew Employee(1, L"Admin", L"0770000000", L"199000000000", L"Admin", L"admin", true);
String^ desc = personRef->GetRoleDescription(); // Dynamically invokes Employee's implementation
```

---

## 4. Class Diagram

```
+---------------------------------------------------------------------------------+
|                                 <<abstract>>                                    |
|                                    Person                                       |
+---------------------------------------------------------------------------------+
| - id: int                                                                       |
| - name: String^                                                                 |
| - contact: String^                                                              |
| - nic: String^                                                                  |
+---------------------------------------------------------------------------------+
| + Person(id: int, name: String^, contact: String^, nic: String^)                |
| + Id: int {get; set;}                                                           |
| + Name: String^ {get; set;}                                                     |
| + Contact: String^ {get; set;}                                                  |
| + Nic: String^ {get; set;}                                                      |
| + GetRoleDescription(): String^ [abstract]                                      |
| + GetSummary(): String^ [virtual]                                               |
+---------------------------------------+-----------------------------------------+
                                        |
                     +------------------+------------------+
                     |                                     |
+--------------------+----------------+   +----------------+---------------------+
|                Customer             |   |               Employee               |
+-------------------------------------+   +--------------------------------------+
| - address: String^                  |   | - role: String^                      |
|                                     |   | - username: String^                  |
|                                     |   | - isActive: bool                     |
+-------------------------------------+   +--------------------------------------+
| + Customer(...)                     |   | + Employee(...)                      |
| + Address: String^ {get; set;}      |   | + Role: String^ {get; set;}          |
| + GetRoleDescription(): String^ [ov]|   | + Username: String^ {get; set;}      |
| + GetSummary(): String^ [override]  |   | + IsActive: bool {get; set;}         |
+-------------------------------------+   | + GetRoleDescription(): String^ [ov] |
                                          | + GetSummary(): String^ [override]   |
                                          +--------------------------------------+

+---------------------------------------+   +------------------------------------+
|              UserSession              |   |          DatabaseService           |
+---------------------------------------+   +------------------------------------+
| + EmployeeId: int                     |   | - connectionString: String^        |
| + FullName: String^                   |   +------------------------------------+
| + Username: String^                   |   | + InitializeConnectionString()     |
| + Role: String^                       |   | + GetConnection(): MySqlConnection |
| + CurrentUser: UserSession^ [static]  |   | + ExecuteQuery(...): DataTable^    |
+---------------------------------------+   | + ExecuteNonQuery(...): int        |
| + IsLoggedIn(): bool                  |   | + HashPassword(raw: String^): Str  |
| + CanManageCustomers(): bool          |   +------------------------------------+
| + CanCreateJobs(): bool               |
| + CanUpdateJobStatus(): bool          |
| + CanAssignRepairer(): bool           |
| + CanDeleteJobs(): bool               |
+---------------------------------------+
```

---

## 5. Database Design & CRUD Demonstration

The MySQL relational schema consists of 3 connected tables with foreign-key constraints:

```
[customers] 1 ────────── ∞ [repair_jobs] ∞ ────────── 1 [employees]
```

### Record Types & CRUD Matrix

| Record Type / Table | Create (Add) | Read (View / Search) | Update (Edit) | Delete (Remove) |
|---------------------|--------------|----------------------|---------------|-----------------|
| **Customers** | Intake form with phone, NIC, address | Search by Name, Contact, or NIC; DataGridView display | Edit phone number, residential address | Admin deletion with integrity checks |
| **Repair Jobs** | Register intake with phone model, fault, initial `Pending` status | Filter by Status (`All`, `Pending`, `In Progress`, `Finished`), keyword search | Status updates (`In Progress`, `Finished`, `Couldn't Repair`), technician assignment | Admin deletion |
| **Employees** | Add staff members with SHA-256 hashed passwords | Employee management grid displaying active staff | Edit name, contact, active status | Soft-delete / deactivate |

---

## 6. Graphical User Interface (Screens Overview)

1. **Screen 1 — Sign-In Authentication (`LoginForm`):** Secure login interface validating credentials against MySQL SHA-256 hashes.
2. **Screen 2 — Executive Dashboard (`MainForm`):** Real-time metric cards (Total Customers, Active Jobs, Monthly Finished Jobs, Total Revenue in LKR).
3. **Screen 3 — Customer Management View:** Full CRUD interface for customer database operations.
4. **Screen 4 — Repair Job Workflow & Assignment View:** Central management screen with role-filtered job grids, status transitions, technician assignments, and cost inputs.
5. **Screen 5 — Employee Governance View:** Admin-only view to configure staff roles and credentials.
6. **Screen 6 — Monthly Revenue & CSV Export Reports:** Real-time billing calculations summarizing Finished repair jobs in LKR with CSV export.

---

## 7. Submission Verification Checklist

* [x] Registered Team of 5 members with computing department.
* [x] Unique business scenario: Mobile Phone Repair Center Management System.
* [x] Implemented at least 2 custom domain classes demonstrating all 6 OOP pillars (`Person`, `Customer`, `Employee`).
* [x] Visual Studio GUI desktop application with 6 distinct functional views (minimum 3 required).
* [x] Real MySQL database connection supporting full CRUD on 3 distinct record types.
* [x] All monetary amounts strictly formatted in Sri Lankan Rupees (LKR).
* [x] Public GitHub repository: `https://github.com/Ravindu2k4/Fixsphere`.
* [x] Project report drafted under the 8-page maximum requirement.
* [ ] 5-minute unlisted YouTube video recorded, uploaded, and link added to final report cover.
