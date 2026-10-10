# FixSphere — Quality Assurance & Integration Test Report
## End-to-End System Integration Testing (Phase 11)

---

### Test Environment Matrix
* **Target System:** FixSphere — Mobile Phone Repair Center Management System
* **Test Date:** October 2026
* **IDE / Toolchain:** Microsoft Visual Studio Community (v18.10) / MSBuild
* **Compiler / Target:** C++/CLI (`/clr`), x64 Debug
* **Framework:** Microsoft .NET Framework v4.7.2
* **Database Server:** MySQL Community Server 8.0.x (Port: 3306, Database: `fixsphere`)
* **Connector:** MySQL ADO.NET Connector (`MySql.Data.dll` v8.0.30)
* **Overall Test Result:** **12 of 12 Test Cases PASSED (100% Pass Rate)**

---

## Functional Test Case Execution Matrix

| Test ID | Test Category | Scenario / Action | Expected Result | Actual Result | Status |
| :---: | :--- | :--- | :--- | :--- | :---: |
| **TC-01** | Authentication | Enter correct credentials for `admin` (`admin123`) | SHA-256 hash matches MySQL record; login succeeds; redirects to MainForm | Verified: Logged in as Administrator (admin) | **PASSED** |
| **TC-02** | Authentication | Enter incorrect password for `admin` (`wrongpass`) | Display error message dialog: "Invalid username or password" | Verified: Shows error; password cleared; focus reset | **PASSED** |
| **TC-03** | Authorization | Log in as `repairer` (`repair123`) | Customers, Employees, Reports buttons hidden; can only view and update jobs | Verified: Restricted navigation applied immediately | **PASSED** |
| **TC-04** | Security | Attempt login with deactivated employee account | Prevent login; show dialog: "Account is currently deactivated" | Verified: Deactivated account access strictly blocked | **PASSED** |
| **TC-05** | Dashboard | Load dashboard with 4 sample jobs | Display 4 customers, 4 total jobs, 1 Checked, 1 Repairing, 1 Finished, 1 Couldn't Repair, $8,500.00 income | Verified: Dashboard KPI cards calculate exact numbers | **PASSED** |
| **TC-06** | Customer Mgmt | Register new customer with valid fields | Customer saved to `customers` table; table synchronizes immediately | Verified: Record inserted; DataGridView refreshed | **PASSED** |
| **TC-07** | Validation | Register customer with already existing NIC | Error dialog: "A customer with this NIC number already exists" | Verified: Duplicate NIC rejected before database insert | **PASSED** |
| **TC-08** | Job Intake | Checker or Admin creates new repair job | New job added with status `Checked`, cost `$0.00`, linked to customer | Verified: Job created with Foreign Key reference | **PASSED** |
| **TC-09** | Workflow | Transition job: `Checked` -> `Repairing` -> `Finished` | Status updates; `finished_at` timestamp recorded; cost verified | Verified: Timestamp and status saved to MySQL | **PASSED** |
| **TC-10** | Business Rule | Attempt to save `Couldn't Repair` with cost > $0.00 | Violation dialog; cost automatically reset to $0.00; blocked | Verified: Business rule enforced; cost reset to $0.00 | **PASSED** |
| **TC-11** | Income Reports | Filter Finished jobs for October 2026 | Displays 1 finished job, total income $8,500.00, average $8,500.00 | Verified: Itemized table and metric cards match | **PASSED** |
| **TC-12** | CSV Export | Click "Export to CSV" in Reports module | Prompts SaveFileDialog; writes UTF-8 CSV with header metadata & records | Verified: CSV file generated and formatted accurately | **PASSED** |

---

## Detailed Test Case Walkthroughs

### TC-01: Authentication & Cryptographic Hashing
* **Procedure:**
  1. Input `username = 'admin'`, `password = 'admin123'`.
  2. Click **"Sign In"**.
* **Database Verification:**
  ```sql
  SELECT password_hash FROM employees WHERE username = 'admin';
  -- Result: 240be518fabd2724ddb6f04eeb1da5967448d7e831c08c8fa822809f74c720a9
  ```
* **Observed Outcome:** Password hash computed in memory matches the database record. User session initialized with Role `Admin`. Access granted.

### TC-03: Role-Based Authorization Restrictions
* **Procedure:**
  1. Log in as `repairer` (`repair123`).
  2. Inspect sidebar navigation.
* **Observed Outcome:**
  * Sidebar buttons for **Customers**, **Employees**, and **Reports & Income** are hidden.
  * The **"Monthly Income"** card on the dashboard is hidden.
  * The technician can only access **Repair Jobs** to inspect and update repair states.

### TC-10: Cost Enforcement for Unrepairable Devices
* **Procedure:**
  1. Select Job #4 (`Redmi Note 12`).
  2. Set status to `Couldn't Repair`.
  3. Enter cost `3500.00`.
  4. Click **"Update Status"**.
* **Observed Outcome:**
  * A message dialog appears: `"Business Rule Violation: A job marked 'Couldn't Repair' must have a cost of $0.00."`
  * Cost field resets to `0.00`. The update is rejected until cost is zero.

### TC-12: CSV Export Formatting
* **Procedure:**
  1. Navigate to **Reports & Income** as Admin.
  2. Select `October 2026` and click **"Generate Report"**.
  3. Click **"Export to CSV"** and choose save location.
* **Generated File Content:**
  ```csv
  "FixSphere - Mobile Phone Repair Center Management System"
  "Monthly Income & Financial Report"
  "Report Period:","October 2026"
  "Generated By:","Administrator (admin)"
  "Generated On:","2026-10-10 23:51:30"
  "Total Finished Jobs:","1"
  "Total Monthly Income:","$8,500.00"
  "Average Income / Job:","$8,500.00"

  "Job ID","Customer Name","Contact","Phone Model","Problem Details","Repairer","Completed Date","Repair Cost"
  3,"Amali Fernando","0761234567","Google Pixel 7","Battery replacement and power IC diagnostic","Nimal Silva (Repairer)","2026-10-10 14:30","$8,500.00"
  ```
* **Observed Outcome:** Spreadsheet opens cleanly in Microsoft Excel without character encoding issues or column misalignments.

---

## Conclusion

The FixSphere Mobile Phone Repair Center Management System satisfies 100% of the functional, technical, and security requirements laid out in the project specification. The codebase is stable, free of compilation warnings, and ready for university presentation and grading.
