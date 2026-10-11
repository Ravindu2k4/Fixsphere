#pragma once

#include "DatabaseService.h"
#include "UserSession.h"
#include "Models.h"

namespace FixSphere {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace MySql::Data::MySqlClient;

	// Helper item class for ComboBox items holding an integer ID
	public ref class ComboItem
	{
	public:
		int Id;
		String^ Display;

		ComboItem(int id, String^ display)
		{
			this->Id = id;
			this->Display = display;
		}

		virtual String^ ToString() override
		{
			return Display;
		}
	};

	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			ApplyRoleSecurity();
			InitReportSelectors();
			LoadDashboardData();
		}

	protected:
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		System::ComponentModel::Container ^components;

		// ----------------------------------------------------
		// Layout & Navigation Panels
		// ----------------------------------------------------
		System::Windows::Forms::Panel^ pnlSidebar;
		System::Windows::Forms::Panel^ pnlBrand;
		System::Windows::Forms::Label^ lblBrandTitle;
		System::Windows::Forms::Label^ lblBrandSubtitle;
		System::Windows::Forms::Panel^ pnlNavButtons;
		System::Windows::Forms::Button^ btnNavDashboard;
		System::Windows::Forms::Button^ btnNavCustomers;
		System::Windows::Forms::Button^ btnNavJobs;
		System::Windows::Forms::Button^ btnNavEmployees;
		System::Windows::Forms::Button^ btnNavReports;
		System::Windows::Forms::Panel^ pnlSidebarFooter;
		System::Windows::Forms::Label^ lblDbStatus;

		System::Windows::Forms::Panel^ pnlHeader;
		System::Windows::Forms::Label^ lblViewTitle;
		System::Windows::Forms::Panel^ pnlUserBadge;
		System::Windows::Forms::Label^ lblLoggedUser;
		System::Windows::Forms::Label^ lblLoggedRole;
		System::Windows::Forms::Button^ btnLogout;

		System::Windows::Forms::Panel^ pnlMainContent;

		// ----------------------------------------------------
		// 1. Dashboard View Components (Phase 6)
		// ----------------------------------------------------
		System::Windows::Forms::Panel^ pnlViewDashboard;
		System::Windows::Forms::FlowLayoutPanel^ flowStatCards;

		System::Windows::Forms::Panel^ cardCustomers;
		System::Windows::Forms::Label^ lblStatCustVal;

		System::Windows::Forms::Panel^ cardJobsTotal;
		System::Windows::Forms::Label^ lblStatJobsVal;

		System::Windows::Forms::Panel^ cardJobsPending;
		System::Windows::Forms::Label^ lblStatPendingVal;

		System::Windows::Forms::Panel^ cardJobsRepairing;
		System::Windows::Forms::Label^ lblStatRepairingVal;

		System::Windows::Forms::Panel^ cardJobsFinished;
		System::Windows::Forms::Label^ lblStatFinishedVal;

		System::Windows::Forms::Panel^ cardJobsFailed;
		System::Windows::Forms::Label^ lblStatFailedVal;

		System::Windows::Forms::Panel^ cardIncome;
		System::Windows::Forms::Label^ lblStatIncomeVal;

		System::Windows::Forms::Panel^ pnlRecentJobsHeader;
		System::Windows::Forms::Label^ lblRecentJobsTitle;
		System::Windows::Forms::Button^ btnRefreshDashboard;
		System::Windows::Forms::DataGridView^ dgvRecentJobs;

		// ----------------------------------------------------
		// 2. Customer Management View Components (Phase 7)
		// ----------------------------------------------------
		System::Windows::Forms::Panel^ pnlViewCustomers;
		System::Windows::Forms::Panel^ pnlCustFormCard;
		System::Windows::Forms::Label^ lblCustFormHeader;
		System::Windows::Forms::Label^ lblCustIdStatus;
		System::Windows::Forms::Label^ lblCustName;
		System::Windows::Forms::TextBox^ txtCustName;
		System::Windows::Forms::Label^ lblCustNic;
		System::Windows::Forms::TextBox^ txtCustNic;
		System::Windows::Forms::Label^ lblCustContact;
		System::Windows::Forms::TextBox^ txtCustContact;
		System::Windows::Forms::Label^ lblCustAddress;
		System::Windows::Forms::TextBox^ txtCustAddress;
		System::Windows::Forms::Button^ btnCustSave;
		System::Windows::Forms::Button^ btnCustUpdate;
		System::Windows::Forms::Button^ btnCustClear;

		System::Windows::Forms::Panel^ pnlCustSpacer;
		System::Windows::Forms::Panel^ pnlCustTableCard;
		System::Windows::Forms::Panel^ pnlCustSearchHeader;
		System::Windows::Forms::Label^ lblCustSearch;
		System::Windows::Forms::TextBox^ txtCustSearch;
		System::Windows::Forms::Button^ btnCustSearch;
		System::Windows::Forms::Button^ btnCustReset;
		System::Windows::Forms::Label^ lblCustCount;
		System::Windows::Forms::DataGridView^ dgvCustomers;
		int selectedCustomerId = 0;

		// ----------------------------------------------------
		// 3. Repair Job Management View Components (Phase 8)
		// ----------------------------------------------------
		System::Windows::Forms::Panel^ pnlViewJobs;
		System::Windows::Forms::Panel^ pnlJobFormCard;
		System::Windows::Forms::Label^ lblJobFormHeader;
		System::Windows::Forms::Label^ lblJobIdStatus;
		System::Windows::Forms::Label^ lblJobCustomer;
		System::Windows::Forms::ComboBox^ cmbJobCustomer;
		System::Windows::Forms::Label^ lblJobPhoneModel;
		System::Windows::Forms::TextBox^ txtJobPhoneModel;
		System::Windows::Forms::Label^ lblJobDetails;
		System::Windows::Forms::TextBox^ txtJobDetails;
		System::Windows::Forms::Label^ lblJobRepairer;
		System::Windows::Forms::ComboBox^ cmbJobRepairer;
		System::Windows::Forms::TextBox^ txtJobRepairerDisplay;
		System::Windows::Forms::Label^ lblJobStatus;
		System::Windows::Forms::ComboBox^ cmbJobStatus;
		System::Windows::Forms::Label^ lblJobCost;
		System::Windows::Forms::TextBox^ txtJobCost;
		System::Windows::Forms::Button^ btnJobSave;
		System::Windows::Forms::Button^ btnJobUpdate;
		System::Windows::Forms::Button^ btnJobDelete;
		System::Windows::Forms::Button^ btnJobClear;

		System::Windows::Forms::Panel^ pnlJobSpacer;
		System::Windows::Forms::Panel^ pnlJobTableCard;
		System::Windows::Forms::Panel^ pnlJobSearchHeader;
		System::Windows::Forms::Label^ lblJobSearch;
		System::Windows::Forms::TextBox^ txtJobSearch;
		System::Windows::Forms::Label^ lblFilterStatus;
		System::Windows::Forms::ComboBox^ cmbFilterStatus;
		System::Windows::Forms::Button^ btnJobSearch;
		System::Windows::Forms::Button^ btnJobReset;
		System::Windows::Forms::Label^ lblJobCount;
		System::Windows::Forms::DataGridView^ dgvJobs;
		int selectedJobId = 0;
		int selectedJobRepairerId = 0;

		// ----------------------------------------------------
		// 4. Employee Management View Components (Phase 9)
		// ----------------------------------------------------
		System::Windows::Forms::Panel^ pnlViewEmployees;
		System::Windows::Forms::Panel^ pnlEmpFormCard;
		System::Windows::Forms::Label^ lblEmpFormHeader;
		System::Windows::Forms::Label^ lblEmpIdStatus;
		System::Windows::Forms::Label^ lblEmpName;
		System::Windows::Forms::TextBox^ txtEmpName;
		System::Windows::Forms::Label^ lblEmpNic;
		System::Windows::Forms::TextBox^ txtEmpNic;
		System::Windows::Forms::Label^ lblEmpContact;
		System::Windows::Forms::TextBox^ txtEmpContact;
		System::Windows::Forms::Label^ lblEmpAddress;
		System::Windows::Forms::TextBox^ txtEmpAddress;
		System::Windows::Forms::Label^ lblEmpRole;
		System::Windows::Forms::ComboBox^ cmbEmpRole;
		System::Windows::Forms::CheckBox^ chkEmpActive;
		System::Windows::Forms::Label^ lblEmpUsername;
		System::Windows::Forms::TextBox^ txtEmpUsername;
		System::Windows::Forms::Label^ lblEmpPassword;
		System::Windows::Forms::TextBox^ txtEmpPassword;
		System::Windows::Forms::CheckBox^ chkEmpShowPass;
		System::Windows::Forms::Button^ btnEmpSave;
		System::Windows::Forms::Button^ btnEmpUpdate;
		System::Windows::Forms::Button^ btnEmpResetPass;
		System::Windows::Forms::Button^ btnEmpToggleActive;
		System::Windows::Forms::Button^ btnEmpClear;

		System::Windows::Forms::Panel^ pnlEmpSpacer;
		System::Windows::Forms::Panel^ pnlEmpTableCard;
		System::Windows::Forms::Panel^ pnlEmpSearchHeader;
		System::Windows::Forms::Label^ lblEmpSearch;
		System::Windows::Forms::TextBox^ txtEmpSearch;
		System::Windows::Forms::Label^ lblFilterEmpRole;
		System::Windows::Forms::ComboBox^ cmbFilterEmpRole;
		System::Windows::Forms::Button^ btnEmpSearch;
		System::Windows::Forms::Button^ btnEmpReset;
		System::Windows::Forms::Label^ lblEmpCount;
		System::Windows::Forms::DataGridView^ dgvEmployees;
		int selectedEmpId = 0;

		// ----------------------------------------------------
		// 5. Reports & Income View Components (Phase 10)
		// ----------------------------------------------------
		System::Windows::Forms::Panel^ pnlViewReports;

		// Filter card
		System::Windows::Forms::Panel^ pnlReportFilterCard;
		System::Windows::Forms::Label^ lblReportFilterTitle;
		System::Windows::Forms::Label^ lblReportMonth;
		System::Windows::Forms::ComboBox^ cmbReportMonth;
		System::Windows::Forms::Label^ lblReportYear;
		System::Windows::Forms::ComboBox^ cmbReportYear;
		System::Windows::Forms::Button^ btnGenerateReport;
		System::Windows::Forms::Button^ btnCurrentMonth;
		System::Windows::Forms::Button^ btnExportCsv;

		// Stats summary cards
		System::Windows::Forms::FlowLayoutPanel^ flowReportStats;
		System::Windows::Forms::Panel^ cardRepIncome;
		System::Windows::Forms::Panel^ cardRepFinished;
		System::Windows::Forms::Panel^ cardRepAvg;
		System::Windows::Forms::Panel^ cardRepPeriod;
		System::Windows::Forms::Label^ lblRepIncomeVal;
		System::Windows::Forms::Label^ lblRepFinishedVal;
		System::Windows::Forms::Label^ lblRepAvgVal;
		System::Windows::Forms::Label^ lblRepPeriodVal;

		// Table card
		System::Windows::Forms::Panel^ pnlReportTableCard;
		System::Windows::Forms::Panel^ pnlReportTableHeader;
		System::Windows::Forms::Label^ lblReportTableTitle;
		System::Windows::Forms::Label^ lblReportCount;
		System::Windows::Forms::DataGridView^ dgvReportJobs;

		// ----------------------------------------------------
		// 6. Placeholder View Panel
		// ----------------------------------------------------
		System::Windows::Forms::Panel^ pnlPlaceholderView;
		System::Windows::Forms::Label^ lblPlaceholderTitle;
		System::Windows::Forms::Label^ lblPlaceholderDesc;

		// Helper to create metric cards
		System::Windows::Forms::Panel^ CreateStatCard(String^ title, [System::Runtime::InteropServices::Out] System::Windows::Forms::Label^% outValLabel, Color accentColor, int width)
		{
			System::Windows::Forms::Panel^ card = gcnew System::Windows::Forms::Panel();
			card->BackColor = Color::White;
			card->Size = System::Drawing::Size(width, 100);
			card->Margin = System::Windows::Forms::Padding(0, 0, 15, 15);
			card->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;

			System::Windows::Forms::Panel^ stripe = gcnew System::Windows::Forms::Panel();
			stripe->BackColor = accentColor;
			stripe->Dock = System::Windows::Forms::DockStyle::Left;
			stripe->Width = 6;
			card->Controls->Add(stripe);

			outValLabel = gcnew System::Windows::Forms::Label();
			outValLabel->AutoSize = false;
			outValLabel->Font = gcnew System::Drawing::Font(L"Segoe UI", 18, System::Drawing::FontStyle::Bold);
			outValLabel->ForeColor = Color::FromArgb(29, 53, 87);
			outValLabel->Location = System::Drawing::Point(16, 12);
			outValLabel->Size = System::Drawing::Size(width - 25, 36);
			outValLabel->Text = L"--";
			card->Controls->Add(outValLabel);

			System::Windows::Forms::Label^ lblTitle = gcnew System::Windows::Forms::Label();
			lblTitle->AutoSize = false;
			lblTitle->Font = gcnew System::Drawing::Font(L"Segoe UI", 8.75f, System::Drawing::FontStyle::Bold);
			lblTitle->ForeColor = Color::FromArgb(108, 117, 125);
			lblTitle->Location = System::Drawing::Point(18, 52);
			lblTitle->Size = System::Drawing::Size(width - 25, 30);
			lblTitle->Text = title->ToUpper();
			card->Controls->Add(lblTitle);

			return card;
		}

		void InitializeComponent(void)
		{
			this->components = gcnew System::ComponentModel::Container();

			// Layout panels
			this->pnlSidebar = (gcnew System::Windows::Forms::Panel());
			this->pnlBrand = (gcnew System::Windows::Forms::Panel());
			this->lblBrandTitle = (gcnew System::Windows::Forms::Label());
			this->lblBrandSubtitle = (gcnew System::Windows::Forms::Label());
			this->pnlNavButtons = (gcnew System::Windows::Forms::Panel());
			this->btnNavDashboard = (gcnew System::Windows::Forms::Button());
			this->btnNavCustomers = (gcnew System::Windows::Forms::Button());
			this->btnNavJobs = (gcnew System::Windows::Forms::Button());
			this->btnNavEmployees = (gcnew System::Windows::Forms::Button());
			this->btnNavReports = (gcnew System::Windows::Forms::Button());
			this->pnlSidebarFooter = (gcnew System::Windows::Forms::Panel());
			this->lblDbStatus = (gcnew System::Windows::Forms::Label());

			this->pnlHeader = (gcnew System::Windows::Forms::Panel());
			this->lblViewTitle = (gcnew System::Windows::Forms::Label());
			this->pnlUserBadge = (gcnew System::Windows::Forms::Panel());
			this->lblLoggedUser = (gcnew System::Windows::Forms::Label());
			this->lblLoggedRole = (gcnew System::Windows::Forms::Label());
			this->btnLogout = (gcnew System::Windows::Forms::Button());

			this->pnlMainContent = (gcnew System::Windows::Forms::Panel());

			// Dashboard controls
			this->pnlViewDashboard = (gcnew System::Windows::Forms::Panel());
			this->flowStatCards = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->pnlRecentJobsHeader = (gcnew System::Windows::Forms::Panel());
			this->lblRecentJobsTitle = (gcnew System::Windows::Forms::Label());
			this->btnRefreshDashboard = (gcnew System::Windows::Forms::Button());
			this->dgvRecentJobs = (gcnew System::Windows::Forms::DataGridView());

			// Customer view controls
			this->pnlViewCustomers = (gcnew System::Windows::Forms::Panel());
			this->pnlCustFormCard = (gcnew System::Windows::Forms::Panel());
			this->lblCustFormHeader = (gcnew System::Windows::Forms::Label());
			this->lblCustIdStatus = (gcnew System::Windows::Forms::Label());
			this->lblCustName = (gcnew System::Windows::Forms::Label());
			this->txtCustName = (gcnew System::Windows::Forms::TextBox());
			this->lblCustNic = (gcnew System::Windows::Forms::Label());
			this->txtCustNic = (gcnew System::Windows::Forms::TextBox());
			this->lblCustContact = (gcnew System::Windows::Forms::Label());
			this->txtCustContact = (gcnew System::Windows::Forms::TextBox());
			this->lblCustAddress = (gcnew System::Windows::Forms::Label());
			this->txtCustAddress = (gcnew System::Windows::Forms::TextBox());
			this->btnCustSave = (gcnew System::Windows::Forms::Button());
			this->btnCustUpdate = (gcnew System::Windows::Forms::Button());
			this->btnCustClear = (gcnew System::Windows::Forms::Button());

			this->pnlCustSpacer = (gcnew System::Windows::Forms::Panel());
			this->pnlCustTableCard = (gcnew System::Windows::Forms::Panel());
			this->pnlCustSearchHeader = (gcnew System::Windows::Forms::Panel());
			this->lblCustSearch = (gcnew System::Windows::Forms::Label());
			this->txtCustSearch = (gcnew System::Windows::Forms::TextBox());
			this->btnCustSearch = (gcnew System::Windows::Forms::Button());
			this->btnCustReset = (gcnew System::Windows::Forms::Button());
			this->lblCustCount = (gcnew System::Windows::Forms::Label());
			this->dgvCustomers = (gcnew System::Windows::Forms::DataGridView());

			// Repair Jobs view controls
			this->pnlViewJobs = (gcnew System::Windows::Forms::Panel());
			this->pnlJobFormCard = (gcnew System::Windows::Forms::Panel());
			this->lblJobFormHeader = (gcnew System::Windows::Forms::Label());
			this->lblJobIdStatus = (gcnew System::Windows::Forms::Label());
			this->lblJobCustomer = (gcnew System::Windows::Forms::Label());
			this->cmbJobCustomer = (gcnew System::Windows::Forms::ComboBox());
			this->lblJobPhoneModel = (gcnew System::Windows::Forms::Label());
			this->txtJobPhoneModel = (gcnew System::Windows::Forms::TextBox());
			this->lblJobDetails = (gcnew System::Windows::Forms::Label());
			this->txtJobDetails = (gcnew System::Windows::Forms::TextBox());
			this->lblJobRepairer = (gcnew System::Windows::Forms::Label());
			this->cmbJobRepairer = (gcnew System::Windows::Forms::ComboBox());
			this->txtJobRepairerDisplay = (gcnew System::Windows::Forms::TextBox());
			this->lblJobStatus = (gcnew System::Windows::Forms::Label());
			this->cmbJobStatus = (gcnew System::Windows::Forms::ComboBox());
			this->lblJobCost = (gcnew System::Windows::Forms::Label());
			this->txtJobCost = (gcnew System::Windows::Forms::TextBox());
			this->btnJobSave = (gcnew System::Windows::Forms::Button());
			this->btnJobUpdate = (gcnew System::Windows::Forms::Button());
			this->btnJobDelete = (gcnew System::Windows::Forms::Button());
			this->btnJobClear = (gcnew System::Windows::Forms::Button());

			this->pnlJobSpacer = (gcnew System::Windows::Forms::Panel());
			this->pnlJobTableCard = (gcnew System::Windows::Forms::Panel());
			this->pnlJobSearchHeader = (gcnew System::Windows::Forms::Panel());
			this->lblJobSearch = (gcnew System::Windows::Forms::Label());
			this->txtJobSearch = (gcnew System::Windows::Forms::TextBox());
			this->lblFilterStatus = (gcnew System::Windows::Forms::Label());
			this->cmbFilterStatus = (gcnew System::Windows::Forms::ComboBox());
			this->btnJobSearch = (gcnew System::Windows::Forms::Button());
			this->btnJobReset = (gcnew System::Windows::Forms::Button());
			this->lblJobCount = (gcnew System::Windows::Forms::Label());
			this->dgvJobs = (gcnew System::Windows::Forms::DataGridView());

			// Employee view controls (Phase 9)
			this->pnlViewEmployees = (gcnew System::Windows::Forms::Panel());
			this->pnlEmpFormCard = (gcnew System::Windows::Forms::Panel());
			this->lblEmpFormHeader = (gcnew System::Windows::Forms::Label());
			this->lblEmpIdStatus = (gcnew System::Windows::Forms::Label());
			this->lblEmpName = (gcnew System::Windows::Forms::Label());
			this->txtEmpName = (gcnew System::Windows::Forms::TextBox());
			this->lblEmpNic = (gcnew System::Windows::Forms::Label());
			this->txtEmpNic = (gcnew System::Windows::Forms::TextBox());
			this->lblEmpContact = (gcnew System::Windows::Forms::Label());
			this->txtEmpContact = (gcnew System::Windows::Forms::TextBox());
			this->lblEmpAddress = (gcnew System::Windows::Forms::Label());
			this->txtEmpAddress = (gcnew System::Windows::Forms::TextBox());
			this->lblEmpRole = (gcnew System::Windows::Forms::Label());
			this->cmbEmpRole = (gcnew System::Windows::Forms::ComboBox());
			this->chkEmpActive = (gcnew System::Windows::Forms::CheckBox());
			this->lblEmpUsername = (gcnew System::Windows::Forms::Label());
			this->txtEmpUsername = (gcnew System::Windows::Forms::TextBox());
			this->lblEmpPassword = (gcnew System::Windows::Forms::Label());
			this->txtEmpPassword = (gcnew System::Windows::Forms::TextBox());
			this->chkEmpShowPass = (gcnew System::Windows::Forms::CheckBox());
			this->btnEmpSave = (gcnew System::Windows::Forms::Button());
			this->btnEmpUpdate = (gcnew System::Windows::Forms::Button());
			this->btnEmpResetPass = (gcnew System::Windows::Forms::Button());
			this->btnEmpToggleActive = (gcnew System::Windows::Forms::Button());
			this->btnEmpClear = (gcnew System::Windows::Forms::Button());

			this->pnlEmpSpacer = (gcnew System::Windows::Forms::Panel());
			this->pnlEmpTableCard = (gcnew System::Windows::Forms::Panel());
			this->pnlEmpSearchHeader = (gcnew System::Windows::Forms::Panel());
			this->lblEmpSearch = (gcnew System::Windows::Forms::Label());
			this->txtEmpSearch = (gcnew System::Windows::Forms::TextBox());
			this->lblFilterEmpRole = (gcnew System::Windows::Forms::Label());
			this->cmbFilterEmpRole = (gcnew System::Windows::Forms::ComboBox());
			this->btnEmpSearch = (gcnew System::Windows::Forms::Button());
			this->btnEmpReset = (gcnew System::Windows::Forms::Button());
			this->lblEmpCount = (gcnew System::Windows::Forms::Label());
			this->dgvEmployees = (gcnew System::Windows::Forms::DataGridView());

			// Reports view controls (Phase 10)
			this->pnlViewReports = (gcnew System::Windows::Forms::Panel());
			this->pnlReportFilterCard = (gcnew System::Windows::Forms::Panel());
			this->lblReportFilterTitle = (gcnew System::Windows::Forms::Label());
			this->lblReportMonth = (gcnew System::Windows::Forms::Label());
			this->cmbReportMonth = (gcnew System::Windows::Forms::ComboBox());
			this->lblReportYear = (gcnew System::Windows::Forms::Label());
			this->cmbReportYear = (gcnew System::Windows::Forms::ComboBox());
			this->btnGenerateReport = (gcnew System::Windows::Forms::Button());
			this->btnCurrentMonth = (gcnew System::Windows::Forms::Button());
			this->btnExportCsv = (gcnew System::Windows::Forms::Button());

			this->flowReportStats = (gcnew System::Windows::Forms::FlowLayoutPanel());

			this->pnlReportTableCard = (gcnew System::Windows::Forms::Panel());
			this->pnlReportTableHeader = (gcnew System::Windows::Forms::Panel());
			this->lblReportTableTitle = (gcnew System::Windows::Forms::Label());
			this->lblReportCount = (gcnew System::Windows::Forms::Label());
			this->dgvReportJobs = (gcnew System::Windows::Forms::DataGridView());

			// Placeholder view controls
			this->pnlPlaceholderView = (gcnew System::Windows::Forms::Panel());
			this->lblPlaceholderTitle = (gcnew System::Windows::Forms::Label());
			this->lblPlaceholderDesc = (gcnew System::Windows::Forms::Label());

			this->pnlSidebar->SuspendLayout();
			this->pnlBrand->SuspendLayout();
			this->pnlNavButtons->SuspendLayout();
			this->pnlSidebarFooter->SuspendLayout();
			this->pnlHeader->SuspendLayout();
			this->pnlUserBadge->SuspendLayout();
			this->pnlMainContent->SuspendLayout();

			this->pnlViewDashboard->SuspendLayout();
			this->pnlRecentJobsHeader->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dgvRecentJobs))->BeginInit();

			this->pnlViewCustomers->SuspendLayout();
			this->pnlCustFormCard->SuspendLayout();
			this->pnlCustTableCard->SuspendLayout();
			this->pnlCustSearchHeader->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dgvCustomers))->BeginInit();

			this->pnlViewJobs->SuspendLayout();
			this->pnlJobFormCard->SuspendLayout();
			this->pnlJobTableCard->SuspendLayout();
			this->pnlJobSearchHeader->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dgvJobs))->BeginInit();

			this->pnlViewEmployees->SuspendLayout();
			this->pnlEmpFormCard->SuspendLayout();
			this->pnlEmpTableCard->SuspendLayout();
			this->pnlEmpSearchHeader->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dgvEmployees))->BeginInit();

			this->pnlViewReports->SuspendLayout();
			this->pnlReportFilterCard->SuspendLayout();
			this->flowReportStats->SuspendLayout();
			this->pnlReportTableCard->SuspendLayout();
			this->pnlReportTableHeader->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dgvReportJobs))->BeginInit();

			this->pnlPlaceholderView->SuspendLayout();
			this->SuspendLayout();

			// ----------------------------------------------------
			// 1. SIDEBAR (Fixed Left Navigation)
			// ----------------------------------------------------
			this->pnlSidebar->BackColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->pnlSidebar->Controls->Add(this->pnlNavButtons);
			this->pnlSidebar->Controls->Add(this->pnlSidebarFooter);
			this->pnlSidebar->Controls->Add(this->pnlBrand);
			this->pnlSidebar->Dock = System::Windows::Forms::DockStyle::Left;
			this->pnlSidebar->Location = System::Drawing::Point(0, 0);
			this->pnlSidebar->Name = L"pnlSidebar";
			this->pnlSidebar->Size = System::Drawing::Size(240, 720);
			this->pnlSidebar->TabIndex = 0;

			this->pnlBrand->BackColor = System::Drawing::Color::FromArgb(20, 40, 68);
			this->pnlBrand->Controls->Add(this->lblBrandSubtitle);
			this->pnlBrand->Controls->Add(this->lblBrandTitle);
			this->pnlBrand->Dock = System::Windows::Forms::DockStyle::Top;
			this->pnlBrand->Location = System::Drawing::Point(0, 0);
			this->pnlBrand->Name = L"pnlBrand";
			this->pnlBrand->Size = System::Drawing::Size(240, 75);
			this->pnlBrand->TabIndex = 0;

			this->lblBrandTitle->AutoSize = true;
			this->lblBrandTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 16, System::Drawing::FontStyle::Bold));
			this->lblBrandTitle->ForeColor = System::Drawing::Color::White;
			this->lblBrandTitle->Location = System::Drawing::Point(18, 14);
			this->lblBrandTitle->Name = L"lblBrandTitle";
			this->lblBrandTitle->Size = System::Drawing::Size(116, 30);
			this->lblBrandTitle->TabIndex = 0;
			this->lblBrandTitle->Text = L"FixSphere";

			this->lblBrandSubtitle->AutoSize = true;
			this->lblBrandSubtitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.25f));
			this->lblBrandSubtitle->ForeColor = System::Drawing::Color::FromArgb(168, 218, 220);
			this->lblBrandSubtitle->Location = System::Drawing::Point(20, 46);
			this->lblBrandSubtitle->Name = L"lblBrandSubtitle";
			this->lblBrandSubtitle->Size = System::Drawing::Size(158, 13);
			this->lblBrandSubtitle->TabIndex = 1;
			this->lblBrandSubtitle->Text = L"REPAIR CENTER MANAGEMENT";

			this->pnlNavButtons->Controls->Add(this->btnNavReports);
			this->pnlNavButtons->Controls->Add(this->btnNavEmployees);
			this->pnlNavButtons->Controls->Add(this->btnNavJobs);
			this->pnlNavButtons->Controls->Add(this->btnNavCustomers);
			this->pnlNavButtons->Controls->Add(this->btnNavDashboard);
			this->pnlNavButtons->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pnlNavButtons->Location = System::Drawing::Point(0, 75);
			this->pnlNavButtons->Name = L"pnlNavButtons";
			this->pnlNavButtons->Padding = System::Windows::Forms::Padding(0, 15, 0, 0);
			this->pnlNavButtons->Size = System::Drawing::Size(240, 595);
			this->pnlNavButtons->TabIndex = 1;

			this->btnNavDashboard->BackColor = System::Drawing::Color::FromArgb(69, 123, 157);
			this->btnNavDashboard->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnNavDashboard->Dock = System::Windows::Forms::DockStyle::Top;
			this->btnNavDashboard->FlatAppearance->BorderSize = 0;
			this->btnNavDashboard->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnNavDashboard->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10.5f, System::Drawing::FontStyle::Bold));
			this->btnNavDashboard->ForeColor = System::Drawing::Color::White;
			this->btnNavDashboard->Location = System::Drawing::Point(0, 15);
			this->btnNavDashboard->Name = L"btnNavDashboard";
			this->btnNavDashboard->Padding = System::Windows::Forms::Padding(22, 0, 0, 0);
			this->btnNavDashboard->Size = System::Drawing::Size(240, 48);
			this->btnNavDashboard->TabIndex = 0;
			this->btnNavDashboard->Text = L"  Dashboard";
			this->btnNavDashboard->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->btnNavDashboard->UseVisualStyleBackColor = false;
			this->btnNavDashboard->Click += gcnew System::EventHandler(this, &MainForm::btnNavDashboard_Click);

			this->btnNavCustomers->BackColor = System::Drawing::Color::Transparent;
			this->btnNavCustomers->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnNavCustomers->Dock = System::Windows::Forms::DockStyle::Top;
			this->btnNavCustomers->FlatAppearance->BorderSize = 0;
			this->btnNavCustomers->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnNavCustomers->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			this->btnNavCustomers->ForeColor = System::Drawing::Color::FromArgb(220, 230, 242);
			this->btnNavCustomers->Location = System::Drawing::Point(0, 63);
			this->btnNavCustomers->Name = L"btnNavCustomers";
			this->btnNavCustomers->Padding = System::Windows::Forms::Padding(22, 0, 0, 0);
			this->btnNavCustomers->Size = System::Drawing::Size(240, 48);
			this->btnNavCustomers->TabIndex = 1;
			this->btnNavCustomers->Text = L"  Customers";
			this->btnNavCustomers->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->btnNavCustomers->UseVisualStyleBackColor = false;
			this->btnNavCustomers->Click += gcnew System::EventHandler(this, &MainForm::btnNavCustomers_Click);

			this->btnNavJobs->BackColor = System::Drawing::Color::Transparent;
			this->btnNavJobs->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnNavJobs->Dock = System::Windows::Forms::DockStyle::Top;
			this->btnNavJobs->FlatAppearance->BorderSize = 0;
			this->btnNavJobs->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnNavJobs->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			this->btnNavJobs->ForeColor = System::Drawing::Color::FromArgb(220, 230, 242);
			this->btnNavJobs->Location = System::Drawing::Point(0, 111);
			this->btnNavJobs->Name = L"btnNavJobs";
			this->btnNavJobs->Padding = System::Windows::Forms::Padding(22, 0, 0, 0);
			this->btnNavJobs->Size = System::Drawing::Size(240, 48);
			this->btnNavJobs->TabIndex = 2;
			this->btnNavJobs->Text = L"  Repair Jobs";
			this->btnNavJobs->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->btnNavJobs->UseVisualStyleBackColor = false;
			this->btnNavJobs->Click += gcnew System::EventHandler(this, &MainForm::btnNavJobs_Click);

			this->btnNavEmployees->BackColor = System::Drawing::Color::Transparent;
			this->btnNavEmployees->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnNavEmployees->Dock = System::Windows::Forms::DockStyle::Top;
			this->btnNavEmployees->FlatAppearance->BorderSize = 0;
			this->btnNavEmployees->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnNavEmployees->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			this->btnNavEmployees->ForeColor = System::Drawing::Color::FromArgb(220, 230, 242);
			this->btnNavEmployees->Location = System::Drawing::Point(0, 159);
			this->btnNavEmployees->Name = L"btnNavEmployees";
			this->btnNavEmployees->Padding = System::Windows::Forms::Padding(22, 0, 0, 0);
			this->btnNavEmployees->Size = System::Drawing::Size(240, 48);
			this->btnNavEmployees->TabIndex = 3;
			this->btnNavEmployees->Text = L"  Employees";
			this->btnNavEmployees->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->btnNavEmployees->UseVisualStyleBackColor = false;
			this->btnNavEmployees->Click += gcnew System::EventHandler(this, &MainForm::btnNavEmployees_Click);

			this->btnNavReports->BackColor = System::Drawing::Color::Transparent;
			this->btnNavReports->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnNavReports->Dock = System::Windows::Forms::DockStyle::Top;
			this->btnNavReports->FlatAppearance->BorderSize = 0;
			this->btnNavReports->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnNavReports->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			this->btnNavReports->ForeColor = System::Drawing::Color::FromArgb(220, 230, 242);
			this->btnNavReports->Location = System::Drawing::Point(0, 207);
			this->btnNavReports->Name = L"btnNavReports";
			this->btnNavReports->Padding = System::Windows::Forms::Padding(22, 0, 0, 0);
			this->btnNavReports->Size = System::Drawing::Size(240, 48);
			this->btnNavReports->TabIndex = 4;
			this->btnNavReports->Text = L"  Reports & Income";
			this->btnNavReports->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->btnNavReports->UseVisualStyleBackColor = false;
			this->btnNavReports->Click += gcnew System::EventHandler(this, &MainForm::btnNavReports_Click);

			this->pnlSidebarFooter->BackColor = System::Drawing::Color::FromArgb(20, 40, 68);
			this->pnlSidebarFooter->Controls->Add(this->lblDbStatus);
			this->pnlSidebarFooter->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->pnlSidebarFooter->Location = System::Drawing::Point(0, 670);
			this->pnlSidebarFooter->Name = L"pnlSidebarFooter";
			this->pnlSidebarFooter->Size = System::Drawing::Size(240, 50);
			this->pnlSidebarFooter->TabIndex = 2;

			this->lblDbStatus->AutoSize = false;
			this->lblDbStatus->Dock = System::Windows::Forms::DockStyle::Fill;
			this->lblDbStatus->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.25f));
			this->lblDbStatus->ForeColor = System::Drawing::Color::FromArgb(168, 218, 220);
			this->lblDbStatus->Location = System::Drawing::Point(0, 0);
			this->lblDbStatus->Name = L"lblDbStatus";
			this->lblDbStatus->Padding = System::Windows::Forms::Padding(15, 0, 0, 0);
			this->lblDbStatus->Size = System::Drawing::Size(240, 50);
			this->lblDbStatus->TabIndex = 0;
			this->lblDbStatus->Text = L"MySQL: fixsphere (Active)\nReady | v1.0.0";
			this->lblDbStatus->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;

			// ----------------------------------------------------
			// 2. TOP HEADER
			// ----------------------------------------------------
			this->pnlHeader->BackColor = System::Drawing::Color::White;
			this->pnlHeader->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pnlHeader->Controls->Add(this->pnlUserBadge);
			this->pnlHeader->Controls->Add(this->lblViewTitle);
			this->pnlHeader->Dock = System::Windows::Forms::DockStyle::Top;
			this->pnlHeader->Location = System::Drawing::Point(240, 0);
			this->pnlHeader->Name = L"pnlHeader";
			this->pnlHeader->Size = System::Drawing::Size(960, 75);
			this->pnlHeader->TabIndex = 1;

			this->lblViewTitle->AutoSize = true;
			this->lblViewTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 14, System::Drawing::FontStyle::Bold));
			this->lblViewTitle->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblViewTitle->Location = System::Drawing::Point(25, 24);
			this->lblViewTitle->Name = L"lblViewTitle";
			this->lblViewTitle->Size = System::Drawing::Size(192, 25);
			this->lblViewTitle->TabIndex = 0;
			this->lblViewTitle->Text = L"Dashboard Overview";

			this->pnlUserBadge->Controls->Add(this->btnLogout);
			this->pnlUserBadge->Controls->Add(this->lblLoggedRole);
			this->pnlUserBadge->Controls->Add(this->lblLoggedUser);
			this->pnlUserBadge->Dock = System::Windows::Forms::DockStyle::Right;
			this->pnlUserBadge->Location = System::Drawing::Point(540, 0);
			this->pnlUserBadge->Name = L"pnlUserBadge";
			this->pnlUserBadge->Size = System::Drawing::Size(418, 73);
			this->pnlUserBadge->TabIndex = 1;

			this->lblLoggedUser->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
			this->lblLoggedUser->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblLoggedUser->Location = System::Drawing::Point(10, 14);
			this->lblLoggedUser->Name = L"lblLoggedUser";
			this->lblLoggedUser->Size = System::Drawing::Size(275, 22);
			this->lblLoggedUser->TabIndex = 0;
			this->lblLoggedUser->Text = L"Administrator (admin)";
			this->lblLoggedUser->TextAlign = System::Drawing::ContentAlignment::MiddleRight;

			this->lblLoggedRole->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.5f, System::Drawing::FontStyle::Bold));
			this->lblLoggedRole->ForeColor = System::Drawing::Color::FromArgb(69, 123, 157);
			this->lblLoggedRole->Location = System::Drawing::Point(10, 38);
			this->lblLoggedRole->Name = L"lblLoggedRole";
			this->lblLoggedRole->Size = System::Drawing::Size(275, 18);
			this->lblLoggedRole->TabIndex = 1;
			this->lblLoggedRole->Text = L"ROLE: ADMIN";
			this->lblLoggedRole->TextAlign = System::Drawing::ContentAlignment::MiddleRight;

			this->btnLogout->BackColor = System::Drawing::Color::FromArgb(244, 246, 249);
			this->btnLogout->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnLogout->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnLogout->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->btnLogout->ForeColor = System::Drawing::Color::FromArgb(220, 53, 69);
			this->btnLogout->Location = System::Drawing::Point(300, 18);
			this->btnLogout->Name = L"btnLogout";
			this->btnLogout->Size = System::Drawing::Size(95, 36);
			this->btnLogout->TabIndex = 2;
			this->btnLogout->Text = L"Sign Out";
			this->btnLogout->UseVisualStyleBackColor = false;
			this->btnLogout->Click += gcnew System::EventHandler(this, &MainForm::btnLogout_Click);

			// ----------------------------------------------------
			// 3. MAIN CONTENT CONTAINER
			// ----------------------------------------------------
			this->pnlMainContent->BackColor = System::Drawing::Color::FromArgb(244, 246, 249);
			this->pnlMainContent->Controls->Add(this->pnlViewDashboard);
			this->pnlMainContent->Controls->Add(this->pnlViewCustomers);
			this->pnlMainContent->Controls->Add(this->pnlViewJobs);
			this->pnlMainContent->Controls->Add(this->pnlViewEmployees);
			this->pnlMainContent->Controls->Add(this->pnlViewReports);
			this->pnlMainContent->Controls->Add(this->pnlPlaceholderView);
			this->pnlMainContent->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pnlMainContent->Location = System::Drawing::Point(240, 75);
			this->pnlMainContent->Name = L"pnlMainContent";
			this->pnlMainContent->Padding = System::Windows::Forms::Padding(25);
			this->pnlMainContent->Size = System::Drawing::Size(960, 645);
			this->pnlMainContent->TabIndex = 2;

			// ----------------------------------------------------
			// 4. DASHBOARD VIEW PANEL
			// ----------------------------------------------------
			this->pnlViewDashboard->Controls->Add(this->dgvRecentJobs);
			this->pnlViewDashboard->Controls->Add(this->pnlRecentJobsHeader);
			this->pnlViewDashboard->Controls->Add(this->flowStatCards);
			this->pnlViewDashboard->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pnlViewDashboard->Location = System::Drawing::Point(25, 25);
			this->pnlViewDashboard->Name = L"pnlViewDashboard";
			this->pnlViewDashboard->Size = System::Drawing::Size(910, 595);
			this->pnlViewDashboard->TabIndex = 0;

			this->flowStatCards->Dock = System::Windows::Forms::DockStyle::Top;
			this->flowStatCards->Location = System::Drawing::Point(0, 0);
			this->flowStatCards->Name = L"flowStatCards";
			this->flowStatCards->Size = System::Drawing::Size(910, 220);
			this->flowStatCards->TabIndex = 0;

			this->cardCustomers = CreateStatCard(L"Total Customers", this->lblStatCustVal, System::Drawing::Color::FromArgb(29, 53, 87), 180);
			this->cardJobsTotal = CreateStatCard(L"Total Repair Jobs", this->lblStatJobsVal, System::Drawing::Color::FromArgb(69, 123, 157), 180);
			this->cardJobsPending = CreateStatCard(L"Pending Jobs", this->lblStatPendingVal, System::Drawing::Color::FromArgb(243, 156, 18), 180);
			this->cardJobsRepairing = CreateStatCard(L"In Progress", this->lblStatRepairingVal, System::Drawing::Color::FromArgb(52, 152, 219), 180);
			this->cardJobsFinished = CreateStatCard(L"Finished Jobs", this->lblStatFinishedVal, System::Drawing::Color::FromArgb(40, 167, 69), 180);
			this->cardJobsFailed = CreateStatCard(L"Couldn't Repair", this->lblStatFailedVal, System::Drawing::Color::FromArgb(108, 117, 125), 180);
			this->cardIncome = CreateStatCard(L"Monthly Income", this->lblStatIncomeVal, System::Drawing::Color::FromArgb(29, 53, 87), 240);

			this->flowStatCards->Controls->Add(this->cardCustomers);
			this->flowStatCards->Controls->Add(this->cardJobsTotal);
			this->flowStatCards->Controls->Add(this->cardJobsPending);
			this->flowStatCards->Controls->Add(this->cardJobsRepairing);
			this->flowStatCards->Controls->Add(this->cardJobsFinished);
			this->flowStatCards->Controls->Add(this->cardJobsFailed);
			this->flowStatCards->Controls->Add(this->cardIncome);

			this->pnlRecentJobsHeader->Controls->Add(this->btnRefreshDashboard);
			this->pnlRecentJobsHeader->Controls->Add(this->lblRecentJobsTitle);
			this->pnlRecentJobsHeader->Dock = System::Windows::Forms::DockStyle::Top;
			this->pnlRecentJobsHeader->Location = System::Drawing::Point(0, 220);
			this->pnlRecentJobsHeader->Name = L"pnlRecentJobsHeader";
			this->pnlRecentJobsHeader->Padding = System::Windows::Forms::Padding(0, 10, 0, 5);
			this->pnlRecentJobsHeader->Size = System::Drawing::Size(910, 45);
			this->pnlRecentJobsHeader->TabIndex = 1;

			this->lblRecentJobsTitle->AutoSize = true;
			this->lblRecentJobsTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Bold));
			this->lblRecentJobsTitle->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblRecentJobsTitle->Location = System::Drawing::Point(2, 12);
			this->lblRecentJobsTitle->Name = L"lblRecentJobsTitle";
			this->lblRecentJobsTitle->Size = System::Drawing::Size(155, 21);
			this->lblRecentJobsTitle->TabIndex = 0;
			this->lblRecentJobsTitle->Text = L"Recent Repair Jobs";

			this->btnRefreshDashboard->BackColor = System::Drawing::Color::FromArgb(69, 123, 157);
			this->btnRefreshDashboard->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnRefreshDashboard->Dock = System::Windows::Forms::DockStyle::Right;
			this->btnRefreshDashboard->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnRefreshDashboard->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.5f, System::Drawing::FontStyle::Bold));
			this->btnRefreshDashboard->ForeColor = System::Drawing::Color::White;
			this->btnRefreshDashboard->Location = System::Drawing::Point(790, 10);
			this->btnRefreshDashboard->Name = L"btnRefreshDashboard";
			this->btnRefreshDashboard->Size = System::Drawing::Size(120, 30);
			this->btnRefreshDashboard->TabIndex = 1;
			this->btnRefreshDashboard->Text = L"Refresh Data";
			this->btnRefreshDashboard->UseVisualStyleBackColor = false;
			this->btnRefreshDashboard->Click += gcnew System::EventHandler(this, &MainForm::btnRefreshDashboard_Click);

			this->dgvRecentJobs->AllowUserToAddRows = false;
			this->dgvRecentJobs->AllowUserToDeleteRows = false;
			this->dgvRecentJobs->AllowUserToResizeRows = false;
			this->dgvRecentJobs->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::Fill;
			this->dgvRecentJobs->BackgroundColor = System::Drawing::Color::White;
			this->dgvRecentJobs->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->dgvRecentJobs->ColumnHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::Single;

			DataGridViewCellStyle^ headerStyle = gcnew DataGridViewCellStyle();
			headerStyle->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			headerStyle->BackColor = System::Drawing::Color::FromArgb(29, 53, 87);
			headerStyle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.25f, System::Drawing::FontStyle::Bold));
			headerStyle->ForeColor = System::Drawing::Color::White;
			headerStyle->SelectionBackColor = System::Drawing::Color::FromArgb(29, 53, 87);
			headerStyle->SelectionForeColor = System::Drawing::Color::White;
			headerStyle->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
			this->dgvRecentJobs->ColumnHeadersDefaultCellStyle = headerStyle;
			this->dgvRecentJobs->ColumnHeadersHeight = 36;
			this->dgvRecentJobs->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::DisableResizing;

			DataGridViewCellStyle^ rowStyle = gcnew DataGridViewCellStyle();
			rowStyle->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			rowStyle->BackColor = System::Drawing::Color::White;
			rowStyle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			rowStyle->ForeColor = System::Drawing::Color::FromArgb(33, 37, 41);
			rowStyle->SelectionBackColor = System::Drawing::Color::FromArgb(168, 218, 220);
			rowStyle->SelectionForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->dgvRecentJobs->DefaultCellStyle = rowStyle;

			this->dgvRecentJobs->Dock = System::Windows::Forms::DockStyle::Fill;
			this->dgvRecentJobs->EnableHeadersVisualStyles = false;
			this->dgvRecentJobs->GridColor = System::Drawing::Color::FromArgb(222, 226, 230);
			this->dgvRecentJobs->Location = System::Drawing::Point(0, 265);
			this->dgvRecentJobs->MultiSelect = false;
			this->dgvRecentJobs->Name = L"dgvRecentJobs";
			this->dgvRecentJobs->ReadOnly = true;
			this->dgvRecentJobs->RowHeadersVisible = false;
			this->dgvRecentJobs->RowTemplate->Height = 32;
			this->dgvRecentJobs->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dgvRecentJobs->Size = System::Drawing::Size(910, 330);
			this->dgvRecentJobs->TabIndex = 2;

			// ----------------------------------------------------
			// 5. CUSTOMER MANAGEMENT VIEW PANEL
			// ----------------------------------------------------
			this->pnlViewCustomers->Controls->Add(this->pnlCustTableCard);
			this->pnlViewCustomers->Controls->Add(this->pnlCustSpacer);
			this->pnlViewCustomers->Controls->Add(this->pnlCustFormCard);
			this->pnlViewCustomers->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pnlViewCustomers->Location = System::Drawing::Point(25, 25);
			this->pnlViewCustomers->Name = L"pnlViewCustomers";
			this->pnlViewCustomers->Size = System::Drawing::Size(910, 595);
			this->pnlViewCustomers->TabIndex = 1;
			this->pnlViewCustomers->Visible = false;

			this->pnlCustFormCard->BackColor = System::Drawing::Color::White;
			this->pnlCustFormCard->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pnlCustFormCard->Controls->Add(this->btnCustClear);
			this->pnlCustFormCard->Controls->Add(this->btnCustUpdate);
			this->pnlCustFormCard->Controls->Add(this->btnCustSave);
			this->pnlCustFormCard->Controls->Add(this->txtCustAddress);
			this->pnlCustFormCard->Controls->Add(this->lblCustAddress);
			this->pnlCustFormCard->Controls->Add(this->txtCustContact);
			this->pnlCustFormCard->Controls->Add(this->lblCustContact);
			this->pnlCustFormCard->Controls->Add(this->txtCustNic);
			this->pnlCustFormCard->Controls->Add(this->lblCustNic);
			this->pnlCustFormCard->Controls->Add(this->txtCustName);
			this->pnlCustFormCard->Controls->Add(this->lblCustName);
			this->pnlCustFormCard->Controls->Add(this->lblCustIdStatus);
			this->pnlCustFormCard->Controls->Add(this->lblCustFormHeader);
			this->pnlCustFormCard->Dock = System::Windows::Forms::DockStyle::Left;
			this->pnlCustFormCard->Location = System::Drawing::Point(0, 0);
			this->pnlCustFormCard->Name = L"pnlCustFormCard";
			this->pnlCustFormCard->Padding = System::Windows::Forms::Padding(18);
			this->pnlCustFormCard->Size = System::Drawing::Size(330, 595);
			this->pnlCustFormCard->TabIndex = 0;

			this->lblCustFormHeader->AutoSize = true;
			this->lblCustFormHeader->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Bold));
			this->lblCustFormHeader->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblCustFormHeader->Location = System::Drawing::Point(16, 16);
			this->lblCustFormHeader->Name = L"lblCustFormHeader";
			this->lblCustFormHeader->Size = System::Drawing::Size(175, 21);
			this->lblCustFormHeader->TabIndex = 0;
			this->lblCustFormHeader->Text = L"Customer Profile";

			this->lblCustIdStatus->AutoSize = true;
			this->lblCustIdStatus->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.5f, System::Drawing::FontStyle::Bold));
			this->lblCustIdStatus->ForeColor = System::Drawing::Color::FromArgb(69, 123, 157);
			this->lblCustIdStatus->Location = System::Drawing::Point(17, 42);
			this->lblCustIdStatus->Name = L"lblCustIdStatus";
			this->lblCustIdStatus->Size = System::Drawing::Size(142, 15);
			this->lblCustIdStatus->TabIndex = 1;
			this->lblCustIdStatus->Text = L"Mode: [New Registration]";

			this->lblCustName->AutoSize = true;
			this->lblCustName->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->lblCustName->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblCustName->Location = System::Drawing::Point(17, 72);
			this->lblCustName->Name = L"lblCustName";
			this->lblCustName->Size = System::Drawing::Size(72, 15);
			this->lblCustName->TabIndex = 2;
			this->lblCustName->Text = L"Full Name *";

			this->txtCustName->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f));
			this->txtCustName->Location = System::Drawing::Point(19, 91);
			this->txtCustName->Name = L"txtCustName";
			this->txtCustName->Size = System::Drawing::Size(290, 24);
			this->txtCustName->TabIndex = 3;

			this->lblCustNic->AutoSize = true;
			this->lblCustNic->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->lblCustNic->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblCustNic->Location = System::Drawing::Point(17, 125);
			this->lblCustNic->Name = L"lblCustNic";
			this->lblCustNic->Size = System::Drawing::Size(86, 15);
			this->lblCustNic->TabIndex = 4;
			this->lblCustNic->Text = L"NIC Number *";

			this->txtCustNic->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f));
			this->txtCustNic->Location = System::Drawing::Point(19, 144);
			this->txtCustNic->Name = L"txtCustNic";
			this->txtCustNic->Size = System::Drawing::Size(290, 24);
			this->txtCustNic->TabIndex = 5;

			this->lblCustContact->AutoSize = true;
			this->lblCustContact->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->lblCustContact->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblCustContact->Location = System::Drawing::Point(17, 178);
			this->lblCustContact->Name = L"lblCustContact";
			this->lblCustContact->Size = System::Drawing::Size(107, 15);
			this->lblCustContact->TabIndex = 6;
			this->lblCustContact->Text = L"Contact / Phone *";

			this->txtCustContact->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f));
			this->txtCustContact->Location = System::Drawing::Point(19, 197);
			this->txtCustContact->Name = L"txtCustContact";
			this->txtCustContact->Size = System::Drawing::Size(290, 24);
			this->txtCustContact->TabIndex = 7;

			this->lblCustAddress->AutoSize = true;
			this->lblCustAddress->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->lblCustAddress->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblCustAddress->Location = System::Drawing::Point(17, 231);
			this->lblCustAddress->Name = L"lblCustAddress";
			this->lblCustAddress->Size = System::Drawing::Size(60, 15);
			this->lblCustAddress->TabIndex = 8;
			this->lblCustAddress->Text = L"Address *";

			this->txtCustAddress->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f));
			this->txtCustAddress->Location = System::Drawing::Point(19, 250);
			this->txtCustAddress->Multiline = true;
			this->txtCustAddress->Name = L"txtCustAddress";
			this->txtCustAddress->Size = System::Drawing::Size(290, 60);
			this->txtCustAddress->TabIndex = 9;

			this->btnCustSave->BackColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->btnCustSave->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnCustSave->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnCustSave->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f, System::Drawing::FontStyle::Bold));
			this->btnCustSave->ForeColor = System::Drawing::Color::White;
			this->btnCustSave->Location = System::Drawing::Point(19, 330);
			this->btnCustSave->Name = L"btnCustSave";
			this->btnCustSave->Size = System::Drawing::Size(290, 38);
			this->btnCustSave->TabIndex = 10;
			this->btnCustSave->Text = L"Register Customer";
			this->btnCustSave->UseVisualStyleBackColor = false;
			this->btnCustSave->Click += gcnew System::EventHandler(this, &MainForm::btnCustSave_Click);

			this->btnCustUpdate->BackColor = System::Drawing::Color::FromArgb(69, 123, 157);
			this->btnCustUpdate->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnCustUpdate->Enabled = false;
			this->btnCustUpdate->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnCustUpdate->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f, System::Drawing::FontStyle::Bold));
			this->btnCustUpdate->ForeColor = System::Drawing::Color::White;
			this->btnCustUpdate->Location = System::Drawing::Point(19, 376);
			this->btnCustUpdate->Name = L"btnCustUpdate";
			this->btnCustUpdate->Size = System::Drawing::Size(290, 36);
			this->btnCustUpdate->TabIndex = 11;
			this->btnCustUpdate->Text = L"Update Details";
			this->btnCustUpdate->UseVisualStyleBackColor = false;
			this->btnCustUpdate->Click += gcnew System::EventHandler(this, &MainForm::btnCustUpdate_Click);

			this->btnCustClear->BackColor = System::Drawing::Color::FromArgb(238, 242, 246);
			this->btnCustClear->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnCustClear->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnCustClear->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			this->btnCustClear->ForeColor = System::Drawing::Color::FromArgb(69, 123, 157);
			this->btnCustClear->Location = System::Drawing::Point(19, 420);
			this->btnCustClear->Name = L"btnCustClear";
			this->btnCustClear->Size = System::Drawing::Size(290, 32);
			this->btnCustClear->TabIndex = 12;
			this->btnCustClear->Text = L"Clear Form";
			this->btnCustClear->UseVisualStyleBackColor = false;
			this->btnCustClear->Click += gcnew System::EventHandler(this, &MainForm::btnCustClear_Click);

			this->pnlCustSpacer->Dock = System::Windows::Forms::DockStyle::Left;
			this->pnlCustSpacer->Location = System::Drawing::Point(330, 0);
			this->pnlCustSpacer->Name = L"pnlCustSpacer";
			this->pnlCustSpacer->Size = System::Drawing::Size(15, 595);
			this->pnlCustSpacer->TabIndex = 1;

			this->pnlCustTableCard->BackColor = System::Drawing::Color::White;
			this->pnlCustTableCard->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pnlCustTableCard->Controls->Add(this->dgvCustomers);
			this->pnlCustTableCard->Controls->Add(this->pnlCustSearchHeader);
			this->pnlCustTableCard->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pnlCustTableCard->Location = System::Drawing::Point(345, 0);
			this->pnlCustTableCard->Name = L"pnlCustTableCard";
			this->pnlCustTableCard->Padding = System::Windows::Forms::Padding(12);
			this->pnlCustTableCard->Size = System::Drawing::Size(565, 595);
			this->pnlCustTableCard->TabIndex = 2;

			this->pnlCustSearchHeader->Controls->Add(this->lblCustCount);
			this->pnlCustSearchHeader->Controls->Add(this->btnCustReset);
			this->pnlCustSearchHeader->Controls->Add(this->btnCustSearch);
			this->pnlCustSearchHeader->Controls->Add(this->txtCustSearch);
			this->pnlCustSearchHeader->Controls->Add(this->lblCustSearch);
			this->pnlCustSearchHeader->Dock = System::Windows::Forms::DockStyle::Top;
			this->pnlCustSearchHeader->Location = System::Drawing::Point(12, 12);
			this->pnlCustSearchHeader->Name = L"pnlCustSearchHeader";
			this->pnlCustSearchHeader->Size = System::Drawing::Size(539, 45);
			this->pnlCustSearchHeader->TabIndex = 0;

			this->lblCustSearch->AutoSize = true;
			this->lblCustSearch->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->lblCustSearch->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblCustSearch->Location = System::Drawing::Point(2, 14);
			this->lblCustSearch->Name = L"lblCustSearch";
			this->lblCustSearch->Size = System::Drawing::Size(51, 15);
			this->lblCustSearch->TabIndex = 0;
			this->lblCustSearch->Text = L"Search:";

			this->txtCustSearch->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f));
			this->txtCustSearch->Location = System::Drawing::Point(58, 10);
			this->txtCustSearch->Name = L"txtCustSearch";
			this->txtCustSearch->Size = System::Drawing::Size(200, 24);
			this->txtCustSearch->TabIndex = 1;
			this->txtCustSearch->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MainForm::txtCustSearch_KeyDown);

			this->btnCustSearch->BackColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->btnCustSearch->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnCustSearch->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnCustSearch->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.5f, System::Drawing::FontStyle::Bold));
			this->btnCustSearch->ForeColor = System::Drawing::Color::White;
			this->btnCustSearch->Location = System::Drawing::Point(265, 8);
			this->btnCustSearch->Name = L"btnCustSearch";
			this->btnCustSearch->Size = System::Drawing::Size(75, 28);
			this->btnCustSearch->TabIndex = 2;
			this->btnCustSearch->Text = L"Search";
			this->btnCustSearch->UseVisualStyleBackColor = false;
			this->btnCustSearch->Click += gcnew System::EventHandler(this, &MainForm::btnCustSearch_Click);

			this->btnCustReset->BackColor = System::Drawing::Color::FromArgb(69, 123, 157);
			this->btnCustReset->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnCustReset->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnCustReset->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.5f));
			this->btnCustReset->ForeColor = System::Drawing::Color::White;
			this->btnCustReset->Location = System::Drawing::Point(346, 8);
			this->btnCustReset->Name = L"btnCustReset";
			this->btnCustReset->Size = System::Drawing::Size(75, 28);
			this->btnCustReset->TabIndex = 3;
			this->btnCustReset->Text = L"Reset";
			this->btnCustReset->UseVisualStyleBackColor = false;
			this->btnCustReset->Click += gcnew System::EventHandler(this, &MainForm::btnCustReset_Click);

			this->lblCustCount->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.5f, System::Drawing::FontStyle::Bold));
			this->lblCustCount->ForeColor = System::Drawing::Color::FromArgb(108, 117, 125);
			this->lblCustCount->Location = System::Drawing::Point(425, 12);
			this->lblCustCount->Name = L"lblCustCount";
			this->lblCustCount->Size = System::Drawing::Size(110, 20);
			this->lblCustCount->TabIndex = 4;
			this->lblCustCount->Text = L"Loading...";
			this->lblCustCount->TextAlign = System::Drawing::ContentAlignment::MiddleRight;

			this->dgvCustomers->AllowUserToAddRows = false;
			this->dgvCustomers->AllowUserToDeleteRows = false;
			this->dgvCustomers->AllowUserToResizeRows = false;
			this->dgvCustomers->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::Fill;
			this->dgvCustomers->BackgroundColor = System::Drawing::Color::White;
			this->dgvCustomers->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->dgvCustomers->ColumnHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::Single;

			DataGridViewCellStyle^ custHeaderStyle = gcnew DataGridViewCellStyle();
			custHeaderStyle->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			custHeaderStyle->BackColor = System::Drawing::Color::FromArgb(29, 53, 87);
			custHeaderStyle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.25f, System::Drawing::FontStyle::Bold));
			custHeaderStyle->ForeColor = System::Drawing::Color::White;
			custHeaderStyle->SelectionBackColor = System::Drawing::Color::FromArgb(29, 53, 87);
			custHeaderStyle->SelectionForeColor = System::Drawing::Color::White;
			custHeaderStyle->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
			this->dgvCustomers->ColumnHeadersDefaultCellStyle = custHeaderStyle;
			this->dgvCustomers->ColumnHeadersHeight = 35;
			this->dgvCustomers->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::DisableResizing;

			DataGridViewCellStyle^ custRowStyle = gcnew DataGridViewCellStyle();
			custRowStyle->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			custRowStyle->BackColor = System::Drawing::Color::White;
			custRowStyle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			custRowStyle->ForeColor = System::Drawing::Color::FromArgb(33, 37, 41);
			custRowStyle->SelectionBackColor = System::Drawing::Color::FromArgb(168, 218, 220);
			custRowStyle->SelectionForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->dgvCustomers->DefaultCellStyle = custRowStyle;

			this->dgvCustomers->Dock = System::Windows::Forms::DockStyle::Fill;
			this->dgvCustomers->EnableHeadersVisualStyles = false;
			this->dgvCustomers->GridColor = System::Drawing::Color::FromArgb(222, 226, 230);
			this->dgvCustomers->Location = System::Drawing::Point(12, 57);
			this->dgvCustomers->MultiSelect = false;
			this->dgvCustomers->Name = L"dgvCustomers";
			this->dgvCustomers->ReadOnly = true;
			this->dgvCustomers->RowHeadersVisible = false;
			this->dgvCustomers->RowTemplate->Height = 32;
			this->dgvCustomers->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dgvCustomers->Size = System::Drawing::Size(539, 524);
			this->dgvCustomers->TabIndex = 1;
			this->dgvCustomers->CellClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MainForm::dgvCustomers_CellClick);

			// ----------------------------------------------------
			// 6. REPAIR JOB MANAGEMENT VIEW PANEL
			// ----------------------------------------------------
			this->pnlViewJobs->Controls->Add(this->pnlJobTableCard);
			this->pnlViewJobs->Controls->Add(this->pnlJobSpacer);
			this->pnlViewJobs->Controls->Add(this->pnlJobFormCard);
			this->pnlViewJobs->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pnlViewJobs->Location = System::Drawing::Point(25, 25);
			this->pnlViewJobs->Name = L"pnlViewJobs";
			this->pnlViewJobs->Size = System::Drawing::Size(910, 595);
			this->pnlViewJobs->TabIndex = 2;
			this->pnlViewJobs->Visible = false;

			this->pnlJobFormCard->AutoScroll = true;
			this->pnlJobFormCard->BackColor = System::Drawing::Color::White;
			this->pnlJobFormCard->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pnlJobFormCard->Controls->Add(this->btnJobDelete);
			this->pnlJobFormCard->Controls->Add(this->btnJobClear);
			this->pnlJobFormCard->Controls->Add(this->btnJobUpdate);
			this->pnlJobFormCard->Controls->Add(this->btnJobSave);
			this->pnlJobFormCard->Controls->Add(this->txtJobCost);
			this->pnlJobFormCard->Controls->Add(this->lblJobCost);
			this->pnlJobFormCard->Controls->Add(this->cmbJobStatus);
			this->pnlJobFormCard->Controls->Add(this->lblJobStatus);
			this->pnlJobFormCard->Controls->Add(this->txtJobRepairerDisplay);
			this->pnlJobFormCard->Controls->Add(this->cmbJobRepairer);
			this->pnlJobFormCard->Controls->Add(this->lblJobRepairer);
			this->pnlJobFormCard->Controls->Add(this->txtJobDetails);
			this->pnlJobFormCard->Controls->Add(this->lblJobDetails);
			this->pnlJobFormCard->Controls->Add(this->txtJobPhoneModel);
			this->pnlJobFormCard->Controls->Add(this->lblJobPhoneModel);
			this->pnlJobFormCard->Controls->Add(this->cmbJobCustomer);
			this->pnlJobFormCard->Controls->Add(this->lblJobCustomer);
			this->pnlJobFormCard->Controls->Add(this->lblJobIdStatus);
			this->pnlJobFormCard->Controls->Add(this->lblJobFormHeader);
			this->pnlJobFormCard->Dock = System::Windows::Forms::DockStyle::Left;
			this->pnlJobFormCard->Location = System::Drawing::Point(0, 0);
			this->pnlJobFormCard->Name = L"pnlJobFormCard";
			this->pnlJobFormCard->Padding = System::Windows::Forms::Padding(18);
			this->pnlJobFormCard->Size = System::Drawing::Size(360, 595);
			this->pnlJobFormCard->TabIndex = 0;

			this->lblJobFormHeader->AutoSize = true;
			this->lblJobFormHeader->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Bold));
			this->lblJobFormHeader->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblJobFormHeader->Location = System::Drawing::Point(16, 16);
			this->lblJobFormHeader->Name = L"lblJobFormHeader";
			this->lblJobFormHeader->Size = System::Drawing::Size(178, 21);
			this->lblJobFormHeader->TabIndex = 0;
			this->lblJobFormHeader->Text = L"Repair Job Intake";

			this->lblJobIdStatus->AutoSize = true;
			this->lblJobIdStatus->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.5f, System::Drawing::FontStyle::Bold));
			this->lblJobIdStatus->ForeColor = System::Drawing::Color::FromArgb(69, 123, 157);
			this->lblJobIdStatus->Location = System::Drawing::Point(17, 40);
			this->lblJobIdStatus->Name = L"lblJobIdStatus";
			this->lblJobIdStatus->Size = System::Drawing::Size(126, 15);
			this->lblJobIdStatus->TabIndex = 1;
			this->lblJobIdStatus->Text = L"Mode: [New Job Intake]";

			this->lblJobCustomer->AutoSize = true;
			this->lblJobCustomer->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->lblJobCustomer->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblJobCustomer->Location = System::Drawing::Point(17, 68);
			this->lblJobCustomer->Name = L"lblJobCustomer";
			this->lblJobCustomer->Size = System::Drawing::Size(70, 15);
			this->lblJobCustomer->TabIndex = 2;
			this->lblJobCustomer->Text = L"Customer *";

			this->cmbJobCustomer->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbJobCustomer->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f));
			this->cmbJobCustomer->FormattingEnabled = true;
			this->cmbJobCustomer->Location = System::Drawing::Point(19, 87);
			this->cmbJobCustomer->Name = L"cmbJobCustomer";
			this->cmbJobCustomer->Size = System::Drawing::Size(320, 24);
			this->cmbJobCustomer->TabIndex = 3;

			this->lblJobPhoneModel->AutoSize = true;
			this->lblJobPhoneModel->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->lblJobPhoneModel->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblJobPhoneModel->Location = System::Drawing::Point(17, 120);
			this->lblJobPhoneModel->Name = L"lblJobPhoneModel";
			this->lblJobPhoneModel->Size = System::Drawing::Size(88, 15);
			this->lblJobPhoneModel->TabIndex = 4;
			this->lblJobPhoneModel->Text = L"Phone Model *";

			this->txtJobPhoneModel->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f));
			this->txtJobPhoneModel->Location = System::Drawing::Point(19, 138);
			this->txtJobPhoneModel->Name = L"txtJobPhoneModel";
			this->txtJobPhoneModel->Size = System::Drawing::Size(320, 24);
			this->txtJobPhoneModel->TabIndex = 5;

			this->lblJobDetails->AutoSize = true;
			this->lblJobDetails->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->lblJobDetails->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblJobDetails->Location = System::Drawing::Point(17, 171);
			this->lblJobDetails->Name = L"lblJobDetails";
			this->lblJobDetails->Size = System::Drawing::Size(158, 15);
			this->lblJobDetails->TabIndex = 6;
			this->lblJobDetails->Text = L"Reported Problem / Notes *";

			this->txtJobDetails->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f));
			this->txtJobDetails->Location = System::Drawing::Point(19, 189);
			this->txtJobDetails->Multiline = true;
			this->txtJobDetails->Name = L"txtJobDetails";
			this->txtJobDetails->Size = System::Drawing::Size(320, 55);
			this->txtJobDetails->TabIndex = 7;

			this->lblJobRepairer->AutoSize = true;
			this->lblJobRepairer->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->lblJobRepairer->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblJobRepairer->Location = System::Drawing::Point(17, 252);
			this->lblJobRepairer->Name = L"lblJobRepairer";
			this->lblJobRepairer->Size = System::Drawing::Size(107, 15);
			this->lblJobRepairer->TabIndex = 8;
			this->lblJobRepairer->Text = L"Assigned Repairer";

			this->cmbJobRepairer->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbJobRepairer->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f));
			this->cmbJobRepairer->FormattingEnabled = true;
			this->cmbJobRepairer->Location = System::Drawing::Point(19, 270);
			this->cmbJobRepairer->Name = L"cmbJobRepairer";
			this->cmbJobRepairer->Size = System::Drawing::Size(320, 24);
			this->cmbJobRepairer->TabIndex = 9;

			this->txtJobRepairerDisplay->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f));
			this->txtJobRepairerDisplay->Location = System::Drawing::Point(19, 270);
			this->txtJobRepairerDisplay->Name = L"txtJobRepairerDisplay";
			this->txtJobRepairerDisplay->Size = System::Drawing::Size(320, 24);
			this->txtJobRepairerDisplay->TabIndex = 9;
			this->txtJobRepairerDisplay->ReadOnly = true;
			this->txtJobRepairerDisplay->BackColor = System::Drawing::Color::FromArgb(244, 246, 249);
			this->txtJobRepairerDisplay->Text = L"Unassigned";
			this->txtJobRepairerDisplay->Visible = false;

			this->lblJobStatus->AutoSize = true;
			this->lblJobStatus->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->lblJobStatus->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblJobStatus->Location = System::Drawing::Point(17, 303);
			this->lblJobStatus->Name = L"lblJobStatus";
			this->lblJobStatus->Size = System::Drawing::Size(91, 15);
			this->lblJobStatus->TabIndex = 10;
			this->lblJobStatus->Text = L"Repair Status *";

			this->cmbJobStatus->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbJobStatus->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f));
			this->cmbJobStatus->FormattingEnabled = true;
			this->cmbJobStatus->Items->AddRange(gcnew cli::array< System::Object^  >(4) {
				L"Pending",
				L"In Progress",
				L"Finished",
				L"Couldn't Repair"
			});
			this->cmbJobStatus->Location = System::Drawing::Point(19, 321);
			this->cmbJobStatus->Name = L"cmbJobStatus";
			this->cmbJobStatus->Size = System::Drawing::Size(320, 24);
			this->cmbJobStatus->TabIndex = 11;
			this->cmbJobStatus->SelectedIndexChanged += gcnew System::EventHandler(this, &MainForm::cmbJobStatus_SelectedIndexChanged);

			this->lblJobCost->AutoSize = true;
			this->lblJobCost->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->lblJobCost->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblJobCost->Location = System::Drawing::Point(17, 354);
			this->lblJobCost->Name = L"lblJobCost";
			this->lblJobCost->Size = System::Drawing::Size(95, 15);
			this->lblJobCost->TabIndex = 12;
			this->lblJobCost->Text = L"Repair Cost (LKR) *";

			this->txtJobCost->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f));
			this->txtJobCost->Location = System::Drawing::Point(19, 372);
			this->txtJobCost->Name = L"txtJobCost";
			this->txtJobCost->Size = System::Drawing::Size(320, 24);
			this->txtJobCost->TabIndex = 13;
			this->txtJobCost->Text = L"0.00";

			this->btnJobSave->BackColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->btnJobSave->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnJobSave->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnJobSave->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f, System::Drawing::FontStyle::Bold));
			this->btnJobSave->ForeColor = System::Drawing::Color::White;
			this->btnJobSave->Location = System::Drawing::Point(19, 412);
			this->btnJobSave->Name = L"btnJobSave";
			this->btnJobSave->Size = System::Drawing::Size(320, 36);
			this->btnJobSave->TabIndex = 14;
			this->btnJobSave->Text = L"Create Repair Job";
			this->btnJobSave->UseVisualStyleBackColor = false;
			this->btnJobSave->Click += gcnew System::EventHandler(this, &MainForm::btnJobSave_Click);

			this->btnJobUpdate->BackColor = System::Drawing::Color::FromArgb(69, 123, 157);
			this->btnJobUpdate->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnJobUpdate->Enabled = false;
			this->btnJobUpdate->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnJobUpdate->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f, System::Drawing::FontStyle::Bold));
			this->btnJobUpdate->ForeColor = System::Drawing::Color::White;
			this->btnJobUpdate->Location = System::Drawing::Point(19, 454);
			this->btnJobUpdate->Name = L"btnJobUpdate";
			this->btnJobUpdate->Size = System::Drawing::Size(320, 36);
			this->btnJobUpdate->TabIndex = 15;
			this->btnJobUpdate->Text = L"Update Job Status";
			this->btnJobUpdate->UseVisualStyleBackColor = false;
			this->btnJobUpdate->Click += gcnew System::EventHandler(this, &MainForm::btnJobUpdate_Click);

			this->btnJobDelete->BackColor = System::Drawing::Color::FromArgb(220, 53, 69);
			this->btnJobDelete->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnJobDelete->Enabled = false;
			this->btnJobDelete->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnJobDelete->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->btnJobDelete->ForeColor = System::Drawing::Color::White;
			this->btnJobDelete->Location = System::Drawing::Point(19, 496);
			this->btnJobDelete->Name = L"btnJobDelete";
			this->btnJobDelete->Size = System::Drawing::Size(155, 32);
			this->btnJobDelete->TabIndex = 16;
			this->btnJobDelete->Text = L"Delete Job";
			this->btnJobDelete->UseVisualStyleBackColor = false;
			this->btnJobDelete->Click += gcnew System::EventHandler(this, &MainForm::btnJobDelete_Click);

			this->btnJobClear->BackColor = System::Drawing::Color::FromArgb(238, 242, 246);
			this->btnJobClear->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnJobClear->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnJobClear->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			this->btnJobClear->ForeColor = System::Drawing::Color::FromArgb(69, 123, 157);
			this->btnJobClear->Location = System::Drawing::Point(184, 496);
			this->btnJobClear->Name = L"btnJobClear";
			this->btnJobClear->Size = System::Drawing::Size(155, 32);
			this->btnJobClear->TabIndex = 17;
			this->btnJobClear->Text = L"Clear Form";
			this->btnJobClear->UseVisualStyleBackColor = false;
			this->btnJobClear->Click += gcnew System::EventHandler(this, &MainForm::btnJobClear_Click);

			this->pnlJobSpacer->Dock = System::Windows::Forms::DockStyle::Left;
			this->pnlJobSpacer->Location = System::Drawing::Point(360, 0);
			this->pnlJobSpacer->Name = L"pnlJobSpacer";
			this->pnlJobSpacer->Size = System::Drawing::Size(15, 595);
			this->pnlJobSpacer->TabIndex = 1;

			this->pnlJobTableCard->BackColor = System::Drawing::Color::White;
			this->pnlJobTableCard->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pnlJobTableCard->Controls->Add(this->dgvJobs);
			this->pnlJobTableCard->Controls->Add(this->pnlJobSearchHeader);
			this->pnlJobTableCard->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pnlJobTableCard->Location = System::Drawing::Point(375, 0);
			this->pnlJobTableCard->Name = L"pnlJobTableCard";
			this->pnlJobTableCard->Padding = System::Windows::Forms::Padding(12);
			this->pnlJobTableCard->Size = System::Drawing::Size(535, 595);
			this->pnlJobTableCard->TabIndex = 2;

			this->pnlJobSearchHeader->Controls->Add(this->lblJobCount);
			this->pnlJobSearchHeader->Controls->Add(this->btnJobReset);
			this->pnlJobSearchHeader->Controls->Add(this->btnJobSearch);
			this->pnlJobSearchHeader->Controls->Add(this->cmbFilterStatus);
			this->pnlJobSearchHeader->Controls->Add(this->lblFilterStatus);
			this->pnlJobSearchHeader->Controls->Add(this->txtJobSearch);
			this->pnlJobSearchHeader->Controls->Add(this->lblJobSearch);
			this->pnlJobSearchHeader->Dock = System::Windows::Forms::DockStyle::Top;
			this->pnlJobSearchHeader->Location = System::Drawing::Point(12, 12);
			this->pnlJobSearchHeader->Name = L"pnlJobSearchHeader";
			this->pnlJobSearchHeader->Size = System::Drawing::Size(509, 45);
			this->pnlJobSearchHeader->TabIndex = 0;

			this->lblJobSearch->AutoSize = true;
			this->lblJobSearch->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->lblJobSearch->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblJobSearch->Location = System::Drawing::Point(2, 14);
			this->lblJobSearch->Name = L"lblJobSearch";
			this->lblJobSearch->Size = System::Drawing::Size(48, 15);
			this->lblJobSearch->TabIndex = 0;
			this->lblJobSearch->Text = L"Search:";

			this->txtJobSearch->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f));
			this->txtJobSearch->Location = System::Drawing::Point(52, 10);
			this->txtJobSearch->Name = L"txtJobSearch";
			this->txtJobSearch->Size = System::Drawing::Size(130, 24);
			this->txtJobSearch->TabIndex = 1;
			this->txtJobSearch->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MainForm::txtJobSearch_KeyDown);

			this->lblFilterStatus->AutoSize = true;
			this->lblFilterStatus->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->lblFilterStatus->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblFilterStatus->Location = System::Drawing::Point(190, 14);
			this->lblFilterStatus->Name = L"lblFilterStatus";
			this->lblFilterStatus->Size = System::Drawing::Size(45, 15);
			this->lblFilterStatus->TabIndex = 2;
			this->lblFilterStatus->Text = L"Status:";

			this->cmbFilterStatus->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbFilterStatus->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.0f));
			this->cmbFilterStatus->FormattingEnabled = true;
			this->cmbFilterStatus->Items->AddRange(gcnew cli::array< System::Object^  >(5) {
				L"All Statuses",
				L"Pending",
				L"In Progress",
				L"Finished",
				L"Couldn't Repair"
			});
			this->cmbFilterStatus->Location = System::Drawing::Point(238, 11);
			this->cmbFilterStatus->Name = L"cmbFilterStatus";
			this->cmbFilterStatus->Size = System::Drawing::Size(115, 23);
			this->cmbFilterStatus->TabIndex = 3;

			this->btnJobSearch->BackColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->btnJobSearch->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnJobSearch->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnJobSearch->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.5f, System::Drawing::FontStyle::Bold));
			this->btnJobSearch->ForeColor = System::Drawing::Color::White;
			this->btnJobSearch->Location = System::Drawing::Point(360, 9);
			this->btnJobSearch->Name = L"btnJobSearch";
			this->btnJobSearch->Size = System::Drawing::Size(60, 27);
			this->btnJobSearch->TabIndex = 4;
			this->btnJobSearch->Text = L"Filter";
			this->btnJobSearch->UseVisualStyleBackColor = false;
			this->btnJobSearch->Click += gcnew System::EventHandler(this, &MainForm::btnJobSearch_Click);

			this->btnJobReset->BackColor = System::Drawing::Color::FromArgb(69, 123, 157);
			this->btnJobReset->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnJobReset->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnJobReset->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.5f));
			this->btnJobReset->ForeColor = System::Drawing::Color::White;
			this->btnJobReset->Location = System::Drawing::Point(425, 9);
			this->btnJobReset->Name = L"btnJobReset";
			this->btnJobReset->Size = System::Drawing::Size(55, 27);
			this->btnJobReset->TabIndex = 5;
			this->btnJobReset->Text = L"Reset";
			this->btnJobReset->UseVisualStyleBackColor = false;
			this->btnJobReset->Click += gcnew System::EventHandler(this, &MainForm::btnJobReset_Click);

			this->lblJobCount->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.5f, System::Drawing::FontStyle::Bold));
			this->lblJobCount->ForeColor = System::Drawing::Color::FromArgb(108, 117, 125);
			this->lblJobCount->Location = System::Drawing::Point(485, 12);
			this->lblJobCount->Name = L"lblJobCount";
			this->lblJobCount->Size = System::Drawing::Size(20, 20);
			this->lblJobCount->TabIndex = 6;
			this->lblJobCount->Text = L"";

			this->dgvJobs->AllowUserToAddRows = false;
			this->dgvJobs->AllowUserToDeleteRows = false;
			this->dgvJobs->AllowUserToResizeRows = false;
			this->dgvJobs->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::Fill;
			this->dgvJobs->BackgroundColor = System::Drawing::Color::White;
			this->dgvJobs->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->dgvJobs->ColumnHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::Single;

			DataGridViewCellStyle^ jobHeaderStyle = gcnew DataGridViewCellStyle();
			jobHeaderStyle->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			jobHeaderStyle->BackColor = System::Drawing::Color::FromArgb(29, 53, 87);
			jobHeaderStyle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.25f, System::Drawing::FontStyle::Bold));
			jobHeaderStyle->ForeColor = System::Drawing::Color::White;
			jobHeaderStyle->SelectionBackColor = System::Drawing::Color::FromArgb(29, 53, 87);
			jobHeaderStyle->SelectionForeColor = System::Drawing::Color::White;
			jobHeaderStyle->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
			this->dgvJobs->ColumnHeadersDefaultCellStyle = jobHeaderStyle;
			this->dgvJobs->ColumnHeadersHeight = 35;
			this->dgvJobs->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::DisableResizing;

			DataGridViewCellStyle^ jobRowStyle = gcnew DataGridViewCellStyle();
			jobRowStyle->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			jobRowStyle->BackColor = System::Drawing::Color::White;
			jobRowStyle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			jobRowStyle->ForeColor = System::Drawing::Color::FromArgb(33, 37, 41);
			jobRowStyle->SelectionBackColor = System::Drawing::Color::FromArgb(168, 218, 220);
			jobRowStyle->SelectionForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->dgvJobs->DefaultCellStyle = jobRowStyle;

			this->dgvJobs->Dock = System::Windows::Forms::DockStyle::Fill;
			this->dgvJobs->EnableHeadersVisualStyles = false;
			this->dgvJobs->GridColor = System::Drawing::Color::FromArgb(222, 226, 230);
			this->dgvJobs->Location = System::Drawing::Point(12, 57);
			this->dgvJobs->MultiSelect = false;
			this->dgvJobs->Name = L"dgvJobs";
			this->dgvJobs->ReadOnly = true;
			this->dgvJobs->RowHeadersVisible = false;
			this->dgvJobs->RowTemplate->Height = 32;
			this->dgvJobs->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dgvJobs->Size = System::Drawing::Size(509, 524);
			this->dgvJobs->TabIndex = 1;
			this->dgvJobs->CellClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MainForm::dgvJobs_CellClick);

			// ----------------------------------------------------
			// 7. EMPLOYEE MANAGEMENT VIEW PANEL (Phase 9)
			// ----------------------------------------------------
			this->pnlViewEmployees->Controls->Add(this->pnlEmpTableCard);
			this->pnlViewEmployees->Controls->Add(this->pnlEmpSpacer);
			this->pnlViewEmployees->Controls->Add(this->pnlEmpFormCard);
			this->pnlViewEmployees->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pnlViewEmployees->Location = System::Drawing::Point(25, 25);
			this->pnlViewEmployees->Name = L"pnlViewEmployees";
			this->pnlViewEmployees->Size = System::Drawing::Size(910, 595);
			this->pnlViewEmployees->TabIndex = 3;
			this->pnlViewEmployees->Visible = false;

			this->pnlEmpFormCard->AutoScroll = true;
			this->pnlEmpFormCard->BackColor = System::Drawing::Color::White;
			this->pnlEmpFormCard->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pnlEmpFormCard->Controls->Add(this->btnEmpClear);
			this->pnlEmpFormCard->Controls->Add(this->btnEmpToggleActive);
			this->pnlEmpFormCard->Controls->Add(this->btnEmpResetPass);
			this->pnlEmpFormCard->Controls->Add(this->btnEmpUpdate);
			this->pnlEmpFormCard->Controls->Add(this->btnEmpSave);
			this->pnlEmpFormCard->Controls->Add(this->chkEmpShowPass);
			this->pnlEmpFormCard->Controls->Add(this->txtEmpPassword);
			this->pnlEmpFormCard->Controls->Add(this->lblEmpPassword);
			this->pnlEmpFormCard->Controls->Add(this->txtEmpUsername);
			this->pnlEmpFormCard->Controls->Add(this->lblEmpUsername);
			this->pnlEmpFormCard->Controls->Add(this->chkEmpActive);
			this->pnlEmpFormCard->Controls->Add(this->cmbEmpRole);
			this->pnlEmpFormCard->Controls->Add(this->lblEmpRole);
			this->pnlEmpFormCard->Controls->Add(this->txtEmpAddress);
			this->pnlEmpFormCard->Controls->Add(this->lblEmpAddress);
			this->pnlEmpFormCard->Controls->Add(this->txtEmpContact);
			this->pnlEmpFormCard->Controls->Add(this->lblEmpContact);
			this->pnlEmpFormCard->Controls->Add(this->txtEmpNic);
			this->pnlEmpFormCard->Controls->Add(this->lblEmpNic);
			this->pnlEmpFormCard->Controls->Add(this->txtEmpName);
			this->pnlEmpFormCard->Controls->Add(this->lblEmpName);
			this->pnlEmpFormCard->Controls->Add(this->lblEmpIdStatus);
			this->pnlEmpFormCard->Controls->Add(this->lblEmpFormHeader);
			this->pnlEmpFormCard->Dock = System::Windows::Forms::DockStyle::Left;
			this->pnlEmpFormCard->Location = System::Drawing::Point(0, 0);
			this->pnlEmpFormCard->Name = L"pnlEmpFormCard";
			this->pnlEmpFormCard->Padding = System::Windows::Forms::Padding(18);
			this->pnlEmpFormCard->Size = System::Drawing::Size(360, 595);
			this->pnlEmpFormCard->TabIndex = 0;

			this->lblEmpFormHeader->AutoSize = true;
			this->lblEmpFormHeader->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Bold));
			this->lblEmpFormHeader->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblEmpFormHeader->Location = System::Drawing::Point(16, 16);
			this->lblEmpFormHeader->Name = L"lblEmpFormHeader";
			this->lblEmpFormHeader->Size = System::Drawing::Size(217, 21);
			this->lblEmpFormHeader->TabIndex = 0;
			this->lblEmpFormHeader->Text = L"Employee Profile & Security";

			this->lblEmpIdStatus->AutoSize = true;
			this->lblEmpIdStatus->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.5f, System::Drawing::FontStyle::Bold));
			this->lblEmpIdStatus->ForeColor = System::Drawing::Color::FromArgb(69, 123, 157);
			this->lblEmpIdStatus->Location = System::Drawing::Point(17, 40);
			this->lblEmpIdStatus->Name = L"lblEmpIdStatus";
			this->lblEmpIdStatus->Size = System::Drawing::Size(155, 15);
			this->lblEmpIdStatus->TabIndex = 1;
			this->lblEmpIdStatus->Text = L"Mode: [New Registration]";

			this->lblEmpName->AutoSize = true;
			this->lblEmpName->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->lblEmpName->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblEmpName->Location = System::Drawing::Point(17, 65);
			this->lblEmpName->Name = L"lblEmpName";
			this->lblEmpName->Size = System::Drawing::Size(72, 15);
			this->lblEmpName->TabIndex = 2;
			this->lblEmpName->Text = L"Full Name *";

			this->txtEmpName->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f));
			this->txtEmpName->Location = System::Drawing::Point(19, 83);
			this->txtEmpName->Name = L"txtEmpName";
			this->txtEmpName->Size = System::Drawing::Size(320, 24);
			this->txtEmpName->TabIndex = 3;

			this->lblEmpNic->AutoSize = true;
			this->lblEmpNic->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->lblEmpNic->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblEmpNic->Location = System::Drawing::Point(17, 112);
			this->lblEmpNic->Name = L"lblEmpNic";
			this->lblEmpNic->Size = System::Drawing::Size(86, 15);
			this->lblEmpNic->TabIndex = 4;
			this->lblEmpNic->Text = L"NIC Number *";

			this->txtEmpNic->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f));
			this->txtEmpNic->Location = System::Drawing::Point(19, 130);
			this->txtEmpNic->Name = L"txtEmpNic";
			this->txtEmpNic->Size = System::Drawing::Size(320, 24);
			this->txtEmpNic->TabIndex = 5;

			this->lblEmpContact->AutoSize = true;
			this->lblEmpContact->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->lblEmpContact->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblEmpContact->Location = System::Drawing::Point(17, 159);
			this->lblEmpContact->Name = L"lblEmpContact";
			this->lblEmpContact->Size = System::Drawing::Size(107, 15);
			this->lblEmpContact->TabIndex = 6;
			this->lblEmpContact->Text = L"Contact / Phone *";

			this->txtEmpContact->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f));
			this->txtEmpContact->Location = System::Drawing::Point(19, 177);
			this->txtEmpContact->Name = L"txtEmpContact";
			this->txtEmpContact->Size = System::Drawing::Size(320, 24);
			this->txtEmpContact->TabIndex = 7;

			this->lblEmpAddress->AutoSize = true;
			this->lblEmpAddress->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->lblEmpAddress->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblEmpAddress->Location = System::Drawing::Point(17, 206);
			this->lblEmpAddress->Name = L"lblEmpAddress";
			this->lblEmpAddress->Size = System::Drawing::Size(60, 15);
			this->lblEmpAddress->TabIndex = 8;
			this->lblEmpAddress->Text = L"Address *";

			this->txtEmpAddress->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f));
			this->txtEmpAddress->Location = System::Drawing::Point(19, 224);
			this->txtEmpAddress->Name = L"txtEmpAddress";
			this->txtEmpAddress->Size = System::Drawing::Size(320, 24);
			this->txtEmpAddress->TabIndex = 9;

			this->lblEmpRole->AutoSize = true;
			this->lblEmpRole->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->lblEmpRole->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblEmpRole->Location = System::Drawing::Point(17, 255);
			this->lblEmpRole->Name = L"lblEmpRole";
			this->lblEmpRole->Size = System::Drawing::Size(97, 15);
			this->lblEmpRole->TabIndex = 10;
			this->lblEmpRole->Text = L"Assigned Role *";

			this->cmbEmpRole->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbEmpRole->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f));
			this->cmbEmpRole->FormattingEnabled = true;
			this->cmbEmpRole->Items->AddRange(gcnew cli::array< System::Object^  >(3) {
				L"Admin",
				L"Checker",
				L"Repairer"
			});
			this->cmbEmpRole->Location = System::Drawing::Point(19, 273);
			this->cmbEmpRole->Name = L"cmbEmpRole";
			this->cmbEmpRole->Size = System::Drawing::Size(170, 24);
			this->cmbEmpRole->TabIndex = 11;

			this->chkEmpActive->AutoSize = true;
			this->chkEmpActive->Checked = true;
			this->chkEmpActive->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkEmpActive->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f, System::Drawing::FontStyle::Bold));
			this->chkEmpActive->ForeColor = System::Drawing::Color::FromArgb(21, 87, 36);
			this->chkEmpActive->Location = System::Drawing::Point(205, 275);
			this->chkEmpActive->Name = L"chkEmpActive";
			this->chkEmpActive->Size = System::Drawing::Size(117, 21);
			this->chkEmpActive->TabIndex = 12;
			this->chkEmpActive->Text = L"Account Active";
			this->chkEmpActive->UseVisualStyleBackColor = true;

			this->lblEmpUsername->AutoSize = true;
			this->lblEmpUsername->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->lblEmpUsername->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblEmpUsername->Location = System::Drawing::Point(17, 305);
			this->lblEmpUsername->Name = L"lblEmpUsername";
			this->lblEmpUsername->Size = System::Drawing::Size(107, 15);
			this->lblEmpUsername->TabIndex = 13;
			this->lblEmpUsername->Text = L"Login Username *";

			this->txtEmpUsername->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f));
			this->txtEmpUsername->Location = System::Drawing::Point(19, 323);
			this->txtEmpUsername->Name = L"txtEmpUsername";
			this->txtEmpUsername->Size = System::Drawing::Size(320, 24);
			this->txtEmpUsername->TabIndex = 14;

			this->lblEmpPassword->AutoSize = true;
			this->lblEmpPassword->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->lblEmpPassword->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblEmpPassword->Location = System::Drawing::Point(17, 354);
			this->lblEmpPassword->Name = L"lblEmpPassword";
			this->lblEmpPassword->Size = System::Drawing::Size(176, 15);
			this->lblEmpPassword->TabIndex = 15;
			this->lblEmpPassword->Text = L"Password (min 6 characters) *";

			this->txtEmpPassword->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f));
			this->txtEmpPassword->Location = System::Drawing::Point(19, 372);
			this->txtEmpPassword->Name = L"txtEmpPassword";
			this->txtEmpPassword->PasswordChar = '*';
			this->txtEmpPassword->Size = System::Drawing::Size(320, 24);
			this->txtEmpPassword->TabIndex = 16;

			this->chkEmpShowPass->AutoSize = true;
			this->chkEmpShowPass->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.25f));
			this->chkEmpShowPass->ForeColor = System::Drawing::Color::FromArgb(108, 117, 125);
			this->chkEmpShowPass->Location = System::Drawing::Point(19, 400);
			this->chkEmpShowPass->Name = L"chkEmpShowPass";
			this->chkEmpShowPass->Size = System::Drawing::Size(106, 17);
			this->chkEmpShowPass->TabIndex = 17;
			this->chkEmpShowPass->Text = L"Show password";
			this->chkEmpShowPass->UseVisualStyleBackColor = true;
			this->chkEmpShowPass->CheckedChanged += gcnew System::EventHandler(this, &MainForm::chkEmpShowPass_CheckedChanged);

			this->btnEmpSave->BackColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->btnEmpSave->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnEmpSave->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnEmpSave->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f, System::Drawing::FontStyle::Bold));
			this->btnEmpSave->ForeColor = System::Drawing::Color::White;
			this->btnEmpSave->Location = System::Drawing::Point(19, 426);
			this->btnEmpSave->Name = L"btnEmpSave";
			this->btnEmpSave->Size = System::Drawing::Size(320, 36);
			this->btnEmpSave->TabIndex = 18;
			this->btnEmpSave->Text = L"Register Employee";
			this->btnEmpSave->UseVisualStyleBackColor = false;
			this->btnEmpSave->Click += gcnew System::EventHandler(this, &MainForm::btnEmpSave_Click);

			this->btnEmpUpdate->BackColor = System::Drawing::Color::FromArgb(69, 123, 157);
			this->btnEmpUpdate->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnEmpUpdate->Enabled = false;
			this->btnEmpUpdate->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnEmpUpdate->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f, System::Drawing::FontStyle::Bold));
			this->btnEmpUpdate->ForeColor = System::Drawing::Color::White;
			this->btnEmpUpdate->Location = System::Drawing::Point(19, 468);
			this->btnEmpUpdate->Name = L"btnEmpUpdate";
			this->btnEmpUpdate->Size = System::Drawing::Size(320, 34);
			this->btnEmpUpdate->TabIndex = 19;
			this->btnEmpUpdate->Text = L"Update Details";
			this->btnEmpUpdate->UseVisualStyleBackColor = false;
			this->btnEmpUpdate->Click += gcnew System::EventHandler(this, &MainForm::btnEmpUpdate_Click);

			this->btnEmpResetPass->BackColor = System::Drawing::Color::FromArgb(243, 156, 18);
			this->btnEmpResetPass->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnEmpResetPass->Enabled = false;
			this->btnEmpResetPass->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnEmpResetPass->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.5f, System::Drawing::FontStyle::Bold));
			this->btnEmpResetPass->ForeColor = System::Drawing::Color::White;
			this->btnEmpResetPass->Location = System::Drawing::Point(19, 508);
			this->btnEmpResetPass->Name = L"btnEmpResetPass";
			this->btnEmpResetPass->Size = System::Drawing::Size(155, 32);
			this->btnEmpResetPass->TabIndex = 20;
			this->btnEmpResetPass->Text = L"Reset Password";
			this->btnEmpResetPass->UseVisualStyleBackColor = false;
			this->btnEmpResetPass->Click += gcnew System::EventHandler(this, &MainForm::btnEmpResetPass_Click);

			this->btnEmpToggleActive->BackColor = System::Drawing::Color::FromArgb(192, 57, 43);
			this->btnEmpToggleActive->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnEmpToggleActive->Enabled = false;
			this->btnEmpToggleActive->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnEmpToggleActive->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.5f, System::Drawing::FontStyle::Bold));
			this->btnEmpToggleActive->ForeColor = System::Drawing::Color::White;
			this->btnEmpToggleActive->Location = System::Drawing::Point(184, 508);
			this->btnEmpToggleActive->Name = L"btnEmpToggleActive";
			this->btnEmpToggleActive->Size = System::Drawing::Size(155, 32);
			this->btnEmpToggleActive->TabIndex = 21;
			this->btnEmpToggleActive->Text = L"Deactivate";
			this->btnEmpToggleActive->UseVisualStyleBackColor = false;
			this->btnEmpToggleActive->Click += gcnew System::EventHandler(this, &MainForm::btnEmpToggleActive_Click);

			this->btnEmpClear->BackColor = System::Drawing::Color::FromArgb(238, 242, 246);
			this->btnEmpClear->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnEmpClear->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnEmpClear->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			this->btnEmpClear->ForeColor = System::Drawing::Color::FromArgb(69, 123, 157);
			this->btnEmpClear->Location = System::Drawing::Point(19, 546);
			this->btnEmpClear->Name = L"btnEmpClear";
			this->btnEmpClear->Size = System::Drawing::Size(320, 30);
			this->btnEmpClear->TabIndex = 22;
			this->btnEmpClear->Text = L"Clear Form";
			this->btnEmpClear->UseVisualStyleBackColor = false;
			this->btnEmpClear->Click += gcnew System::EventHandler(this, &MainForm::btnEmpClear_Click);

			this->pnlEmpSpacer->Dock = System::Windows::Forms::DockStyle::Left;
			this->pnlEmpSpacer->Location = System::Drawing::Point(360, 0);
			this->pnlEmpSpacer->Name = L"pnlEmpSpacer";
			this->pnlEmpSpacer->Size = System::Drawing::Size(15, 595);
			this->pnlEmpSpacer->TabIndex = 1;

			this->pnlEmpTableCard->BackColor = System::Drawing::Color::White;
			this->pnlEmpTableCard->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pnlEmpTableCard->Controls->Add(this->dgvEmployees);
			this->pnlEmpTableCard->Controls->Add(this->pnlEmpSearchHeader);
			this->pnlEmpTableCard->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pnlEmpTableCard->Location = System::Drawing::Point(375, 0);
			this->pnlEmpTableCard->Name = L"pnlEmpTableCard";
			this->pnlEmpTableCard->Padding = System::Windows::Forms::Padding(12);
			this->pnlEmpTableCard->Size = System::Drawing::Size(535, 595);
			this->pnlEmpTableCard->TabIndex = 2;

			this->pnlEmpSearchHeader->Controls->Add(this->lblEmpCount);
			this->pnlEmpSearchHeader->Controls->Add(this->btnEmpReset);
			this->pnlEmpSearchHeader->Controls->Add(this->btnEmpSearch);
			this->pnlEmpSearchHeader->Controls->Add(this->cmbFilterEmpRole);
			this->pnlEmpSearchHeader->Controls->Add(this->lblFilterEmpRole);
			this->pnlEmpSearchHeader->Controls->Add(this->txtEmpSearch);
			this->pnlEmpSearchHeader->Controls->Add(this->lblEmpSearch);
			this->pnlEmpSearchHeader->Dock = System::Windows::Forms::DockStyle::Top;
			this->pnlEmpSearchHeader->Location = System::Drawing::Point(12, 12);
			this->pnlEmpSearchHeader->Name = L"pnlEmpSearchHeader";
			this->pnlEmpSearchHeader->Size = System::Drawing::Size(509, 45);
			this->pnlEmpSearchHeader->TabIndex = 0;

			this->lblEmpSearch->AutoSize = true;
			this->lblEmpSearch->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->lblEmpSearch->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblEmpSearch->Location = System::Drawing::Point(2, 14);
			this->lblEmpSearch->Name = L"lblEmpSearch";
			this->lblEmpSearch->Size = System::Drawing::Size(48, 15);
			this->lblEmpSearch->TabIndex = 0;
			this->lblEmpSearch->Text = L"Search:";

			this->txtEmpSearch->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5f));
			this->txtEmpSearch->Location = System::Drawing::Point(52, 10);
			this->txtEmpSearch->Name = L"txtEmpSearch";
			this->txtEmpSearch->Size = System::Drawing::Size(130, 24);
			this->txtEmpSearch->TabIndex = 1;
			this->txtEmpSearch->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MainForm::txtEmpSearch_KeyDown);

			this->lblFilterEmpRole->AutoSize = true;
			this->lblFilterEmpRole->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->lblFilterEmpRole->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblFilterEmpRole->Location = System::Drawing::Point(190, 14);
			this->lblFilterEmpRole->Name = L"lblFilterEmpRole";
			this->lblFilterEmpRole->Size = System::Drawing::Size(35, 15);
			this->lblFilterEmpRole->TabIndex = 2;
			this->lblFilterEmpRole->Text = L"Role:";

			this->cmbFilterEmpRole->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbFilterEmpRole->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.0f));
			this->cmbFilterEmpRole->FormattingEnabled = true;
			this->cmbFilterEmpRole->Items->AddRange(gcnew cli::array< System::Object^  >(4) {
				L"All Roles",
				L"Admin",
				L"Checker",
				L"Repairer"
			});
			this->cmbFilterEmpRole->Location = System::Drawing::Point(230, 11);
			this->cmbFilterEmpRole->Name = L"cmbFilterEmpRole";
			this->cmbFilterEmpRole->Size = System::Drawing::Size(100, 23);
			this->cmbFilterEmpRole->TabIndex = 3;

			this->btnEmpSearch->BackColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->btnEmpSearch->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnEmpSearch->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnEmpSearch->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.5f, System::Drawing::FontStyle::Bold));
			this->btnEmpSearch->ForeColor = System::Drawing::Color::White;
			this->btnEmpSearch->Location = System::Drawing::Point(340, 9);
			this->btnEmpSearch->Name = L"btnEmpSearch";
			this->btnEmpSearch->Size = System::Drawing::Size(60, 27);
			this->btnEmpSearch->TabIndex = 4;
			this->btnEmpSearch->Text = L"Filter";
			this->btnEmpSearch->UseVisualStyleBackColor = false;
			this->btnEmpSearch->Click += gcnew System::EventHandler(this, &MainForm::btnEmpSearch_Click);

			this->btnEmpReset->BackColor = System::Drawing::Color::FromArgb(69, 123, 157);
			this->btnEmpReset->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnEmpReset->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnEmpReset->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.5f));
			this->btnEmpReset->ForeColor = System::Drawing::Color::White;
			this->btnEmpReset->Location = System::Drawing::Point(405, 9);
			this->btnEmpReset->Name = L"btnEmpReset";
			this->btnEmpReset->Size = System::Drawing::Size(55, 27);
			this->btnEmpReset->TabIndex = 5;
			this->btnEmpReset->Text = L"Reset";
			this->btnEmpReset->UseVisualStyleBackColor = false;
			this->btnEmpReset->Click += gcnew System::EventHandler(this, &MainForm::btnEmpReset_Click);

			this->lblEmpCount->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.5f, System::Drawing::FontStyle::Bold));
			this->lblEmpCount->ForeColor = System::Drawing::Color::FromArgb(108, 117, 125);
			this->lblEmpCount->Location = System::Drawing::Point(465, 12);
			this->lblEmpCount->Name = L"lblEmpCount";
			this->lblEmpCount->Size = System::Drawing::Size(40, 20);
			this->lblEmpCount->TabIndex = 6;
			this->lblEmpCount->Text = L"";

			this->dgvEmployees->AllowUserToAddRows = false;
			this->dgvEmployees->AllowUserToDeleteRows = false;
			this->dgvEmployees->AllowUserToResizeRows = false;
			this->dgvEmployees->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::Fill;
			this->dgvEmployees->BackgroundColor = System::Drawing::Color::White;
			this->dgvEmployees->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->dgvEmployees->ColumnHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::Single;

			DataGridViewCellStyle^ empHeaderStyle = gcnew DataGridViewCellStyle();
			empHeaderStyle->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			empHeaderStyle->BackColor = System::Drawing::Color::FromArgb(29, 53, 87);
			empHeaderStyle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.25f, System::Drawing::FontStyle::Bold));
			empHeaderStyle->ForeColor = System::Drawing::Color::White;
			empHeaderStyle->SelectionBackColor = System::Drawing::Color::FromArgb(29, 53, 87);
			empHeaderStyle->SelectionForeColor = System::Drawing::Color::White;
			empHeaderStyle->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
			this->dgvEmployees->ColumnHeadersDefaultCellStyle = empHeaderStyle;
			this->dgvEmployees->ColumnHeadersHeight = 35;
			this->dgvEmployees->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::DisableResizing;

			DataGridViewCellStyle^ empRowStyle = gcnew DataGridViewCellStyle();
			empRowStyle->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			empRowStyle->BackColor = System::Drawing::Color::White;
			empRowStyle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			empRowStyle->ForeColor = System::Drawing::Color::FromArgb(33, 37, 41);
			empRowStyle->SelectionBackColor = System::Drawing::Color::FromArgb(168, 218, 220);
			empRowStyle->SelectionForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->dgvEmployees->DefaultCellStyle = empRowStyle;

			this->dgvEmployees->Dock = System::Windows::Forms::DockStyle::Fill;
			this->dgvEmployees->EnableHeadersVisualStyles = false;
			this->dgvEmployees->GridColor = System::Drawing::Color::FromArgb(222, 226, 230);
			this->dgvEmployees->Location = System::Drawing::Point(12, 57);
			this->dgvEmployees->MultiSelect = false;
			this->dgvEmployees->Name = L"dgvEmployees";
			this->dgvEmployees->ReadOnly = true;
			this->dgvEmployees->RowHeadersVisible = false;
			this->dgvEmployees->RowTemplate->Height = 32;
			this->dgvEmployees->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dgvEmployees->Size = System::Drawing::Size(509, 524);
			this->dgvEmployees->TabIndex = 1;
			this->dgvEmployees->CellClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MainForm::dgvEmployees_CellClick);

			// ----------------------------------------------------
			// 8. REPORTS & MONTHLY INCOME VIEW PANEL (Phase 10)
			// ----------------------------------------------------
			this->pnlViewReports->Controls->Add(this->pnlReportTableCard);
			this->pnlViewReports->Controls->Add(this->flowReportStats);
			this->pnlViewReports->Controls->Add(this->pnlReportFilterCard);
			this->pnlViewReports->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pnlViewReports->Location = System::Drawing::Point(25, 25);
			this->pnlViewReports->Name = L"pnlViewReports";
			this->pnlViewReports->Size = System::Drawing::Size(910, 595);
			this->pnlViewReports->TabIndex = 4;
			this->pnlViewReports->Visible = false;

			// Filter card
			this->pnlReportFilterCard->BackColor = System::Drawing::Color::White;
			this->pnlReportFilterCard->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pnlReportFilterCard->Controls->Add(this->btnExportCsv);
			this->pnlReportFilterCard->Controls->Add(this->btnCurrentMonth);
			this->pnlReportFilterCard->Controls->Add(this->btnGenerateReport);
			this->pnlReportFilterCard->Controls->Add(this->cmbReportYear);
			this->pnlReportFilterCard->Controls->Add(this->lblReportYear);
			this->pnlReportFilterCard->Controls->Add(this->cmbReportMonth);
			this->pnlReportFilterCard->Controls->Add(this->lblReportMonth);
			this->pnlReportFilterCard->Controls->Add(this->lblReportFilterTitle);
			this->pnlReportFilterCard->Dock = System::Windows::Forms::DockStyle::Top;
			this->pnlReportFilterCard->Location = System::Drawing::Point(0, 0);
			this->pnlReportFilterCard->Name = L"pnlReportFilterCard";
			this->pnlReportFilterCard->Size = System::Drawing::Size(910, 60);
			this->pnlReportFilterCard->TabIndex = 0;

			this->lblReportFilterTitle->AutoSize = true;
			this->lblReportFilterTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
			this->lblReportFilterTitle->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblReportFilterTitle->Location = System::Drawing::Point(15, 18);
			this->lblReportFilterTitle->Name = L"lblReportFilterTitle";
			this->lblReportFilterTitle->Size = System::Drawing::Size(95, 19);
			this->lblReportFilterTitle->TabIndex = 0;
			this->lblReportFilterTitle->Text = L"Select Period:";

			this->lblReportMonth->AutoSize = true;
			this->lblReportMonth->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			this->lblReportMonth->ForeColor = System::Drawing::Color::FromArgb(50, 50, 50);
			this->lblReportMonth->Location = System::Drawing::Point(125, 20);
			this->lblReportMonth->Name = L"lblReportMonth";
			this->lblReportMonth->Size = System::Drawing::Size(46, 15);
			this->lblReportMonth->TabIndex = 1;
			this->lblReportMonth->Text = L"Month:";

			this->cmbReportMonth->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbReportMonth->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			this->cmbReportMonth->FormattingEnabled = true;
			this->cmbReportMonth->Location = System::Drawing::Point(175, 17);
			this->cmbReportMonth->Name = L"cmbReportMonth";
			this->cmbReportMonth->Size = System::Drawing::Size(125, 23);
			this->cmbReportMonth->TabIndex = 2;

			this->lblReportYear->AutoSize = true;
			this->lblReportYear->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			this->lblReportYear->ForeColor = System::Drawing::Color::FromArgb(50, 50, 50);
			this->lblReportYear->Location = System::Drawing::Point(315, 20);
			this->lblReportYear->Name = L"lblReportYear";
			this->lblReportYear->Size = System::Drawing::Size(32, 15);
			this->lblReportYear->TabIndex = 3;
			this->lblReportYear->Text = L"Year:";

			this->cmbReportYear->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbReportYear->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			this->cmbReportYear->FormattingEnabled = true;
			this->cmbReportYear->Location = System::Drawing::Point(352, 17);
			this->cmbReportYear->Name = L"cmbReportYear";
			this->cmbReportYear->Size = System::Drawing::Size(85, 23);
			this->cmbReportYear->TabIndex = 4;

			this->btnGenerateReport->BackColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->btnGenerateReport->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnGenerateReport->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnGenerateReport->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->btnGenerateReport->ForeColor = System::Drawing::Color::White;
			this->btnGenerateReport->Location = System::Drawing::Point(455, 14);
			this->btnGenerateReport->Name = L"btnGenerateReport";
			this->btnGenerateReport->Size = System::Drawing::Size(130, 30);
			this->btnGenerateReport->TabIndex = 5;
			this->btnGenerateReport->Text = L"Generate Report";
			this->btnGenerateReport->UseVisualStyleBackColor = false;
			this->btnGenerateReport->Click += gcnew System::EventHandler(this, &MainForm::btnGenerateReport_Click);

			this->btnCurrentMonth->BackColor = System::Drawing::Color::FromArgb(69, 123, 157);
			this->btnCurrentMonth->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnCurrentMonth->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnCurrentMonth->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.5f));
			this->btnCurrentMonth->ForeColor = System::Drawing::Color::White;
			this->btnCurrentMonth->Location = System::Drawing::Point(595, 14);
			this->btnCurrentMonth->Name = L"btnCurrentMonth";
			this->btnCurrentMonth->Size = System::Drawing::Size(95, 30);
			this->btnCurrentMonth->TabIndex = 6;
			this->btnCurrentMonth->Text = L"This Month";
			this->btnCurrentMonth->UseVisualStyleBackColor = false;
			this->btnCurrentMonth->Click += gcnew System::EventHandler(this, &MainForm::btnCurrentMonth_Click);

			this->btnExportCsv->BackColor = System::Drawing::Color::FromArgb(39, 174, 96);
			this->btnExportCsv->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnExportCsv->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnExportCsv->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->btnExportCsv->ForeColor = System::Drawing::Color::White;
			this->btnExportCsv->Location = System::Drawing::Point(700, 14);
			this->btnExportCsv->Name = L"btnExportCsv";
			this->btnExportCsv->Size = System::Drawing::Size(130, 30);
			this->btnExportCsv->TabIndex = 7;
			this->btnExportCsv->Text = L"Export to CSV";
			this->btnExportCsv->UseVisualStyleBackColor = false;
			this->btnExportCsv->Click += gcnew System::EventHandler(this, &MainForm::btnExportCsv_Click);

			// Stats flow panel
			this->flowReportStats->Dock = System::Windows::Forms::DockStyle::Top;
			this->flowReportStats->Location = System::Drawing::Point(0, 60);
			this->flowReportStats->Name = L"flowReportStats";
			this->flowReportStats->Padding = System::Windows::Forms::Padding(0, 15, 0, 0);
			this->flowReportStats->Size = System::Drawing::Size(910, 120);
			this->flowReportStats->TabIndex = 1;

			this->cardRepIncome = CreateStatCard(L"Completed Jobs Income", this->lblRepIncomeVal, System::Drawing::Color::FromArgb(29, 53, 87), 215);
			this->cardRepFinished = CreateStatCard(L"Finished Jobs", this->lblRepFinishedVal, System::Drawing::Color::FromArgb(40, 167, 69), 170);
			this->cardRepAvg = CreateStatCard(L"Average Income / Job", this->lblRepAvgVal, System::Drawing::Color::FromArgb(69, 123, 157), 200);
			this->cardRepPeriod = CreateStatCard(L"Selected Period", this->lblRepPeriodVal, System::Drawing::Color::FromArgb(243, 156, 18), 215);
			this->lblRepPeriodVal->Font = (gcnew System::Drawing::Font(L"Segoe UI", 13, System::Drawing::FontStyle::Bold));

			this->flowReportStats->Controls->Add(this->cardRepIncome);
			this->flowReportStats->Controls->Add(this->cardRepFinished);
			this->flowReportStats->Controls->Add(this->cardRepAvg);
			this->flowReportStats->Controls->Add(this->cardRepPeriod);

			// Table card
			this->pnlReportTableCard->BackColor = System::Drawing::Color::White;
			this->pnlReportTableCard->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pnlReportTableCard->Controls->Add(this->dgvReportJobs);
			this->pnlReportTableCard->Controls->Add(this->pnlReportTableHeader);
			this->pnlReportTableCard->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pnlReportTableCard->Location = System::Drawing::Point(0, 180);
			this->pnlReportTableCard->Name = L"pnlReportTableCard";
			this->pnlReportTableCard->Padding = System::Windows::Forms::Padding(12);
			this->pnlReportTableCard->Size = System::Drawing::Size(910, 415);
			this->pnlReportTableCard->TabIndex = 2;

			this->pnlReportTableHeader->Controls->Add(this->lblReportCount);
			this->pnlReportTableHeader->Controls->Add(this->lblReportTableTitle);
			this->pnlReportTableHeader->Dock = System::Windows::Forms::DockStyle::Top;
			this->pnlReportTableHeader->Location = System::Drawing::Point(12, 12);
			this->pnlReportTableHeader->Name = L"pnlReportTableHeader";
			this->pnlReportTableHeader->Size = System::Drawing::Size(884, 40);
			this->pnlReportTableHeader->TabIndex = 0;

			this->lblReportTableTitle->AutoSize = true;
			this->lblReportTableTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11, System::Drawing::FontStyle::Bold));
			this->lblReportTableTitle->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblReportTableTitle->Location = System::Drawing::Point(0, 8);
			this->lblReportTableTitle->Name = L"lblReportTableTitle";
			this->lblReportTableTitle->Size = System::Drawing::Size(270, 20);
			this->lblReportTableTitle->TabIndex = 0;
			this->lblReportTableTitle->Text = L"Finished Repair Jobs Itemized Breakdown";

			this->lblReportCount->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold));
			this->lblReportCount->ForeColor = System::Drawing::Color::FromArgb(108, 117, 125);
			this->lblReportCount->Location = System::Drawing::Point(550, 8);
			this->lblReportCount->Name = L"lblReportCount";
			this->lblReportCount->Size = System::Drawing::Size(330, 20);
			this->lblReportCount->TabIndex = 1;
			this->lblReportCount->Text = L"";
			this->lblReportCount->TextAlign = System::Drawing::ContentAlignment::MiddleRight;

			this->dgvReportJobs->AllowUserToAddRows = false;
			this->dgvReportJobs->AllowUserToDeleteRows = false;
			this->dgvReportJobs->AllowUserToResizeRows = false;
			this->dgvReportJobs->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::Fill;
			this->dgvReportJobs->BackgroundColor = System::Drawing::Color::White;
			this->dgvReportJobs->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->dgvReportJobs->ColumnHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::Single;

			DataGridViewCellStyle^ repHeaderStyle = gcnew DataGridViewCellStyle();
			repHeaderStyle->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			repHeaderStyle->BackColor = System::Drawing::Color::FromArgb(29, 53, 87);
			repHeaderStyle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.25f, System::Drawing::FontStyle::Bold));
			repHeaderStyle->ForeColor = System::Drawing::Color::White;
			repHeaderStyle->SelectionBackColor = System::Drawing::Color::FromArgb(29, 53, 87);
			repHeaderStyle->SelectionForeColor = System::Drawing::Color::White;
			repHeaderStyle->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
			this->dgvReportJobs->ColumnHeadersDefaultCellStyle = repHeaderStyle;
			this->dgvReportJobs->ColumnHeadersHeight = 35;
			this->dgvReportJobs->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::DisableResizing;

			DataGridViewCellStyle^ repRowStyle = gcnew DataGridViewCellStyle();
			repRowStyle->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			repRowStyle->BackColor = System::Drawing::Color::White;
			repRowStyle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			repRowStyle->ForeColor = System::Drawing::Color::FromArgb(33, 37, 41);
			repRowStyle->SelectionBackColor = System::Drawing::Color::FromArgb(168, 218, 220);
			repRowStyle->SelectionForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->dgvReportJobs->DefaultCellStyle = repRowStyle;

			this->dgvReportJobs->Dock = System::Windows::Forms::DockStyle::Fill;
			this->dgvReportJobs->EnableHeadersVisualStyles = false;
			this->dgvReportJobs->GridColor = System::Drawing::Color::FromArgb(222, 226, 230);
			this->dgvReportJobs->Location = System::Drawing::Point(12, 52);
			this->dgvReportJobs->MultiSelect = false;
			this->dgvReportJobs->Name = L"dgvReportJobs";
			this->dgvReportJobs->ReadOnly = true;
			this->dgvReportJobs->RowHeadersVisible = false;
			this->dgvReportJobs->RowTemplate->Height = 32;
			this->dgvReportJobs->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dgvReportJobs->Size = System::Drawing::Size(884, 349);
			this->dgvReportJobs->TabIndex = 1;

			// ----------------------------------------------------
			// 9. PLACEHOLDER VIEW PANEL
			// ----------------------------------------------------
			this->pnlPlaceholderView->BackColor = System::Drawing::Color::White;
			this->pnlPlaceholderView->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pnlPlaceholderView->Controls->Add(this->lblPlaceholderDesc);
			this->pnlPlaceholderView->Controls->Add(this->lblPlaceholderTitle);
			this->pnlPlaceholderView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pnlPlaceholderView->Location = System::Drawing::Point(25, 25);
			this->pnlPlaceholderView->Name = L"pnlPlaceholderView";
			this->pnlPlaceholderView->Padding = System::Windows::Forms::Padding(30);
			this->pnlPlaceholderView->Size = System::Drawing::Size(910, 595);
			this->pnlPlaceholderView->TabIndex = 4;
			this->pnlPlaceholderView->Visible = false;

			this->lblPlaceholderTitle->AutoSize = true;
			this->lblPlaceholderTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 16, System::Drawing::FontStyle::Bold));
			this->lblPlaceholderTitle->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblPlaceholderTitle->Location = System::Drawing::Point(30, 30);
			this->lblPlaceholderTitle->Name = L"lblPlaceholderTitle";
			this->lblPlaceholderTitle->Size = System::Drawing::Size(155, 30);
			this->lblPlaceholderTitle->TabIndex = 0;
			this->lblPlaceholderTitle->Text = L"Module View";

			this->lblPlaceholderDesc->AutoSize = false;
			this->lblPlaceholderDesc->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11));
			this->lblPlaceholderDesc->ForeColor = System::Drawing::Color::FromArgb(108, 117, 125);
			this->lblPlaceholderDesc->Location = System::Drawing::Point(32, 75);
			this->lblPlaceholderDesc->Name = L"lblPlaceholderDesc";
			this->lblPlaceholderDesc->Size = System::Drawing::Size(840, 100);
			this->lblPlaceholderDesc->TabIndex = 1;
			this->lblPlaceholderDesc->Text = L"This module is scheduled for implementation in the next phase.";

			// MainForm Form Settings
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(244, 246, 249);
			this->ClientSize = System::Drawing::Size(1200, 720);
			this->Controls->Add(this->pnlMainContent);
			this->Controls->Add(this->pnlHeader);
			this->Controls->Add(this->pnlSidebar);
			this->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			this->MinimumSize = System::Drawing::Size(1100, 680);
			this->Name = L"MainForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"FixSphere - Mobile Phone Repair Center Management System";

			this->pnlSidebar->ResumeLayout(false);
			this->pnlBrand->ResumeLayout(false);
			this->pnlBrand->PerformLayout();
			this->pnlNavButtons->ResumeLayout(false);
			this->pnlSidebarFooter->ResumeLayout(false);
			this->pnlHeader->ResumeLayout(false);
			this->pnlHeader->PerformLayout();
			this->pnlUserBadge->ResumeLayout(false);
			this->pnlMainContent->ResumeLayout(false);

			this->pnlViewDashboard->ResumeLayout(false);
			this->pnlRecentJobsHeader->ResumeLayout(false);
			this->pnlRecentJobsHeader->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dgvRecentJobs))->EndInit();

			this->pnlViewCustomers->ResumeLayout(false);
			this->pnlCustFormCard->ResumeLayout(false);
			this->pnlCustFormCard->PerformLayout();
			this->pnlCustTableCard->ResumeLayout(false);
			this->pnlCustSearchHeader->ResumeLayout(false);
			this->pnlCustSearchHeader->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dgvCustomers))->EndInit();

			this->pnlViewJobs->ResumeLayout(false);
			this->pnlJobFormCard->ResumeLayout(false);
			this->pnlJobTableCard->ResumeLayout(false);
			this->pnlJobSearchHeader->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dgvJobs))->EndInit();

			this->pnlViewEmployees->ResumeLayout(false);
			this->pnlEmpFormCard->ResumeLayout(false);
			this->pnlEmpFormCard->PerformLayout();
			this->pnlEmpTableCard->ResumeLayout(false);
			this->pnlEmpSearchHeader->ResumeLayout(false);
			this->pnlEmpSearchHeader->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dgvEmployees))->EndInit();

			this->pnlViewReports->ResumeLayout(false);
			this->pnlReportFilterCard->ResumeLayout(false);
			this->pnlReportFilterCard->PerformLayout();
			this->flowReportStats->ResumeLayout(false);
			this->pnlReportTableCard->ResumeLayout(false);
			this->pnlReportTableHeader->ResumeLayout(false);
			this->pnlReportTableHeader->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dgvReportJobs))->EndInit();

			this->pnlPlaceholderView->ResumeLayout(false);
			this->pnlPlaceholderView->PerformLayout();
			this->ResumeLayout(false);
		}

	private:
		// ----------------------------------------------------
		// Role-Based UI Security & Permissions
		// ----------------------------------------------------
		void ApplyRoleSecurity()
		{
			if (UserSession::IsLoggedIn())
			{
				UserSession^ user = UserSession::CurrentUser;
				this->lblLoggedUser->Text = String::Format(L"{0} ({1})", user->FullName, user->Username);
				this->lblLoggedRole->Text = String::Format(L"ROLE: {0}", user->Role->ToUpper());

				if (!user->CanManageCustomers())
				{
					this->btnNavCustomers->Visible = false;
				}

				if (!user->CanCreateJobs())
				{
					this->btnJobSave->Visible = false;
				}

				if (!user->CanDeleteJobs())
				{
					this->btnJobDelete->Visible = false;
				}

				if (!user->CanManageEmployees())
				{
					this->btnNavEmployees->Visible = false;
				}

				if (!user->CanViewFinancialReports())
				{
					this->btnNavReports->Visible = false;
					this->cardIncome->Visible = false;
				}

				if (!user->CanAssignRepairer())
				{
					this->cmbJobRepairer->Visible = false;
					this->txtJobRepairerDisplay->Visible = true;
				}
				else
				{
					this->cmbJobRepairer->Visible = true;
					this->txtJobRepairerDisplay->Visible = false;
				}

				if (user->IsChecker())
				{
					this->btnJobUpdate->Visible = false;
					this->btnJobDelete->Visible = false;
					this->cmbJobStatus->Enabled = false;
					this->txtJobCost->ReadOnly = true;
				}
				else if (user->IsRepairer())
				{
					this->btnJobSave->Visible = false;
					this->btnJobDelete->Visible = false;
				}
			}
		}

		// ----------------------------------------------------
		// Dashboard Data Loading (Phase 6)
		// ----------------------------------------------------
		void LoadDashboardData()
		{
			try
			{
				MySqlConnection^ conn = DatabaseService::CreateConnection();
				try
				{
					MySqlCommand^ cmdCust = gcnew MySqlCommand(L"SELECT COUNT(*) FROM customers;", conn);
					Object^ custObj = cmdCust->ExecuteScalar();
					int totalCust = (custObj != nullptr && custObj != DBNull::Value) ? Convert::ToInt32(custObj) : 0;
					this->lblStatCustVal->Text = totalCust.ToString();

					String^ statsQuery = 
						L"SELECT "
						L"COUNT(*) AS total_jobs, "
						L"SUM(CASE WHEN status = 'Pending' THEN 1 ELSE 0 END) AS pending_jobs, "
						L"SUM(CASE WHEN status = 'In Progress' THEN 1 ELSE 0 END) AS in_progress_jobs, "
						L"SUM(CASE WHEN status = 'Finished' THEN 1 ELSE 0 END) AS finished_jobs, "
						L"SUM(CASE WHEN status = 'Couldn''t Repair' THEN 1 ELSE 0 END) AS failed_jobs, "
						L"IFNULL(SUM(CASE WHEN status = 'Finished' AND MONTH(COALESCE(finished_at, created_at)) = MONTH(CURRENT_DATE()) AND YEAR(COALESCE(finished_at, created_at)) = YEAR(CURRENT_DATE()) THEN cost ELSE 0 END), 0) AS monthly_income "
						L"FROM repair_jobs;";

					MySqlCommand^ cmdStats = gcnew MySqlCommand(statsQuery, conn);
					MySqlDataReader^ reader = cmdStats->ExecuteReader();

					if (reader->Read())
					{
						int totalJobs = reader->IsDBNull(0) ? 0 : reader->GetInt32(0);
						int pendingJobs = reader->IsDBNull(1) ? 0 : reader->GetInt32(1);
						int inProgressJobs = reader->IsDBNull(2) ? 0 : reader->GetInt32(2);
						int finishedJobs = reader->IsDBNull(3) ? 0 : reader->GetInt32(3);
						int failedJobs = reader->IsDBNull(4) ? 0 : reader->GetInt32(4);
						Decimal monthlyIncome = reader->IsDBNull(5) ? Decimal(0) : reader->GetDecimal(5);

						this->lblStatJobsVal->Text = totalJobs.ToString();
						this->lblStatPendingVal->Text = pendingJobs.ToString();
						this->lblStatRepairingVal->Text = inProgressJobs.ToString();
						this->lblStatFinishedVal->Text = finishedJobs.ToString();
						this->lblStatFailedVal->Text = failedJobs.ToString();

						this->lblStatIncomeVal->Text = String::Format(L"LKR {0:N2}", monthlyIncome);
					}
					reader->Close();

					String^ recentJobsQuery =
						L"SELECT "
						L"r.job_id AS 'Job ID', "
						L"r.customer_name AS 'Customer', "
						L"r.customer_contact AS 'Contact', "
						L"r.phone_model AS 'Phone Model', "
						L"r.details AS 'Problem Details', "
						L"r.status AS 'Status', "
						L"CONCAT('LKR ', FORMAT(r.cost, 2)) AS 'Cost', "
						L"DATE_FORMAT(r.created_at, '%Y-%m-%d %H:%i') AS 'Received Date' "
						L"FROM repair_jobs r "
						L"ORDER BY r.job_id DESC "
						L"LIMIT 10;";

					MySqlDataAdapter^ adapter = gcnew MySqlDataAdapter(recentJobsQuery, conn);
					DataTable^ dt = gcnew DataTable();
					adapter->Fill(dt);

					this->dgvRecentJobs->DataSource = dt;

					if (this->dgvRecentJobs->Columns[L"Job ID"] != nullptr)
						this->dgvRecentJobs->Columns[L"Job ID"]->FillWeight = 50;
					if (this->dgvRecentJobs->Columns[L"Customer"] != nullptr)
						this->dgvRecentJobs->Columns[L"Customer"]->FillWeight = 110;
					if (this->dgvRecentJobs->Columns[L"Phone Model"] != nullptr)
						this->dgvRecentJobs->Columns[L"Phone Model"]->FillWeight = 100;
					if (this->dgvRecentJobs->Columns[L"Problem Details"] != nullptr)
						this->dgvRecentJobs->Columns[L"Problem Details"]->FillWeight = 140;
					if (this->dgvRecentJobs->Columns[L"Status"] != nullptr)
						this->dgvRecentJobs->Columns[L"Status"]->FillWeight = 85;
					if (this->dgvRecentJobs->Columns[L"Cost"] != nullptr)
						this->dgvRecentJobs->Columns[L"Cost"]->FillWeight = 70;
					if (this->dgvRecentJobs->Columns[L"Received Date"] != nullptr)
						this->dgvRecentJobs->Columns[L"Received Date"]->FillWeight = 100;
				}
				finally
				{
					conn->Close();
				}
			}
			catch (Exception^ ex)
			{
				MessageBox::Show(
					String::Format(L"Error loading dashboard data:\n{0}", ex->Message),
					L"Database Error",
					MessageBoxButtons::OK,
					MessageBoxIcon::Error
				);
			}
		}

		// ----------------------------------------------------
		// Customer Management Logic (Phase 7)
		// ----------------------------------------------------
		void LoadCustomersData(String^ filter)
		{
			try
			{
				MySqlConnection^ conn = DatabaseService::CreateConnection();
				try
				{
					String^ sql = L"";
					MySqlCommand^ cmd = gcnew MySqlCommand();
					cmd->Connection = conn;

					if (String::IsNullOrEmpty(filter))
					{
						sql = L"SELECT customer_id AS 'ID', name AS 'Name', contact AS 'Phone Number', nic AS 'NIC Number', address AS 'Address', DATE_FORMAT(created_at, '%Y-%m-%d %H:%i') AS 'Registered Date' "
							L"FROM customers ORDER BY customer_id DESC;";
					}
					else
					{
						sql = L"SELECT customer_id AS 'ID', name AS 'Name', contact AS 'Phone Number', nic AS 'NIC Number', address AS 'Address', DATE_FORMAT(created_at, '%Y-%m-%d %H:%i') AS 'Registered Date' "
							L"FROM customers "
							L"WHERE name LIKE @search OR contact LIKE @search OR nic LIKE @search OR CAST(customer_id AS CHAR) = @exactId "
							L"ORDER BY customer_id DESC;";

						cmd->Parameters->AddWithValue(L"@search", String::Format(L"%{0}%", filter));
						cmd->Parameters->AddWithValue(L"@exactId", filter);
					}

					cmd->CommandText = sql;
					MySqlDataAdapter^ adapter = gcnew MySqlDataAdapter(cmd);
					DataTable^ dt = gcnew DataTable();
					adapter->Fill(dt);

					this->dgvCustomers->DataSource = dt;

					if (this->dgvCustomers->Columns[L"ID"] != nullptr)
						this->dgvCustomers->Columns[L"ID"]->FillWeight = 40;
					if (this->dgvCustomers->Columns[L"Name"] != nullptr)
						this->dgvCustomers->Columns[L"Name"]->FillWeight = 120;
					if (this->dgvCustomers->Columns[L"Phone Number"] != nullptr)
						this->dgvCustomers->Columns[L"Phone Number"]->FillWeight = 90;
					if (this->dgvCustomers->Columns[L"NIC Number"] != nullptr)
						this->dgvCustomers->Columns[L"NIC Number"]->FillWeight = 100;
					if (this->dgvCustomers->Columns[L"Address"] != nullptr)
						this->dgvCustomers->Columns[L"Address"]->FillWeight = 120;
					if (this->dgvCustomers->Columns[L"Registered Date"] != nullptr)
						this->dgvCustomers->Columns[L"Registered Date"]->FillWeight = 90;

					this->lblCustCount->Text = String::Format(L"{0} Customer(s)", dt->Rows->Count);
				}
				finally
				{
					conn->Close();
				}
			}
			catch (Exception^ ex)
			{
				MessageBox::Show(
					String::Format(L"Error loading customers:\n{0}", ex->Message),
					L"Database Error",
					MessageBoxButtons::OK,
					MessageBoxIcon::Error
				);
			}
		}

		void ResetCustomerForm()
		{
			this->selectedCustomerId = 0;
			this->txtCustName->Clear();
			this->txtCustNic->Clear();
			this->txtCustContact->Clear();
			this->txtCustAddress->Clear();

			this->lblCustIdStatus->Text = L"Mode: [New Registration]";
			this->lblCustIdStatus->ForeColor = System::Drawing::Color::FromArgb(69, 123, 157);
			this->btnCustSave->Enabled = true;
			this->btnCustUpdate->Enabled = false;
			this->txtCustName->Focus();
		}

		System::Void btnCustSave_Click(System::Object^ sender, System::EventArgs^ e)
		{
			String^ name = this->txtCustName->Text->Trim();
			String^ nic = this->txtCustNic->Text->Trim();
			String^ contact = this->txtCustContact->Text->Trim();
			String^ address = this->txtCustAddress->Text->Trim();

			if (String::IsNullOrEmpty(name))
			{
				MessageBox::Show(L"Customer full name is required.", L"Validation Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				this->txtCustName->Focus();
				return;
			}

			if (String::IsNullOrEmpty(nic) || nic->Length < 9)
			{
				MessageBox::Show(L"Please enter a valid NIC number (minimum 9 characters).", L"Validation Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				this->txtCustNic->Focus();
				return;
			}

			if (String::IsNullOrEmpty(contact) || contact->Length < 9)
			{
				MessageBox::Show(L"Please enter a valid contact phone number.", L"Validation Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				this->txtCustContact->Focus();
				return;
			}

			if (String::IsNullOrEmpty(address))
			{
				MessageBox::Show(L"Customer address is required.", L"Validation Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				this->txtCustAddress->Focus();
				return;
			}

			try
			{
				MySqlConnection^ conn = DatabaseService::CreateConnection();
				try
				{
					MySqlCommand^ chkCmd = gcnew MySqlCommand(L"SELECT COUNT(*) FROM customers WHERE nic = @nic;", conn);
					chkCmd->Parameters->AddWithValue(L"@nic", nic);
					int exists = Convert::ToInt32(chkCmd->ExecuteScalar());

					if (exists > 0)
					{
						MessageBox::Show(
							String::Format(L"A customer with NIC '{0}' is already registered in the system.", nic),
							L"Duplicate Record",
							MessageBoxButtons::OK,
							MessageBoxIcon::Warning
						);
						this->txtCustNic->Focus();
						return;
					}

					String^ insertSql = L"INSERT INTO customers (name, contact, nic, address) VALUES (@name, @contact, @nic, @address);";
					MySqlCommand^ insCmd = gcnew MySqlCommand(insertSql, conn);
					insCmd->Parameters->AddWithValue(L"@name", name);
					insCmd->Parameters->AddWithValue(L"@contact", contact);
					insCmd->Parameters->AddWithValue(L"@nic", nic);
					insCmd->Parameters->AddWithValue(L"@address", address);
					insCmd->ExecuteNonQuery();

					MessageBox::Show(
						String::Format(L"Customer '{0}' was registered successfully!", name),
						L"Customer Registered",
						MessageBoxButtons::OK,
						MessageBoxIcon::Information
					);

					ResetCustomerForm();
					LoadCustomersData(nullptr);
					LoadDashboardData();
				}
				finally
				{
					conn->Close();
				}
			}
			catch (Exception^ ex)
			{
				MessageBox::Show(
					String::Format(L"Failed to register customer:\n{0}", ex->Message),
					L"Database Error",
					MessageBoxButtons::OK,
					MessageBoxIcon::Error
				);
			}
		}

		System::Void btnCustUpdate_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (this->selectedCustomerId <= 0)
			{
				MessageBox::Show(L"Please select a customer from the table to edit.", L"Selection Required", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}

			String^ name = this->txtCustName->Text->Trim();
			String^ nic = this->txtCustNic->Text->Trim();
			String^ contact = this->txtCustContact->Text->Trim();
			String^ address = this->txtCustAddress->Text->Trim();

			if (String::IsNullOrEmpty(name) || String::IsNullOrEmpty(nic) || String::IsNullOrEmpty(contact) || String::IsNullOrEmpty(address))
			{
				MessageBox::Show(L"All customer fields are required.", L"Validation Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}

			try
			{
				MySqlConnection^ conn = DatabaseService::CreateConnection();
				try
				{
					MySqlCommand^ chkCmd = gcnew MySqlCommand(L"SELECT COUNT(*) FROM customers WHERE nic = @nic AND customer_id != @id;", conn);
					chkCmd->Parameters->AddWithValue(L"@nic", nic);
					chkCmd->Parameters->AddWithValue(L"@id", this->selectedCustomerId);
					int exists = Convert::ToInt32(chkCmd->ExecuteScalar());

					if (exists > 0)
					{
						MessageBox::Show(
							String::Format(L"Another customer is already using NIC '{0}'.", nic),
							L"Duplicate NIC",
							MessageBoxButtons::OK,
							MessageBoxIcon::Warning
						);
						return;
					}

					String^ updateSql = L"UPDATE customers SET name = @name, contact = @contact, nic = @nic, address = @address WHERE customer_id = @id;";
					MySqlCommand^ updCmd = gcnew MySqlCommand(updateSql, conn);
					updCmd->Parameters->AddWithValue(L"@name", name);
					updCmd->Parameters->AddWithValue(L"@contact", contact);
					updCmd->Parameters->AddWithValue(L"@nic", nic);
					updCmd->Parameters->AddWithValue(L"@address", address);
					updCmd->Parameters->AddWithValue(L"@id", this->selectedCustomerId);
					updCmd->ExecuteNonQuery();

					String^ syncJobsSql = L"UPDATE repair_jobs SET customer_name = @name, customer_contact = @contact, customer_nic = @nic, customer_address = @address WHERE customer_id = @id;";
					MySqlCommand^ syncCmd = gcnew MySqlCommand(syncJobsSql, conn);
					syncCmd->Parameters->AddWithValue(L"@name", name);
					syncCmd->Parameters->AddWithValue(L"@contact", contact);
					syncCmd->Parameters->AddWithValue(L"@nic", nic);
					syncCmd->Parameters->AddWithValue(L"@address", address);
					syncCmd->Parameters->AddWithValue(L"@id", this->selectedCustomerId);
					syncCmd->ExecuteNonQuery();

					MessageBox::Show(
						L"Customer profile updated successfully!",
						L"Update Complete",
						MessageBoxButtons::OK,
						MessageBoxIcon::Information
					);

					ResetCustomerForm();
					LoadCustomersData(nullptr);
				}
				finally
				{
					conn->Close();
				}
			}
			catch (Exception^ ex)
			{
				MessageBox::Show(
					String::Format(L"Failed to update customer:\n{0}", ex->Message),
					L"Database Error",
					MessageBoxButtons::OK,
					MessageBoxIcon::Error
				);
			}
		}

		System::Void btnCustClear_Click(System::Object^ sender, System::EventArgs^ e)
		{
			ResetCustomerForm();
		}

		System::Void dgvCustomers_CellClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellEventArgs^ e)
		{
			if (e->RowIndex >= 0 && e->RowIndex < this->dgvCustomers->Rows->Count)
			{
				DataGridViewRow^ row = this->dgvCustomers->Rows[e->RowIndex];
				if (row->Cells[L"ID"]->Value != nullptr)
				{
					this->selectedCustomerId = Convert::ToInt32(row->Cells[L"ID"]->Value);
					this->txtCustName->Text = row->Cells[L"Name"]->Value != nullptr ? row->Cells[L"Name"]->Value->ToString() : L"";
					this->txtCustContact->Text = row->Cells[L"Phone Number"]->Value != nullptr ? row->Cells[L"Phone Number"]->Value->ToString() : L"";
					this->txtCustNic->Text = row->Cells[L"NIC Number"]->Value != nullptr ? row->Cells[L"NIC Number"]->Value->ToString() : L"";
					this->txtCustAddress->Text = row->Cells[L"Address"]->Value != nullptr ? row->Cells[L"Address"]->Value->ToString() : L"";

					this->lblCustIdStatus->Text = String::Format(L"Mode: [Editing Customer ID #{0}]", this->selectedCustomerId);
					this->lblCustIdStatus->ForeColor = System::Drawing::Color::FromArgb(21, 87, 36);

					this->btnCustSave->Enabled = false;
					this->btnCustUpdate->Enabled = true;
				}
			}
		}

		System::Void btnCustSearch_Click(System::Object^ sender, System::EventArgs^ e)
		{
			String^ query = this->txtCustSearch->Text->Trim();
			LoadCustomersData(query);
		}

		System::Void btnCustReset_Click(System::Object^ sender, System::EventArgs^ e)
		{
			this->txtCustSearch->Clear();
			LoadCustomersData(nullptr);
		}

		System::Void txtCustSearch_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e)
		{
			if (e->KeyCode == Keys::Enter)
			{
				e->SuppressKeyPress = true;
				btnCustSearch_Click(sender, e);
			}
		}

		// ----------------------------------------------------
		// 3. Repair Job Management Logic (Phase 8)
		// ----------------------------------------------------
		void PopulateCustomerDropdown()
		{
			this->cmbJobCustomer->Items->Clear();
			this->cmbJobCustomer->Items->Add(gcnew ComboItem(0, L"-- Select Customer --"));

			try
			{
				MySqlConnection^ conn = DatabaseService::CreateConnection();
				try
				{
					MySqlCommand^ cmd = gcnew MySqlCommand(L"SELECT customer_id, name, contact FROM customers ORDER BY name;", conn);
					MySqlDataReader^ r = cmd->ExecuteReader();
					while (r->Read())
					{
						int id = r->GetInt32(0);
						String^ name = r->GetString(1);
						String^ phone = r->GetString(2);
						this->cmbJobCustomer->Items->Add(gcnew ComboItem(id, String::Format(L"#{0}: {1} ({2})", id, name, phone)));
					}
					r->Close();
				}
				finally
				{
					conn->Close();
				}
			}
			catch (Exception^)
			{
				// Keep fallback
			}
			this->cmbJobCustomer->SelectedIndex = 0;
		}

		void PopulateRepairerDropdown()
		{
			this->cmbJobRepairer->Items->Clear();
			this->cmbJobRepairer->Items->Add(gcnew ComboItem(0, L"-- Unassigned --"));

			try
			{
				MySqlConnection^ conn = DatabaseService::CreateConnection();
				try
				{
					MySqlCommand^ cmd = gcnew MySqlCommand(L"SELECT emp_id, name, role FROM employees WHERE role = 'Repairer' AND is_active = 1 ORDER BY name;", conn);
					MySqlDataReader^ r = cmd->ExecuteReader();
					while (r->Read())
					{
						int id = r->GetInt32(0);
						String^ name = r->GetString(1);
						String^ role = r->GetString(2);
						this->cmbJobRepairer->Items->Add(gcnew ComboItem(id, String::Format(L"{0} ({1})", name, role)));
					}
					r->Close();
				}
				finally
				{
					conn->Close();
				}
			}
			catch (Exception^)
			{
				// Keep fallback
			}
			this->cmbJobRepairer->SelectedIndex = 0;
		}

		void LoadJobsData(String^ filter, String^ statusFilter)
		{
			try
			{
				MySqlConnection^ conn = DatabaseService::CreateConnection();
				try
				{
					String^ sql = 
						L"SELECT "
						L"r.job_id AS 'Job ID', "
						L"r.customer_name AS 'Customer', "
						L"r.customer_contact AS 'Contact', "
						L"r.phone_model AS 'Phone Model', "
						L"r.details AS 'Problem Details', "
						L"IFNULL(e.name, 'Unassigned') AS 'Repairer', "
						L"r.status AS 'Status', "
						L"CONCAT('LKR ', FORMAT(r.cost, 2)) AS 'Cost', "
						L"DATE_FORMAT(r.created_at, '%Y-%m-%d %H:%i') AS 'Date Received', "
						L"IFNULL(DATE_FORMAT(r.finished_at, '%Y-%m-%d %H:%i'), '-') AS 'Date Finished', "
						L"IFNULL(r.repairer_id, 0) AS 'repairer_id' "
						L"FROM repair_jobs r "
						L"LEFT JOIN employees e ON r.repairer_id = e.emp_id "
						L"WHERE 1=1 ";

					MySqlCommand^ cmd = gcnew MySqlCommand();
					cmd->Connection = conn;

					UserSession^ currentUser = UserSession::CurrentUser;
					if (currentUser != nullptr && currentUser->IsRepairer())
					{
						sql += L"AND r.repairer_id = @currentRepairerId ";
						cmd->Parameters->AddWithValue(L"@currentRepairerId", currentUser->EmployeeId);
					}

					if (!String::IsNullOrEmpty(statusFilter) && !statusFilter->Equals(L"All Statuses"))
					{
						sql += L"AND r.status = @statusFilter ";
						cmd->Parameters->AddWithValue(L"@statusFilter", statusFilter);
					}

					if (!String::IsNullOrEmpty(filter))
					{
						sql += L"AND (r.customer_name LIKE @search OR r.phone_model LIKE @search OR r.details LIKE @search OR CAST(r.job_id AS CHAR) = @exactId) ";
						cmd->Parameters->AddWithValue(L"@search", String::Format(L"%{0}%", filter));
						cmd->Parameters->AddWithValue(L"@exactId", filter);
					}

					sql += L"ORDER BY r.job_id DESC;";
					cmd->CommandText = sql;

					MySqlDataAdapter^ adapter = gcnew MySqlDataAdapter(cmd);
					DataTable^ dt = gcnew DataTable();
					adapter->Fill(dt);

					this->dgvJobs->DataSource = dt;

					if (this->dgvJobs->Columns[L"Job ID"] != nullptr)
						this->dgvJobs->Columns[L"Job ID"]->FillWeight = 45;
					if (this->dgvJobs->Columns[L"Customer"] != nullptr)
						this->dgvJobs->Columns[L"Customer"]->FillWeight = 95;
					if (this->dgvJobs->Columns[L"Contact"] != nullptr)
						this->dgvJobs->Columns[L"Contact"]->FillWeight = 80;
					if (this->dgvJobs->Columns[L"Phone Model"] != nullptr)
						this->dgvJobs->Columns[L"Phone Model"]->FillWeight = 85;
					if (this->dgvJobs->Columns[L"Problem Details"] != nullptr)
						this->dgvJobs->Columns[L"Problem Details"]->FillWeight = 110;
					if (this->dgvJobs->Columns[L"Repairer"] != nullptr)
						this->dgvJobs->Columns[L"Repairer"]->FillWeight = 85;
					if (this->dgvJobs->Columns[L"Status"] != nullptr)
						this->dgvJobs->Columns[L"Status"]->FillWeight = 75;
					if (this->dgvJobs->Columns[L"Cost"] != nullptr)
						this->dgvJobs->Columns[L"Cost"]->FillWeight = 65;
					if (this->dgvJobs->Columns[L"Date Received"] != nullptr)
						this->dgvJobs->Columns[L"Date Received"]->FillWeight = 85;
					if (this->dgvJobs->Columns[L"Date Finished"] != nullptr)
						this->dgvJobs->Columns[L"Date Finished"]->FillWeight = 85;
					if (this->dgvJobs->Columns[L"repairer_id"] != nullptr)
						this->dgvJobs->Columns[L"repairer_id"]->Visible = false;

					this->lblJobCount->Text = String::Format(L"{0} Job(s)", dt->Rows->Count);
				}
				finally
				{
					conn->Close();
				}
			}
			catch (Exception^ ex)
			{
				MessageBox::Show(
					String::Format(L"Error loading repair jobs:\n{0}", ex->Message),
					L"Database Error",
					MessageBoxButtons::OK,
					MessageBoxIcon::Error
				);
			}
		}

		void ResetJobForm()
		{
			this->selectedJobId = 0;
			this->selectedJobRepairerId = 0;
			this->txtJobPhoneModel->Clear();
			this->txtJobDetails->Clear();
			this->txtJobCost->Text = L"0.00";

			if (this->cmbJobCustomer->Items->Count > 0)
				this->cmbJobCustomer->SelectedIndex = 0;

			if (this->cmbJobRepairer->Items->Count > 0)
				this->cmbJobRepairer->SelectedIndex = 0;

			this->txtJobRepairerDisplay->Text = L"Unassigned";
			this->cmbJobStatus->SelectedIndex = 0;

			this->lblJobIdStatus->Text = L"Mode: [New Job Intake]";
			this->lblJobIdStatus->ForeColor = System::Drawing::Color::FromArgb(69, 123, 157);

			UserSession^ user = UserSession::CurrentUser;
			bool canCreate = (user != nullptr && user->CanCreateJobs());
			this->btnJobSave->Enabled = canCreate;
			this->btnJobSave->Visible = canCreate;
			this->btnJobUpdate->Enabled = false;
			this->btnJobDelete->Enabled = false;
			this->cmbJobCustomer->Enabled = canCreate;

			if (user != nullptr && user->IsAdmin())
			{
				this->cmbJobRepairer->Visible = true;
				this->txtJobRepairerDisplay->Visible = false;
				this->cmbJobStatus->Enabled = true;
				this->txtJobPhoneModel->ReadOnly = false;
				this->txtJobDetails->ReadOnly = false;
				this->txtJobCost->ReadOnly = false;
			}
			else if (user != nullptr && user->IsChecker())
			{
				this->cmbJobRepairer->Visible = false;
				this->txtJobRepairerDisplay->Visible = true;
				this->txtJobRepairerDisplay->Text = L"Unassigned (Admin assigns)";
				this->cmbJobStatus->Enabled = false;
				this->txtJobPhoneModel->ReadOnly = false;
				this->txtJobDetails->ReadOnly = false;
				this->txtJobCost->ReadOnly = true;
			}
			else // Repairer
			{
				this->cmbJobRepairer->Visible = false;
				this->txtJobRepairerDisplay->Visible = true;
				this->txtJobRepairerDisplay->Text = L"Unassigned";
				this->cmbJobStatus->Enabled = false;
				this->txtJobPhoneModel->ReadOnly = true;
				this->txtJobDetails->ReadOnly = true;
				this->txtJobCost->ReadOnly = true;
			}
		}

		System::Void cmbJobStatus_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
		{
			String^ status = this->cmbJobStatus->SelectedItem != nullptr ? this->cmbJobStatus->SelectedItem->ToString() : L"";
			if (status->Equals(L"Couldn't Repair", StringComparison::OrdinalIgnoreCase))
			{
				this->txtJobCost->Text = L"0.00";
				this->txtJobCost->ReadOnly = true;
			}
			else
			{
				UserSession^ user = UserSession::CurrentUser;
				if (user != nullptr && (user->IsAdmin() || (user->IsRepairer() && this->selectedJobRepairerId == user->EmployeeId)))
				{
					this->txtJobCost->ReadOnly = false;
				}
				else
				{
					this->txtJobCost->ReadOnly = true;
				}
			}
		}

		System::Void btnJobSave_Click(System::Object^ sender, System::EventArgs^ e)
		{
			UserSession^ user = UserSession::CurrentUser;
			if (user == nullptr || !user->CanCreateJobs())
			{
				MessageBox::Show(L"Access Denied: Only Admins and Checkers can register new repair jobs.", L"Permission Denied", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}

			if (this->cmbJobCustomer->SelectedIndex <= 0)
			{
				MessageBox::Show(L"Please select a registered customer for this repair job.", L"Customer Required", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				this->cmbJobCustomer->Focus();
				return;
			}

			ComboItem^ custItem = safe_cast<ComboItem^>(this->cmbJobCustomer->SelectedItem);
			int customerId = custItem->Id;

			String^ phoneModel = this->txtJobPhoneModel->Text->Trim();
			String^ details = this->txtJobDetails->Text->Trim();
			String^ status = this->cmbJobStatus->SelectedItem != nullptr ? this->cmbJobStatus->SelectedItem->ToString() : L"Pending";

			if (String::IsNullOrEmpty(phoneModel))
			{
				MessageBox::Show(L"Please specify the phone brand and model.", L"Validation Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				this->txtJobPhoneModel->Focus();
				return;
			}

			if (String::IsNullOrEmpty(details))
			{
				MessageBox::Show(L"Please provide the reported problem and inspection details.", L"Validation Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				this->txtJobDetails->Focus();
				return;
			}

			Decimal cost = Decimal(0);
			try
			{
				cost = Decimal::Parse(this->txtJobCost->Text->Trim());
			}
			catch (Exception^)
			{
				MessageBox::Show(L"Please enter a valid numeric cost.", L"Validation Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				this->txtJobCost->Focus();
				return;
			}

			if (status->Equals(L"Couldn't Repair", StringComparison::OrdinalIgnoreCase) && cost > Decimal(0))
			{
				MessageBox::Show(L"Business Rule Violation: A job marked 'Couldn't Repair' must have a cost of LKR 0.00.", L"Cost Rule", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				this->txtJobCost->Text = L"0.00";
				return;
			}

			// Validate Assignment: Only Admin can assign/reassign repair jobs to Repairer employees
			int repairerId = 0;
			if (user->IsAdmin())
			{
				if (this->cmbJobRepairer->SelectedIndex > 0)
				{
					ComboItem^ repItem = safe_cast<ComboItem^>(this->cmbJobRepairer->SelectedItem);
					repairerId = repItem->Id;
				}
			}
			else
			{
				// Checker is strictly prohibited from assigning jobs
				repairerId = 0;
			}

			try
			{
				MySqlConnection^ conn = DatabaseService::CreateConnection();
				try
				{
					String^ custQuery = L"SELECT name, contact, nic, address FROM customers WHERE customer_id = @cid;";
					MySqlCommand^ cCmd = gcnew MySqlCommand(custQuery, conn);
					cCmd->Parameters->AddWithValue(L"@cid", customerId);
					MySqlDataReader^ cReader = cCmd->ExecuteReader();

					String^ custName = L"";
					String^ custContact = L"";
					String^ custNic = L"";
					String^ custAddress = L"";

					if (cReader->Read())
					{
						custName = cReader->GetString(0);
						custContact = cReader->GetString(1);
						custNic = cReader->GetString(2);
						custAddress = cReader->GetString(3);
					}
					cReader->Close();

					String^ insertJobSql = 
						L"INSERT INTO repair_jobs (customer_id, customer_name, customer_contact, customer_nic, customer_address, phone_model, details, status, cost, created_by, repairer_id, created_at, finished_at) "
						L"VALUES (@cid, @cname, @ccontact, @cnic, @caddress, @model, @details, @status, @cost, @creator, @repid, NOW(), "
						L"CASE WHEN @status = 'Finished' THEN NOW() ELSE NULL END);";

					MySqlCommand^ insCmd = gcnew MySqlCommand(insertJobSql, conn);
					insCmd->Parameters->AddWithValue(L"@cid", customerId);
					insCmd->Parameters->AddWithValue(L"@cname", custName);
					insCmd->Parameters->AddWithValue(L"@ccontact", custContact);
					insCmd->Parameters->AddWithValue(L"@cnic", custNic);
					insCmd->Parameters->AddWithValue(L"@caddress", custAddress);
					insCmd->Parameters->AddWithValue(L"@model", phoneModel);
					insCmd->Parameters->AddWithValue(L"@details", details);
					insCmd->Parameters->AddWithValue(L"@status", status);
					insCmd->Parameters->AddWithValue(L"@cost", cost);
					insCmd->Parameters->AddWithValue(L"@creator", user->EmployeeId);

					if (repairerId > 0)
						insCmd->Parameters->AddWithValue(L"@repid", repairerId);
					else
						insCmd->Parameters->AddWithValue(L"@repid", DBNull::Value);

					insCmd->ExecuteNonQuery();

					MessageBox::Show(
						String::Format(L"Repair job for '{0} ({1})' was registered successfully!", custName, phoneModel),
						L"Job Intake Complete",
						MessageBoxButtons::OK,
						MessageBoxIcon::Information
					);

					ResetJobForm();
					LoadJobsData(nullptr, nullptr);
					LoadDashboardData();
				}
				finally
				{
					conn->Close();
				}
			}
			catch (Exception^ ex)
			{
				MessageBox::Show(
					String::Format(L"Failed to create repair job:\n{0}", ex->Message),
					L"Database Error",
					MessageBoxButtons::OK,
					MessageBoxIcon::Error
				);
			}
		}

		System::Void btnJobUpdate_Click(System::Object^ sender, System::EventArgs^ e)
		{
			UserSession^ user = UserSession::CurrentUser;
			if (user == nullptr)
			{
				MessageBox::Show(L"Access Denied: Please sign in.", L"Permission Denied", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}

			// Checker is strictly prohibited from updating statuses
			if (user->IsChecker())
			{
				MessageBox::Show(L"Access Denied: Checkers are not permitted to update repair job statuses.", L"Permission Denied", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}

			if (this->selectedJobId <= 0)
			{
				MessageBox::Show(L"Please select a repair job from the table.", L"Selection Required", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}

			// Validate Repairer job ownership before update
			if (user->IsRepairer())
			{
				try
				{
					MySqlConnection^ chkConn = DatabaseService::CreateConnection();
					try
					{
						MySqlCommand^ chkCmd = gcnew MySqlCommand(L"SELECT IFNULL(repairer_id, 0) FROM repair_jobs WHERE job_id = @jid;", chkConn);
						chkCmd->Parameters->AddWithValue(L"@jid", this->selectedJobId);
						Object^ repObj = chkCmd->ExecuteScalar();
						int dbRepId = (repObj != nullptr && repObj != DBNull::Value) ? Convert::ToInt32(repObj) : 0;
						if (dbRepId != user->EmployeeId)
						{
							MessageBox::Show(L"Access Denied: You can only update repair jobs assigned to your own account.", L"Ownership Verification", MessageBoxButtons::OK, MessageBoxIcon::Warning);
							return;
						}
					}
					finally
					{
						chkConn->Close();
					}
				}
				catch (Exception^ ex)
				{
					MessageBox::Show(String::Format(L"Error verifying job ownership:\n{0}", ex->Message), L"Database Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
					return;
				}
			}

			String^ phoneModel = this->txtJobPhoneModel->Text->Trim();
			String^ details = this->txtJobDetails->Text->Trim();
			String^ status = this->cmbJobStatus->SelectedItem != nullptr ? this->cmbJobStatus->SelectedItem->ToString() : L"Pending";

			if (String::IsNullOrEmpty(phoneModel) || String::IsNullOrEmpty(details))
			{
				MessageBox::Show(L"Phone model and repair details are required.", L"Validation Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}

			Decimal cost = Decimal(0);
			try
			{
				cost = Decimal::Parse(this->txtJobCost->Text->Trim());
			}
			catch (Exception^)
			{
				MessageBox::Show(L"Please enter a valid numeric cost.", L"Validation Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}

			if (status->Equals(L"Couldn't Repair", StringComparison::OrdinalIgnoreCase) && cost > Decimal(0))
			{
				MessageBox::Show(L"Business Rule Violation: A job marked 'Couldn't Repair' must have a cost of LKR 0.00.", L"Cost Rule", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				this->txtJobCost->Text = L"0.00";
				return;
			}

			if (cost < Decimal(0))
			{
				MessageBox::Show(L"Repair cost cannot be negative.", L"Validation Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}

			int repairerId = 0;
			if (user->IsAdmin())
			{
				if (this->cmbJobRepairer->SelectedIndex > 0)
				{
					ComboItem^ repItem = safe_cast<ComboItem^>(this->cmbJobRepairer->SelectedItem);
					repairerId = repItem->Id;
				}
			}

			try
			{
				MySqlConnection^ conn = DatabaseService::CreateConnection();
				try
				{
					String^ updateJobSql;
					MySqlCommand^ updCmd = gcnew MySqlCommand();
					updCmd->Connection = conn;

					if (user->IsAdmin())
					{
						// Admin can update model, details, status, cost, and reassign repairer
						updateJobSql = 
							L"UPDATE repair_jobs SET "
							L"phone_model = @model, "
							L"details = @details, "
							L"status = @status, "
							L"cost = @cost, "
							L"repairer_id = @repid, "
							L"finished_at = CASE "
							L"    WHEN @status = 'Finished' THEN IFNULL(finished_at, NOW()) "
							L"    ELSE NULL "
							L"END "
							L"WHERE job_id = @jid;";

						if (repairerId > 0)
							updCmd->Parameters->AddWithValue(L"@repid", repairerId);
						else
							updCmd->Parameters->AddWithValue(L"@repid", DBNull::Value);
					}
					else
					{
						// Repairer can update details/notes, status, and cost, but CANNOT reassign the job!
						updateJobSql = 
							L"UPDATE repair_jobs SET "
							L"details = @details, "
							L"status = @status, "
							L"cost = @cost, "
							L"finished_at = CASE "
							L"    WHEN @status = 'Finished' THEN IFNULL(finished_at, NOW()) "
							L"    ELSE NULL "
							L"END "
							L"WHERE job_id = @jid AND repairer_id = @repIdCheck;";

						updCmd->Parameters->AddWithValue(L"@repIdCheck", user->EmployeeId);
					}

					updCmd->CommandText = updateJobSql;
					updCmd->Parameters->AddWithValue(L"@model", phoneModel);
					updCmd->Parameters->AddWithValue(L"@details", details);
					updCmd->Parameters->AddWithValue(L"@status", status);
					updCmd->Parameters->AddWithValue(L"@cost", cost);
					updCmd->Parameters->AddWithValue(L"@jid", this->selectedJobId);

					int rowsAffected = updCmd->ExecuteNonQuery();
					if (rowsAffected > 0)
					{
						MessageBox::Show(
							String::Format(L"Repair Job #{0} updated successfully!\nStatus: {1}\nCost: LKR {2:N2}", this->selectedJobId, status, cost),
							L"Status Update Saved",
							MessageBoxButtons::OK,
							MessageBoxIcon::Information
						);
					}
					else
					{
						MessageBox::Show(
							L"Update was not applied. You might not have permission to update this job.",
							L"Update Failed",
							MessageBoxButtons::OK,
							MessageBoxIcon::Warning
						);
					}

					ResetJobForm();
					LoadJobsData(nullptr, nullptr);
					LoadDashboardData();
				}
				finally
				{
					conn->Close();
				}
			}
			catch (Exception^ ex)
			{
				MessageBox::Show(
					String::Format(L"Failed to update repair job:\n{0}", ex->Message),
					L"Database Error",
					MessageBoxButtons::OK,
					MessageBoxIcon::Error
				);
			}
		}

		System::Void btnJobDelete_Click(System::Object^ sender, System::EventArgs^ e)
		{
			UserSession^ user = UserSession::CurrentUser;
			if (user == nullptr || !user->CanDeleteJobs())
			{
				MessageBox::Show(L"Access Denied: Only Administrators are permitted to delete repair jobs.", L"Permission Denied", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}

			if (this->selectedJobId <= 0)
			{
				MessageBox::Show(L"Please select a repair job from the table to delete.", L"Selection Required", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}

			System::Windows::Forms::DialogResult dr = MessageBox::Show(
				String::Format(L"Are you sure you want to permanently delete Repair Job #{0}?\n\nThis destructive action cannot be undone.", this->selectedJobId),
				L"Confirm Job Deletion",
				MessageBoxButtons::YesNo,
				MessageBoxIcon::Warning
			);

			if (dr == System::Windows::Forms::DialogResult::Yes)
			{
				try
				{
					MySqlConnection^ conn = DatabaseService::CreateConnection();
					try
					{
						MySqlCommand^ delCmd = gcnew MySqlCommand(L"DELETE FROM repair_jobs WHERE job_id = @jid;", conn);
						delCmd->Parameters->AddWithValue(L"@jid", this->selectedJobId);
						delCmd->ExecuteNonQuery();

						MessageBox::Show(
							String::Format(L"Repair Job #{0} was permanently deleted.", this->selectedJobId),
							L"Job Deleted",
							MessageBoxButtons::OK,
							MessageBoxIcon::Information
						);

						ResetJobForm();
						LoadJobsData(nullptr, nullptr);
						LoadDashboardData();
					}
					finally
					{
						conn->Close();
					}
				}
				catch (Exception^ ex)
				{
					MessageBox::Show(
						String::Format(L"Failed to delete repair job:\n{0}", ex->Message),
						L"Database Error",
						MessageBoxButtons::OK,
						MessageBoxIcon::Error
					);
				}
			}
		}

		System::Void btnJobClear_Click(System::Object^ sender, System::EventArgs^ e)
		{
			ResetJobForm();
		}

		System::Void dgvJobs_CellClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellEventArgs^ e)
		{
			if (e->RowIndex >= 0 && e->RowIndex < this->dgvJobs->Rows->Count)
			{
				DataGridViewRow^ row = this->dgvJobs->Rows[e->RowIndex];
				if (row->Cells[L"Job ID"]->Value != nullptr)
				{
					this->selectedJobId = Convert::ToInt32(row->Cells[L"Job ID"]->Value);
					this->txtJobPhoneModel->Text = row->Cells[L"Phone Model"]->Value != nullptr ? row->Cells[L"Phone Model"]->Value->ToString() : L"";
					this->txtJobDetails->Text = row->Cells[L"Problem Details"]->Value != nullptr ? row->Cells[L"Problem Details"]->Value->ToString() : L"";

					String^ costStr = row->Cells[L"Cost"]->Value != nullptr ? row->Cells[L"Cost"]->Value->ToString() : L"0.00";
					costStr = costStr->Replace(L"LKR", L"")->Replace(L"$", L"")->Replace(L",", L"")->Trim();
					this->txtJobCost->Text = costStr;

					this->selectedJobRepairerId = (row->Cells[L"repairer_id"] != nullptr && row->Cells[L"repairer_id"]->Value != nullptr) ? Convert::ToInt32(row->Cells[L"repairer_id"]->Value) : 0;

					String^ statusVal = row->Cells[L"Status"]->Value != nullptr ? row->Cells[L"Status"]->Value->ToString() : L"Pending";
					for (int i = 0; i < this->cmbJobStatus->Items->Count; i++)
					{
						if (this->cmbJobStatus->Items[i]->ToString()->Equals(statusVal, StringComparison::OrdinalIgnoreCase))
						{
							this->cmbJobStatus->SelectedIndex = i;
							break;
						}
					}

					String^ repVal = row->Cells[L"Repairer"]->Value != nullptr ? row->Cells[L"Repairer"]->Value->ToString() : L"Unassigned";
					for (int i = 0; i < this->cmbJobRepairer->Items->Count; i++)
					{
						if (this->cmbJobRepairer->Items[i]->ToString()->StartsWith(repVal))
						{
							this->cmbJobRepairer->SelectedIndex = i;
							break;
						}
					}

					String^ custName = row->Cells[L"Customer"]->Value != nullptr ? row->Cells[L"Customer"]->Value->ToString() : L"";
					for (int i = 0; i < this->cmbJobCustomer->Items->Count; i++)
					{
						if (this->cmbJobCustomer->Items[i]->ToString()->Contains(custName))
						{
							this->cmbJobCustomer->SelectedIndex = i;
							break;
						}
					}

					this->lblJobIdStatus->Text = String::Format(L"Mode: [Editing Job #{0}]", this->selectedJobId);
					this->lblJobIdStatus->ForeColor = System::Drawing::Color::FromArgb(21, 87, 36);

					UserSession^ user = UserSession::CurrentUser;
					this->btnJobSave->Enabled = false;
					this->cmbJobCustomer->Enabled = false;

					if (user != nullptr && user->IsAdmin())
					{
						this->cmbJobRepairer->Visible = true;
						this->txtJobRepairerDisplay->Visible = false;
						this->cmbJobStatus->Enabled = true;
						this->txtJobPhoneModel->ReadOnly = false;
						this->txtJobDetails->ReadOnly = false;
						this->txtJobCost->ReadOnly = statusVal->Equals(L"Couldn't Repair", StringComparison::OrdinalIgnoreCase);
						this->btnJobUpdate->Enabled = true;
						this->btnJobDelete->Enabled = true;
					}
					else if (user != nullptr && user->IsRepairer())
					{
						this->cmbJobRepairer->Visible = false;
						this->txtJobRepairerDisplay->Visible = true;
						this->txtJobRepairerDisplay->Text = repVal;
						this->btnJobDelete->Enabled = false;

						// Repairer can only update status of their own assigned jobs!
						bool isMyJob = (this->selectedJobRepairerId == user->EmployeeId);
						this->cmbJobStatus->Enabled = isMyJob;
						this->txtJobPhoneModel->ReadOnly = true;
						this->txtJobDetails->ReadOnly = !isMyJob;
						this->txtJobCost->ReadOnly = !isMyJob || statusVal->Equals(L"Couldn't Repair", StringComparison::OrdinalIgnoreCase);
						this->btnJobUpdate->Enabled = isMyJob;
					}
					else // Checker
					{
						this->cmbJobRepairer->Visible = false;
						this->txtJobRepairerDisplay->Visible = true;
						this->txtJobRepairerDisplay->Text = repVal;
						this->cmbJobStatus->Enabled = false;
						this->txtJobPhoneModel->ReadOnly = true;
						this->txtJobDetails->ReadOnly = true;
						this->txtJobCost->ReadOnly = true;
						this->btnJobUpdate->Enabled = false;
						this->btnJobDelete->Enabled = false;
					}
				}
			}
		}

		System::Void btnJobSearch_Click(System::Object^ sender, System::EventArgs^ e)
		{
			String^ query = this->txtJobSearch->Text->Trim();
			String^ status = this->cmbFilterStatus->SelectedItem != nullptr ? this->cmbFilterStatus->SelectedItem->ToString() : L"";
			LoadJobsData(query, status);
		}

		System::Void btnJobReset_Click(System::Object^ sender, System::EventArgs^ e)
		{
			this->txtJobSearch->Clear();
			this->cmbFilterStatus->SelectedIndex = 0;
			LoadJobsData(nullptr, nullptr);
		}

		System::Void txtJobSearch_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e)
		{
			if (e->KeyCode == Keys::Enter)
			{
				e->SuppressKeyPress = true;
				btnJobSearch_Click(sender, e);
			}
		}

		// ----------------------------------------------------
		// 4. Employee Management Logic (Phase 9)
		// ----------------------------------------------------
		void LoadEmployeesData(String^ filter, String^ roleFilter)
		{
			try
			{
				MySqlConnection^ conn = DatabaseService::CreateConnection();
				try
				{
					String^ sql = 
						L"SELECT "
						L"emp_id AS 'ID', "
						L"name AS 'Full Name', "
						L"username AS 'Username', "
						L"role AS 'Role', "
						L"CASE WHEN is_active = 1 THEN 'Active' ELSE 'Deactivated' END AS 'Account Status', "
						L"contact AS 'Phone Number', "
						L"nic AS 'NIC Number', "
						L"address AS 'Address', "
						L"DATE_FORMAT(created_at, '%Y-%m-%d %H:%i') AS 'Registered Date' "
						L"FROM employees "
						L"WHERE 1=1 ";

					MySqlCommand^ cmd = gcnew MySqlCommand();
					cmd->Connection = conn;

					if (!String::IsNullOrEmpty(roleFilter) && !roleFilter->Equals(L"All Roles"))
					{
						sql += L"AND role = @roleFilter ";
						cmd->Parameters->AddWithValue(L"@roleFilter", roleFilter);
					}

					if (!String::IsNullOrEmpty(filter))
					{
						sql += L"AND (name LIKE @search OR username LIKE @search OR nic LIKE @search OR contact LIKE @search OR CAST(emp_id AS CHAR) = @exactId) ";
						cmd->Parameters->AddWithValue(L"@search", String::Format(L"%{0}%", filter));
						cmd->Parameters->AddWithValue(L"@exactId", filter);
					}

					sql += L"ORDER BY emp_id ASC;";
					cmd->CommandText = sql;

					MySqlDataAdapter^ adapter = gcnew MySqlDataAdapter(cmd);
					DataTable^ dt = gcnew DataTable();
					adapter->Fill(dt);

					this->dgvEmployees->DataSource = dt;

					if (this->dgvEmployees->Columns[L"ID"] != nullptr)
						this->dgvEmployees->Columns[L"ID"]->FillWeight = 40;
					if (this->dgvEmployees->Columns[L"Full Name"] != nullptr)
						this->dgvEmployees->Columns[L"Full Name"]->FillWeight = 110;
					if (this->dgvEmployees->Columns[L"Username"] != nullptr)
						this->dgvEmployees->Columns[L"Username"]->FillWeight = 85;
					if (this->dgvEmployees->Columns[L"Role"] != nullptr)
						this->dgvEmployees->Columns[L"Role"]->FillWeight = 75;
					if (this->dgvEmployees->Columns[L"Account Status"] != nullptr)
						this->dgvEmployees->Columns[L"Account Status"]->FillWeight = 85;
					if (this->dgvEmployees->Columns[L"Phone Number"] != nullptr)
						this->dgvEmployees->Columns[L"Phone Number"]->FillWeight = 85;
					if (this->dgvEmployees->Columns[L"NIC Number"] != nullptr)
						this->dgvEmployees->Columns[L"NIC Number"]->FillWeight = 95;
					if (this->dgvEmployees->Columns[L"Address"] != nullptr)
						this->dgvEmployees->Columns[L"Address"]->FillWeight = 100;
					if (this->dgvEmployees->Columns[L"Registered Date"] != nullptr)
						this->dgvEmployees->Columns[L"Registered Date"]->FillWeight = 85;

					this->lblEmpCount->Text = String::Format(L"{0} Staff", dt->Rows->Count);
				}
				finally
				{
					conn->Close();
				}
			}
			catch (Exception^ ex)
			{
				MessageBox::Show(
					String::Format(L"Error loading employees:\n{0}", ex->Message),
					L"Database Error",
					MessageBoxButtons::OK,
					MessageBoxIcon::Error
				);
			}
		}

		void ResetEmployeeForm()
		{
			this->selectedEmpId = 0;
			this->txtEmpName->Clear();
			this->txtEmpNic->Clear();
			this->txtEmpContact->Clear();
			this->txtEmpAddress->Clear();
			this->txtEmpUsername->Clear();
			this->txtEmpUsername->ReadOnly = false;
			this->txtEmpPassword->Clear();
			this->chkEmpActive->Checked = true;
			this->cmbEmpRole->SelectedIndex = 0; // "Admin"

			this->lblEmpIdStatus->Text = L"Mode: [New Registration]";
			this->lblEmpIdStatus->ForeColor = System::Drawing::Color::FromArgb(69, 123, 157);

			this->btnEmpSave->Enabled = true;
			this->btnEmpUpdate->Enabled = false;
			this->btnEmpResetPass->Enabled = false;
			this->btnEmpToggleActive->Enabled = false;
			this->btnEmpToggleActive->Text = L"Deactivate";
		}

		System::Void chkEmpShowPass_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
		{
			this->txtEmpPassword->PasswordChar = this->chkEmpShowPass->Checked ? '\0' : '*';
		}

		System::Void btnEmpSave_Click(System::Object^ sender, System::EventArgs^ e)
		{
			UserSession^ user = UserSession::CurrentUser;
			if (user == nullptr || !user->CanManageEmployees())
			{
				MessageBox::Show(L"Access Denied: Only Administrators can create employee accounts.", L"Permission Denied", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}

			String^ name = this->txtEmpName->Text->Trim();
			String^ nic = this->txtEmpNic->Text->Trim();
			String^ contact = this->txtEmpContact->Text->Trim();
			String^ address = this->txtEmpAddress->Text->Trim();
			String^ role = this->cmbEmpRole->SelectedItem != nullptr ? this->cmbEmpRole->SelectedItem->ToString() : L"Checker";
			String^ username = this->txtEmpUsername->Text->Trim();
			String^ password = this->txtEmpPassword->Text;
			int isActive = this->chkEmpActive->Checked ? 1 : 0;

			if (String::IsNullOrEmpty(name) || String::IsNullOrEmpty(nic) || String::IsNullOrEmpty(contact) || String::IsNullOrEmpty(address) || String::IsNullOrEmpty(username))
			{
				MessageBox::Show(L"All employee details are required.", L"Validation Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}

			if (String::IsNullOrEmpty(password) || password->Length < 6)
			{
				MessageBox::Show(L"Please provide a secure password with at least 6 characters.", L"Validation Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				this->txtEmpPassword->Focus();
				return;
			}

			try
			{
				MySqlConnection^ conn = DatabaseService::CreateConnection();
				try
				{
					// Check unique username
					MySqlCommand^ uCmd = gcnew MySqlCommand(L"SELECT COUNT(*) FROM employees WHERE username = @user;", conn);
					uCmd->Parameters->AddWithValue(L"@user", username);
					if (Convert::ToInt32(uCmd->ExecuteScalar()) > 0)
					{
						MessageBox::Show(String::Format(L"Username '{0}' is already taken. Please choose another username.", username), L"Duplicate Username", MessageBoxButtons::OK, MessageBoxIcon::Warning);
						this->txtEmpUsername->Focus();
						return;
					}

					// Check unique NIC
					MySqlCommand^ nCmd = gcnew MySqlCommand(L"SELECT COUNT(*) FROM employees WHERE nic = @nic;", conn);
					nCmd->Parameters->AddWithValue(L"@nic", nic);
					if (Convert::ToInt32(nCmd->ExecuteScalar()) > 0)
					{
						MessageBox::Show(String::Format(L"An employee with NIC '{0}' already exists.", nic), L"Duplicate NIC", MessageBoxButtons::OK, MessageBoxIcon::Warning);
						this->txtEmpNic->Focus();
						return;
					}

					// Secure SHA-256 password hash
					String^ passwordHash = DatabaseService::HashPassword(password);

					String^ insertSql = 
						L"INSERT INTO employees (name, nic, contact, address, role, is_active, username, password_hash) "
						L"VALUES (@name, @nic, @contact, @address, @role, @active, @user, @hash);";

					MySqlCommand^ insCmd = gcnew MySqlCommand(insertSql, conn);
					insCmd->Parameters->AddWithValue(L"@name", name);
					insCmd->Parameters->AddWithValue(L"@nic", nic);
					insCmd->Parameters->AddWithValue(L"@contact", contact);
					insCmd->Parameters->AddWithValue(L"@address", address);
					insCmd->Parameters->AddWithValue(L"@role", role);
					insCmd->Parameters->AddWithValue(L"@active", isActive);
					insCmd->Parameters->AddWithValue(L"@user", username);
					insCmd->Parameters->AddWithValue(L"@hash", passwordHash);
					insCmd->ExecuteNonQuery();

					MessageBox::Show(
						String::Format(L"Employee '{0}' ({1} - {2}) created successfully!", name, username, role),
						L"Employee Registered",
						MessageBoxButtons::OK,
						MessageBoxIcon::Information
					);

					ResetEmployeeForm();
					LoadEmployeesData(nullptr, nullptr);
				}
				finally
				{
					conn->Close();
				}
			}
			catch (Exception^ ex)
			{
				MessageBox::Show(
					String::Format(L"Failed to register employee:\n{0}", ex->Message),
					L"Database Error",
					MessageBoxButtons::OK,
					MessageBoxIcon::Error
				);
			}
		}

		System::Void btnEmpUpdate_Click(System::Object^ sender, System::EventArgs^ e)
		{
			UserSession^ user = UserSession::CurrentUser;
			if (user == nullptr || !user->CanManageEmployees())
			{
				MessageBox::Show(L"Access Denied: Only Administrators can update employee profiles.", L"Permission Denied", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}

			if (this->selectedEmpId <= 0)
			{
				MessageBox::Show(L"Please select an employee from the table to edit.", L"Selection Required", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}

			String^ name = this->txtEmpName->Text->Trim();
			String^ nic = this->txtEmpNic->Text->Trim();
			String^ contact = this->txtEmpContact->Text->Trim();
			String^ address = this->txtEmpAddress->Text->Trim();
			String^ role = this->cmbEmpRole->SelectedItem != nullptr ? this->cmbEmpRole->SelectedItem->ToString() : L"Checker";
			int isActive = this->chkEmpActive->Checked ? 1 : 0;

			if (String::IsNullOrEmpty(name) || String::IsNullOrEmpty(nic) || String::IsNullOrEmpty(contact) || String::IsNullOrEmpty(address))
			{
				MessageBox::Show(L"All employee details are required.", L"Validation Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}

			// Prevent Admin from demoting or deactivating themselves
			if (this->selectedEmpId == user->EmployeeId && !role->Equals(L"Admin", StringComparison::OrdinalIgnoreCase))
			{
				MessageBox::Show(L"Security Restriction: You cannot remove Administrator role from your own active account.", L"Role Restriction", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}

			if (this->selectedEmpId == user->EmployeeId && isActive == 0)
			{
				MessageBox::Show(L"Security Restriction: You cannot deactivate your own active account.", L"Security Restriction", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}

			try
			{
				MySqlConnection^ conn = DatabaseService::CreateConnection();
				try
				{
					MySqlCommand^ nChk = gcnew MySqlCommand(L"SELECT COUNT(*) FROM employees WHERE nic = @nic AND emp_id != @id;", conn);
					nChk->Parameters->AddWithValue(L"@nic", nic);
					nChk->Parameters->AddWithValue(L"@id", this->selectedEmpId);
					if (Convert::ToInt32(nChk->ExecuteScalar()) > 0)
					{
						MessageBox::Show(String::Format(L"NIC '{0}' is already registered to another staff member.", nic), L"Duplicate NIC", MessageBoxButtons::OK, MessageBoxIcon::Warning);
						return;
					}

					String^ updateSql = 
						L"UPDATE employees SET name = @name, nic = @nic, contact = @contact, address = @address, role = @role, is_active = @active "
						L"WHERE emp_id = @id;";

					MySqlCommand^ updCmd = gcnew MySqlCommand(updateSql, conn);
					updCmd->Parameters->AddWithValue(L"@name", name);
					updCmd->Parameters->AddWithValue(L"@nic", nic);
					updCmd->Parameters->AddWithValue(L"@contact", contact);
					updCmd->Parameters->AddWithValue(L"@address", address);
					updCmd->Parameters->AddWithValue(L"@role", role);
					updCmd->Parameters->AddWithValue(L"@active", isActive);
					updCmd->Parameters->AddWithValue(L"@id", this->selectedEmpId);
					updCmd->ExecuteNonQuery();

					MessageBox::Show(
						String::Format(L"Employee profile #{0} updated successfully!", this->selectedEmpId),
						L"Update Saved",
						MessageBoxButtons::OK,
						MessageBoxIcon::Information
					);

					ResetEmployeeForm();
					LoadEmployeesData(nullptr, nullptr);
				}
				finally
				{
					conn->Close();
				}
			}
			catch (Exception^ ex)
			{
				MessageBox::Show(
					String::Format(L"Failed to update employee:\n{0}", ex->Message),
					L"Database Error",
					MessageBoxButtons::OK,
					MessageBoxIcon::Error
				);
			}
		}

		System::Void btnEmpResetPass_Click(System::Object^ sender, System::EventArgs^ e)
		{
			UserSession^ user = UserSession::CurrentUser;
			if (user == nullptr || !user->CanManageEmployees())
			{
				MessageBox::Show(L"Access Denied: Only Administrators can reset passwords.", L"Permission Denied", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}

			if (this->selectedEmpId <= 0)
			{
				MessageBox::Show(L"Please select an employee from the table first.", L"Selection Required", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}

			String^ newPassword = this->txtEmpPassword->Text;
			if (String::IsNullOrEmpty(newPassword) || newPassword->Length < 6)
			{
				MessageBox::Show(L"Please enter the new password (minimum 6 characters) in the Password field above.", L"Password Required", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				this->txtEmpPassword->Focus();
				return;
			}

			System::Windows::Forms::DialogResult dr = MessageBox::Show(
				String::Format(L"Are you sure you want to reset the password for employee '{0}' ({1})?", this->txtEmpName->Text, this->txtEmpUsername->Text),
				L"Confirm Password Reset",
				MessageBoxButtons::YesNo,
				MessageBoxIcon::Question
			);

			if (dr == System::Windows::Forms::DialogResult::Yes)
			{
				try
				{
					MySqlConnection^ conn = DatabaseService::CreateConnection();
					try
					{
						String^ newHash = DatabaseService::HashPassword(newPassword);
						MySqlCommand^ cmd = gcnew MySqlCommand(L"UPDATE employees SET password_hash = @hash WHERE emp_id = @id;", conn);
						cmd->Parameters->AddWithValue(L"@hash", newHash);
						cmd->Parameters->AddWithValue(L"@id", this->selectedEmpId);
						cmd->ExecuteNonQuery();

						this->txtEmpPassword->Clear();

						MessageBox::Show(
							String::Format(L"Password for '{0}' has been securely updated!", this->txtEmpUsername->Text),
							L"Password Reset Successful",
							MessageBoxButtons::OK,
							MessageBoxIcon::Information
						);
					}
					finally
					{
						conn->Close();
					}
				}
				catch (Exception^ ex)
				{
					MessageBox::Show(
						String::Format(L"Failed to reset password:\n{0}", ex->Message),
						L"Database Error",
						MessageBoxButtons::OK,
						MessageBoxIcon::Error
					);
				}
			}
		}

		System::Void btnEmpToggleActive_Click(System::Object^ sender, System::EventArgs^ e)
		{
			UserSession^ user = UserSession::CurrentUser;
			if (user == nullptr || !user->CanManageEmployees())
			{
				MessageBox::Show(L"Access Denied: Only Administrators can toggle account status.", L"Permission Denied", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}

			if (this->selectedEmpId <= 0)
			{
				MessageBox::Show(L"Please select an employee from the table first.", L"Selection Required", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}

			if (this->selectedEmpId == user->EmployeeId)
			{
				MessageBox::Show(L"Security Restriction: You cannot deactivate your own active Administrator account.", L"Self-Deactivation Blocked", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}

			bool currentlyActive = this->chkEmpActive->Checked;
			int newActive = currentlyActive ? 0 : 1;
			String^ action = currentlyActive ? L"DEACTIVATE" : L"ACTIVATE";

			System::Windows::Forms::DialogResult dr = MessageBox::Show(
				String::Format(L"Are you sure you want to {0} the account for '{1}' ({2})?", action, this->txtEmpName->Text, this->txtEmpUsername->Text),
				L"Confirm Account Status Change",
				MessageBoxButtons::YesNo,
				MessageBoxIcon::Warning
			);

			if (dr == System::Windows::Forms::DialogResult::Yes)
			{
				try
				{
					MySqlConnection^ conn = DatabaseService::CreateConnection();
					try
					{
						MySqlCommand^ cmd = gcnew MySqlCommand(L"UPDATE employees SET is_active = @active WHERE emp_id = @id;", conn);
						cmd->Parameters->AddWithValue(L"@active", newActive);
						cmd->Parameters->AddWithValue(L"@id", this->selectedEmpId);
						cmd->ExecuteNonQuery();

						MessageBox::Show(
							String::Format(L"Employee account has been {0}D.", action),
							L"Status Changed",
							MessageBoxButtons::OK,
							MessageBoxIcon::Information
						);

						ResetEmployeeForm();
						LoadEmployeesData(nullptr, nullptr);
					}
					finally
					{
						conn->Close();
					}
				}
				catch (Exception^ ex)
				{
					MessageBox::Show(
						String::Format(L"Failed to change account status:\n{0}", ex->Message),
						L"Database Error",
						MessageBoxButtons::OK,
						MessageBoxIcon::Error
					);
				}
			}
		}

		System::Void btnEmpClear_Click(System::Object^ sender, System::EventArgs^ e)
		{
			ResetEmployeeForm();
		}

		System::Void dgvEmployees_CellClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellEventArgs^ e)
		{
			if (e->RowIndex >= 0 && e->RowIndex < this->dgvEmployees->Rows->Count)
			{
				DataGridViewRow^ row = this->dgvEmployees->Rows[e->RowIndex];
				if (row->Cells[L"ID"]->Value != nullptr)
				{
					this->selectedEmpId = Convert::ToInt32(row->Cells[L"ID"]->Value);
					this->txtEmpName->Text = row->Cells[L"Full Name"]->Value != nullptr ? row->Cells[L"Full Name"]->Value->ToString() : L"";
					this->txtEmpUsername->Text = row->Cells[L"Username"]->Value != nullptr ? row->Cells[L"Username"]->Value->ToString() : L"";
					this->txtEmpContact->Text = row->Cells[L"Phone Number"]->Value != nullptr ? row->Cells[L"Phone Number"]->Value->ToString() : L"";
					this->txtEmpNic->Text = row->Cells[L"NIC Number"]->Value != nullptr ? row->Cells[L"NIC Number"]->Value->ToString() : L"";
					this->txtEmpAddress->Text = row->Cells[L"Address"]->Value != nullptr ? row->Cells[L"Address"]->Value->ToString() : L"";

					String^ roleVal = row->Cells[L"Role"]->Value != nullptr ? row->Cells[L"Role"]->Value->ToString() : L"Checker";
					for (int i = 0; i < this->cmbEmpRole->Items->Count; i++)
					{
						if (this->cmbEmpRole->Items[i]->ToString()->Equals(roleVal, StringComparison::OrdinalIgnoreCase))
						{
							this->cmbEmpRole->SelectedIndex = i;
							break;
						}
					}

					String^ statusVal = row->Cells[L"Account Status"]->Value != nullptr ? row->Cells[L"Account Status"]->Value->ToString() : L"Active";
					bool isActive = statusVal->Equals(L"Active", StringComparison::OrdinalIgnoreCase);
					this->chkEmpActive->Checked = isActive;

					this->txtEmpPassword->Clear();
					this->txtEmpUsername->ReadOnly = true;

					this->lblEmpIdStatus->Text = String::Format(L"Mode: [Editing Employee #{0} - {1}]", this->selectedEmpId, this->txtEmpUsername->Text);
					this->lblEmpIdStatus->ForeColor = System::Drawing::Color::FromArgb(21, 87, 36);

					this->btnEmpSave->Enabled = false;
					this->btnEmpUpdate->Enabled = true;
					this->btnEmpResetPass->Enabled = true;
					this->btnEmpToggleActive->Enabled = true;
					this->btnEmpToggleActive->Text = isActive ? L"Deactivate" : L"Activate";
					this->btnEmpToggleActive->BackColor = isActive ? System::Drawing::Color::FromArgb(192, 57, 43) : System::Drawing::Color::FromArgb(39, 174, 96);
				}
			}
		}

		System::Void btnEmpSearch_Click(System::Object^ sender, System::EventArgs^ e)
		{
			String^ query = this->txtEmpSearch->Text->Trim();
			String^ role = this->cmbFilterEmpRole->SelectedItem != nullptr ? this->cmbFilterEmpRole->SelectedItem->ToString() : L"";
			LoadEmployeesData(query, role);
		}

		System::Void btnEmpReset_Click(System::Object^ sender, System::EventArgs^ e)
		{
			this->txtEmpSearch->Clear();
			this->cmbFilterEmpRole->SelectedIndex = 0;
			LoadEmployeesData(nullptr, nullptr);
		}

		System::Void txtEmpSearch_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e)
		{
			if (e->KeyCode == Keys::Enter)
			{
				e->SuppressKeyPress = true;
				btnEmpSearch_Click(sender, e);
			}
		}

		// Helper to highlight active navigation button
		void SetActiveNavButton(Button^ activeButton)
		{
			this->btnNavDashboard->BackColor = System::Drawing::Color::Transparent;
			this->btnNavCustomers->BackColor = System::Drawing::Color::Transparent;
			this->btnNavJobs->BackColor = System::Drawing::Color::Transparent;
			this->btnNavEmployees->BackColor = System::Drawing::Color::Transparent;
			this->btnNavReports->BackColor = System::Drawing::Color::Transparent;

			activeButton->BackColor = System::Drawing::Color::FromArgb(69, 123, 157);
		}

		// ----------------------------------------------------
		// 13. Monthly Income Reports Module (Phase 10)
		// ----------------------------------------------------
		void InitReportSelectors()
		{
			this->cmbReportMonth->Items->Clear();
			this->cmbReportMonth->Items->Add(L"January");
			this->cmbReportMonth->Items->Add(L"February");
			this->cmbReportMonth->Items->Add(L"March");
			this->cmbReportMonth->Items->Add(L"April");
			this->cmbReportMonth->Items->Add(L"May");
			this->cmbReportMonth->Items->Add(L"June");
			this->cmbReportMonth->Items->Add(L"July");
			this->cmbReportMonth->Items->Add(L"August");
			this->cmbReportMonth->Items->Add(L"September");
			this->cmbReportMonth->Items->Add(L"October");
			this->cmbReportMonth->Items->Add(L"November");
			this->cmbReportMonth->Items->Add(L"December");

			DateTime now = DateTime::Now;
			this->cmbReportMonth->SelectedIndex = now.Month - 1;

			this->cmbReportYear->Items->Clear();
			for (int y = now.Year - 3; y <= now.Year + 3; y++)
			{
				this->cmbReportYear->Items->Add(y.ToString());
			}
			this->cmbReportYear->SelectedItem = now.Year.ToString();
		}

		void LoadReportsData()
		{
			try
			{
				if (this->cmbReportMonth->SelectedIndex < 0 || this->cmbReportYear->SelectedItem == nullptr)
				{
					return;
				}

				int month = this->cmbReportMonth->SelectedIndex + 1;
				int year = Convert::ToInt32(this->cmbReportYear->SelectedItem->ToString());
				String^ monthName = this->cmbReportMonth->SelectedItem->ToString();

				this->lblRepPeriodVal->Text = String::Format(L"{0} {1}", monthName, year);

				MySqlConnection^ conn = DatabaseService::CreateConnection();
				try
				{
					// 1. Calculate aggregated financial metrics for Finished jobs in the selected month & year
					String^ statsSql = 
						L"SELECT "
						L"  COUNT(*) AS total_finished, "
						L"  IFNULL(SUM(cost), 0.00) AS total_income, "
						L"  IFNULL(AVG(cost), 0.00) AS avg_cost "
						L"FROM repair_jobs "
						L"WHERE status = 'Finished' "
						L"  AND MONTH(COALESCE(finished_at, created_at)) = @month "
						L"  AND YEAR(COALESCE(finished_at, created_at)) = @year;";

					MySqlCommand^ cmdStats = gcnew MySqlCommand(statsSql, conn);
					cmdStats->Parameters->AddWithValue(L"@month", month);
					cmdStats->Parameters->AddWithValue(L"@year", year);

					MySqlDataReader^ reader = cmdStats->ExecuteReader();
					int totalFinished = 0;
					Decimal totalIncome = Decimal(0);
					Decimal avgCost = Decimal(0);

					if (reader->Read())
					{
						totalFinished = reader->IsDBNull(0) ? 0 : reader->GetInt32(0);
						totalIncome = reader->IsDBNull(1) ? Decimal(0) : reader->GetDecimal(1);
						avgCost = reader->IsDBNull(2) ? Decimal(0) : reader->GetDecimal(2);
					}
					reader->Close();

					this->lblRepFinishedVal->Text = totalFinished.ToString();
					this->lblRepIncomeVal->Text = String::Format(L"LKR {0:N2}", totalIncome);
					this->lblRepAvgVal->Text = String::Format(L"LKR {0:N2}", avgCost);

					// 2. Fetch itemized finished repairs breakdown
					String^ listSql =
						L"SELECT "
						L"  r.job_id AS 'Job ID', "
						L"  r.customer_name AS 'Customer Name', "
						L"  r.customer_contact AS 'Contact', "
						L"  r.phone_model AS 'Phone Model', "
						L"  r.details AS 'Problem Details', "
						L"  IFNULL(e.name, 'Unassigned') AS 'Repairer', "
						L"  DATE_FORMAT(COALESCE(r.finished_at, r.created_at), '%Y-%m-%d %H:%i') AS 'Completed Date', "
						L"  CONCAT('LKR ', FORMAT(r.cost, 2)) AS 'Repair Cost' "
						L"FROM repair_jobs r "
						L"LEFT JOIN employees e ON r.repairer_id = e.emp_id "
						L"WHERE r.status = 'Finished' "
						L"  AND MONTH(COALESCE(r.finished_at, r.created_at)) = @month "
						L"  AND YEAR(COALESCE(r.finished_at, r.created_at)) = @year "
						L"ORDER BY COALESCE(r.finished_at, r.created_at) DESC, r.job_id DESC;";

					MySqlCommand^ cmdList = gcnew MySqlCommand(listSql, conn);
					cmdList->Parameters->AddWithValue(L"@month", month);
					cmdList->Parameters->AddWithValue(L"@year", year);

					MySqlDataAdapter^ adapter = gcnew MySqlDataAdapter(cmdList);
					DataTable^ dt = gcnew DataTable();
					adapter->Fill(dt);

					this->dgvReportJobs->DataSource = dt;
					this->lblReportCount->Text = String::Format(L"{0} finished repair(s) in {1} {2}", dt->Rows->Count, monthName, year);

					if (this->dgvReportJobs->Columns[L"Job ID"] != nullptr)
						this->dgvReportJobs->Columns[L"Job ID"]->FillWeight = 50;
					if (this->dgvReportJobs->Columns[L"Customer Name"] != nullptr)
						this->dgvReportJobs->Columns[L"Customer Name"]->FillWeight = 110;
					if (this->dgvReportJobs->Columns[L"Contact"] != nullptr)
						this->dgvReportJobs->Columns[L"Contact"]->FillWeight = 85;
					if (this->dgvReportJobs->Columns[L"Phone Model"] != nullptr)
						this->dgvReportJobs->Columns[L"Phone Model"]->FillWeight = 95;
					if (this->dgvReportJobs->Columns[L"Problem Details"] != nullptr)
						this->dgvReportJobs->Columns[L"Problem Details"]->FillWeight = 130;
					if (this->dgvReportJobs->Columns[L"Repairer"] != nullptr)
						this->dgvReportJobs->Columns[L"Repairer"]->FillWeight = 95;
					if (this->dgvReportJobs->Columns[L"Completed Date"] != nullptr)
						this->dgvReportJobs->Columns[L"Completed Date"]->FillWeight = 100;
					if (this->dgvReportJobs->Columns[L"Repair Cost"] != nullptr)
						this->dgvReportJobs->Columns[L"Repair Cost"]->FillWeight = 80;
				}
				finally
				{
					conn->Close();
				}
			}
			catch (Exception^ ex)
			{
				MessageBox::Show(
					String::Format(L"Failed to generate monthly income report:\n{0}", ex->Message),
					L"Report Error",
					MessageBoxButtons::OK,
					MessageBoxIcon::Error
				);
			}
		}

		void ExportReportToCsv()
		{
			if (this->dgvReportJobs->DataSource == nullptr)
			{
				MessageBox::Show(L"Please generate a report first before exporting.", L"No Report Data", MessageBoxButtons::OK, MessageBoxIcon::Information);
				return;
			}

			DataTable^ dt = dynamic_cast<DataTable^>(this->dgvReportJobs->DataSource);
			if (dt == nullptr || dt->Rows->Count == 0)
			{
				MessageBox::Show(L"There are no completed repair jobs in the selected period to export.", L"No Data to Export", MessageBoxButtons::OK, MessageBoxIcon::Information);
				return;
			}

			int year = Convert::ToInt32(this->cmbReportYear->SelectedItem->ToString());
			String^ monthName = this->cmbReportMonth->SelectedItem->ToString();

			SaveFileDialog^ sfd = gcnew SaveFileDialog();
			sfd->Filter = L"CSV Files (*.csv)|*.csv|All Files (*.*)|*.*";
			sfd->FileName = String::Format(L"FixSphere_Income_Report_{0}_{1}.csv", year, monthName);
			sfd->Title = L"Export Monthly Income Report to CSV";

			if (sfd->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				try
				{
					System::IO::StreamWriter^ sw = gcnew System::IO::StreamWriter(sfd->FileName, false, System::Text::Encoding::UTF8);

					// Title and metadata block
					sw->WriteLine(L"\"FixSphere - Mobile Phone Repair Center Management System\"");
					sw->WriteLine(L"\"Monthly Income & Financial Report\"");
					sw->WriteLine(String::Format(L"\"Report Period:\",\"{0} {1}\"", monthName, year));
					sw->WriteLine(String::Format(L"\"Generated By:\",\"{0} ({1})\"", UserSession::CurrentUser != nullptr ? UserSession::CurrentUser->FullName : L"Admin", UserSession::CurrentUser != nullptr ? UserSession::CurrentUser->Username : L"admin"));
					sw->WriteLine(String::Format(L"\"Generated On:\",\"{0}\"", DateTime::Now.ToString(L"yyyy-MM-dd HH:mm:ss")));
					sw->WriteLine(String::Format(L"\"Total Finished Jobs:\",\"{0}\"", this->lblRepFinishedVal->Text));
					sw->WriteLine(String::Format(L"\"Total Monthly Income:\",\"{0}\"", this->lblRepIncomeVal->Text));
					sw->WriteLine(String::Format(L"\"Average Income / Job:\",\"{0}\"", this->lblRepAvgVal->Text));
					sw->WriteLine(L"");

					// Header row
					System::Text::StringBuilder^ headerSb = gcnew System::Text::StringBuilder();
					for (int i = 0; i < dt->Columns->Count; i++)
					{
						if (i > 0) headerSb->Append(L",");
						headerSb->Append(L"\"" + dt->Columns[i]->ColumnName->Replace(L"\"", L"\"\"") + L"\"");
					}
					sw->WriteLine(headerSb->ToString());

					// Data rows
					for (int r = 0; r < dt->Rows->Count; r++)
					{
						System::Text::StringBuilder^ rowSb = gcnew System::Text::StringBuilder();
						for (int c = 0; c < dt->Columns->Count; c++)
						{
							if (c > 0) rowSb->Append(L",");
							String^ val = dt->Rows[r][c] != nullptr ? dt->Rows[r][c]->ToString() : L"";
							rowSb->Append(L"\"" + val->Replace(L"\"", L"\"\"") + L"\"");
						}
						sw->WriteLine(rowSb->ToString());
					}

					sw->Close();

					MessageBox::Show(
						String::Format(L"Monthly Income Report exported successfully!\n\nSaved to:\n{0}", sfd->FileName),
						L"Export Successful",
						MessageBoxButtons::OK,
						MessageBoxIcon::Information
					);
				}
				catch (Exception^ ex)
				{
					MessageBox::Show(
						String::Format(L"Failed to export CSV file:\n{0}", ex->Message),
						L"Export Error",
						MessageBoxButtons::OK,
						MessageBoxIcon::Error
					);
				}
			}
		}

		System::Void btnGenerateReport_Click(System::Object^ sender, System::EventArgs^ e)
		{
			LoadReportsData();
		}

		System::Void btnCurrentMonth_Click(System::Object^ sender, System::EventArgs^ e)
		{
			DateTime now = DateTime::Now;
			this->cmbReportMonth->SelectedIndex = now.Month - 1;
			this->cmbReportYear->SelectedItem = now.Year.ToString();
			LoadReportsData();
		}

		System::Void btnExportCsv_Click(System::Object^ sender, System::EventArgs^ e)
		{
			ExportReportToCsv();
		}

		// ----------------------------------------------------
		// Navigation Button Event Handlers
		// ----------------------------------------------------
		System::Void btnNavDashboard_Click(System::Object^ sender, System::EventArgs^ e)
		{
			SetActiveNavButton(this->btnNavDashboard);
			this->lblViewTitle->Text = L"Dashboard Overview";
			this->pnlViewCustomers->Visible = false;
			this->pnlViewJobs->Visible = false;
			this->pnlViewEmployees->Visible = false;
			this->pnlViewReports->Visible = false;
			this->pnlPlaceholderView->Visible = false;
			this->pnlViewDashboard->Visible = true;
			LoadDashboardData();
		}

		System::Void btnNavCustomers_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (UserSession::CurrentUser == nullptr || !UserSession::CurrentUser->CanManageCustomers())
			{
				MessageBox::Show(L"Access Denied: You do not have permission to view Customer Management.", L"Permission Denied", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}
			SetActiveNavButton(this->btnNavCustomers);
			this->lblViewTitle->Text = L"Customer Management";
			this->pnlViewDashboard->Visible = false;
			this->pnlViewJobs->Visible = false;
			this->pnlViewEmployees->Visible = false;
			this->pnlViewReports->Visible = false;
			this->pnlPlaceholderView->Visible = false;
			this->pnlViewCustomers->Visible = true;
			ResetCustomerForm();
			LoadCustomersData(nullptr);
		}

		System::Void btnNavJobs_Click(System::Object^ sender, System::EventArgs^ e)
		{
			SetActiveNavButton(this->btnNavJobs);
			this->lblViewTitle->Text = L"Repair Job Management";
			this->pnlViewDashboard->Visible = false;
			this->pnlViewCustomers->Visible = false;
			this->pnlViewEmployees->Visible = false;
			this->pnlViewReports->Visible = false;
			this->pnlPlaceholderView->Visible = false;
			this->pnlViewJobs->Visible = true;
			PopulateCustomerDropdown();
			PopulateRepairerDropdown();
			ResetJobForm();
			this->cmbFilterStatus->SelectedIndex = 0;
			LoadJobsData(nullptr, nullptr);
		}

		System::Void btnNavEmployees_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (UserSession::CurrentUser == nullptr || !UserSession::CurrentUser->CanManageEmployees())
			{
				MessageBox::Show(L"Access Denied: Employee Management is restricted to Administrators.", L"Permission Denied", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}
			SetActiveNavButton(this->btnNavEmployees);
			this->lblViewTitle->Text = L"Employee Management (Admin Access)";
			this->pnlViewDashboard->Visible = false;
			this->pnlViewCustomers->Visible = false;
			this->pnlViewJobs->Visible = false;
			this->pnlViewReports->Visible = false;
			this->pnlPlaceholderView->Visible = false;
			this->pnlViewEmployees->Visible = true;
			ResetEmployeeForm();
			this->cmbFilterEmpRole->SelectedIndex = 0;
			LoadEmployeesData(nullptr, nullptr);
		}

		System::Void btnNavReports_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (UserSession::CurrentUser == nullptr || !UserSession::CurrentUser->CanViewFinancialReports())
			{
				MessageBox::Show(L"Access Denied: Financial and Income Reports are restricted to Administrators.", L"Permission Denied", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}
			SetActiveNavButton(this->btnNavReports);
			this->lblViewTitle->Text = L"Income & Financial Reports (Admin Only)";
			this->pnlViewDashboard->Visible = false;
			this->pnlViewCustomers->Visible = false;
			this->pnlViewJobs->Visible = false;
			this->pnlViewEmployees->Visible = false;
			this->pnlPlaceholderView->Visible = false;
			this->pnlViewReports->Visible = true;
			LoadReportsData();
		}

		System::Void btnRefreshDashboard_Click(System::Object^ sender, System::EventArgs^ e)
		{
			LoadDashboardData();
		}

		System::Void btnLogout_Click(System::Object^ sender, System::EventArgs^ e)
		{
			System::Windows::Forms::DialogResult dr = MessageBox::Show(
				L"Are you sure you want to sign out of FixSphere?",
				L"Confirm Sign Out",
				MessageBoxButtons::YesNo,
				MessageBoxIcon::Question
			);

			if (dr == System::Windows::Forms::DialogResult::Yes)
			{
				UserSession::Logout();
				this->Close();
			}
		}
	};
}
