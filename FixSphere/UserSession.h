#pragma once

using namespace System;

namespace FixSphere {

	public ref class UserSession
	{
	public:
		property int EmployeeId;
		property String^ FullName;
		property String^ Username;
		property String^ Role;

		UserSession(int empId, String^ fullName, String^ username, String^ role)
		{
			this->EmployeeId = empId;
			this->FullName = fullName;
			this->Username = username;
			this->Role = role;
		}

		// Global session instance for the currently logged-in user
		static UserSession^ CurrentUser = nullptr;

		static bool IsLoggedIn()
		{
			return CurrentUser != nullptr;
		}

		static void Logout()
		{
			CurrentUser = nullptr;
		}

		// Role checks
		bool IsAdmin()
		{
			return Role != nullptr && Role->Equals(L"Admin", StringComparison::OrdinalIgnoreCase);
		}

		bool IsChecker()
		{
			return Role != nullptr && Role->Equals(L"Checker", StringComparison::OrdinalIgnoreCase);
		}

		bool IsRepairer()
		{
			return Role != nullptr && Role->Equals(L"Repairer", StringComparison::OrdinalIgnoreCase);
		}

		// --- Feature authorization rules ---

		// Admin and Checker can register customers
		bool CanManageCustomers()
		{
			return IsAdmin() || IsChecker();
		}

		// Admin and Checker can create new repair job intake records
		bool CanCreateJobs()
		{
			return IsAdmin() || IsChecker();
		}

		// Only Admin can update the status of any job.
		// Repairer can update status but only for jobs assigned to them (checked at DB level).
		// Checker CANNOT update any job status.
		bool CanUpdateJobStatus()
		{
			return IsAdmin() || IsRepairer();
		}

		// Only Admin can assign / reassign a job to a Repairer
		bool CanAssignRepairer()
		{
			return IsAdmin();
		}

		// Only Admin can delete repair jobs
		bool CanDeleteJobs()
		{
			return IsAdmin();
		}

		// Only Admin can manage employees
		bool CanManageEmployees()
		{
			return IsAdmin();
		}

		// Only Admin can view financial reports
		bool CanViewFinancialReports()
		{
			return IsAdmin();
		}
	};
}
