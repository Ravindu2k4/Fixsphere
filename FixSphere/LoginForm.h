#pragma once

#include "DatabaseService.h"
#include "UserSession.h"

namespace FixSphere {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace MySql::Data::MySqlClient;

	public ref class LoginForm : public System::Windows::Forms::Form
	{
	public:
		LoginForm(void)
		{
			InitializeComponent();
		}

	protected:
		~LoginForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		System::ComponentModel::Container ^components;
		System::Windows::Forms::Panel^ pnlHeader;
		System::Windows::Forms::Label^ lblBrand;
		System::Windows::Forms::Label^ lblSubtitle;
		System::Windows::Forms::Panel^ pnlCard;
		System::Windows::Forms::Label^ lblLoginTitle;
		System::Windows::Forms::Label^ lblLoginDesc;
		System::Windows::Forms::Label^ lblUser;
		System::Windows::Forms::TextBox^ txtUsername;
		System::Windows::Forms::Label^ lblPass;
		System::Windows::Forms::TextBox^ txtPassword;
		System::Windows::Forms::CheckBox^ chkShowPass;
		System::Windows::Forms::Label^ lblError;
		System::Windows::Forms::Button^ btnLogin;
		System::Windows::Forms::Button^ btnCancel;
		System::Windows::Forms::Label^ lblRolesHint;

		void InitializeComponent(void)
		{
			this->components = gcnew System::ComponentModel::Container();

			this->pnlHeader = (gcnew System::Windows::Forms::Panel());
			this->lblBrand = (gcnew System::Windows::Forms::Label());
			this->lblSubtitle = (gcnew System::Windows::Forms::Label());
			this->pnlCard = (gcnew System::Windows::Forms::Panel());
			this->lblLoginTitle = (gcnew System::Windows::Forms::Label());
			this->lblLoginDesc = (gcnew System::Windows::Forms::Label());
			this->lblUser = (gcnew System::Windows::Forms::Label());
			this->txtUsername = (gcnew System::Windows::Forms::TextBox());
			this->lblPass = (gcnew System::Windows::Forms::Label());
			this->txtPassword = (gcnew System::Windows::Forms::TextBox());
			this->chkShowPass = (gcnew System::Windows::Forms::CheckBox());
			this->lblError = (gcnew System::Windows::Forms::Label());
			this->btnLogin = (gcnew System::Windows::Forms::Button());
			this->btnCancel = (gcnew System::Windows::Forms::Button());
			this->lblRolesHint = (gcnew System::Windows::Forms::Label());

			this->pnlHeader->SuspendLayout();
			this->pnlCard->SuspendLayout();
			this->SuspendLayout();

			// pnlHeader: Primary Navy #1D3557 (29, 53, 87)
			this->pnlHeader->BackColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->pnlHeader->Controls->Add(this->lblSubtitle);
			this->pnlHeader->Controls->Add(this->lblBrand);
			this->pnlHeader->Dock = System::Windows::Forms::DockStyle::Top;
			this->pnlHeader->Location = System::Drawing::Point(0, 0);
			this->pnlHeader->Name = L"pnlHeader";
			this->pnlHeader->Size = System::Drawing::Size(480, 85);
			this->pnlHeader->TabIndex = 0;

			// lblBrand
			this->lblBrand->AutoSize = true;
			this->lblBrand->Font = (gcnew System::Drawing::Font(L"Segoe UI", 20, System::Drawing::FontStyle::Bold));
			this->lblBrand->ForeColor = System::Drawing::Color::White;
			this->lblBrand->Location = System::Drawing::Point(24, 14);
			this->lblBrand->Name = L"lblBrand";
			this->lblBrand->Size = System::Drawing::Size(142, 37);
			this->lblBrand->TabIndex = 0;
			this->lblBrand->Text = L"FixSphere";

			// lblSubtitle: Light Cyan #A8DADC (168, 218, 220)
			this->lblSubtitle->AutoSize = true;
			this->lblSubtitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.25F));
			this->lblSubtitle->ForeColor = System::Drawing::Color::FromArgb(168, 218, 220);
			this->lblSubtitle->Location = System::Drawing::Point(26, 52);
			this->lblSubtitle->Name = L"lblSubtitle";
			this->lblSubtitle->Size = System::Drawing::Size(306, 17);
			this->lblSubtitle->TabIndex = 1;
			this->lblSubtitle->Text = L"Mobile Phone Repair Center Management System";

			// pnlCard: White modern card on light background
			this->pnlCard->BackColor = System::Drawing::Color::White;
			this->pnlCard->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pnlCard->Controls->Add(this->lblRolesHint);
			this->pnlCard->Controls->Add(this->btnCancel);
			this->pnlCard->Controls->Add(this->btnLogin);
			this->pnlCard->Controls->Add(this->lblError);
			this->pnlCard->Controls->Add(this->chkShowPass);
			this->pnlCard->Controls->Add(this->txtPassword);
			this->pnlCard->Controls->Add(this->lblPass);
			this->pnlCard->Controls->Add(this->txtUsername);
			this->pnlCard->Controls->Add(this->lblUser);
			this->pnlCard->Controls->Add(this->lblLoginDesc);
			this->pnlCard->Controls->Add(this->lblLoginTitle);
			this->pnlCard->Location = System::Drawing::Point(35, 110);
			this->pnlCard->Name = L"pnlCard";
			this->pnlCard->Size = System::Drawing::Size(410, 425);
			this->pnlCard->TabIndex = 1;

			// lblLoginTitle
			this->lblLoginTitle->AutoSize = true;
			this->lblLoginTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 14, System::Drawing::FontStyle::Bold));
			this->lblLoginTitle->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblLoginTitle->Location = System::Drawing::Point(25, 20);
			this->lblLoginTitle->Name = L"lblLoginTitle";
			this->lblLoginTitle->Size = System::Drawing::Size(176, 25);
			this->lblLoginTitle->TabIndex = 0;
			this->lblLoginTitle->Text = L"Employee Sign In";

			// lblLoginDesc
			this->lblLoginDesc->AutoSize = true;
			this->lblLoginDesc->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			this->lblLoginDesc->ForeColor = System::Drawing::Color::FromArgb(108, 117, 125);
			this->lblLoginDesc->Location = System::Drawing::Point(26, 48);
			this->lblLoginDesc->Name = L"lblLoginDesc";
			this->lblLoginDesc->Size = System::Drawing::Size(273, 15);
			this->lblLoginDesc->TabIndex = 1;
			this->lblLoginDesc->Text = L"Enter your credentials to access repair management.";

			// lblUser
			this->lblUser->AutoSize = true;
			this->lblUser->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5F, System::Drawing::FontStyle::Bold));
			this->lblUser->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblUser->Location = System::Drawing::Point(26, 85);
			this->lblUser->Name = L"lblUser";
			this->lblUser->Size = System::Drawing::Size(69, 17);
			this->lblUser->TabIndex = 2;
			this->lblUser->Text = L"Username";

			// txtUsername
			this->txtUsername->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10.5F));
			this->txtUsername->Location = System::Drawing::Point(29, 107);
			this->txtUsername->Name = L"txtUsername";
			this->txtUsername->Size = System::Drawing::Size(350, 26);
			this->txtUsername->TabIndex = 3;
			this->txtUsername->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &LoginForm::txtInput_KeyDown);

			// lblPass
			this->lblPass->AutoSize = true;
			this->lblPass->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5F, System::Drawing::FontStyle::Bold));
			this->lblPass->ForeColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->lblPass->Location = System::Drawing::Point(26, 148);
			this->lblPass->Name = L"lblPass";
			this->lblPass->Size = System::Drawing::Size(66, 17);
			this->lblPass->TabIndex = 4;
			this->lblPass->Text = L"Password";

			// txtPassword
			this->txtPassword->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10.5F));
			this->txtPassword->Location = System::Drawing::Point(29, 170);
			this->txtPassword->Name = L"txtPassword";
			this->txtPassword->PasswordChar = '*';
			this->txtPassword->Size = System::Drawing::Size(350, 26);
			this->txtPassword->TabIndex = 5;
			this->txtPassword->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &LoginForm::txtInput_KeyDown);

			// chkShowPass
			this->chkShowPass->AutoSize = true;
			this->chkShowPass->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.5F));
			this->chkShowPass->ForeColor = System::Drawing::Color::FromArgb(108, 117, 125);
			this->chkShowPass->Location = System::Drawing::Point(29, 203);
			this->chkShowPass->Name = L"chkShowPass";
			this->chkShowPass->Size = System::Drawing::Size(107, 19);
			this->chkShowPass->TabIndex = 6;
			this->chkShowPass->Text = L"Show password";
			this->chkShowPass->UseVisualStyleBackColor = true;
			this->chkShowPass->CheckedChanged += gcnew System::EventHandler(this, &LoginForm::chkShowPass_CheckedChanged);

			// lblError
			this->lblError->AutoSize = false;
			this->lblError->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.5F, System::Drawing::FontStyle::Bold));
			this->lblError->ForeColor = System::Drawing::Color::FromArgb(220, 53, 69);
			this->lblError->Location = System::Drawing::Point(26, 230);
			this->lblError->Name = L"lblError";
			this->lblError->Size = System::Drawing::Size(353, 35);
			this->lblError->TabIndex = 7;
			this->lblError->Text = L"";

			// btnLogin: Primary Navy #1D3557 (29, 53, 87)
			this->btnLogin->BackColor = System::Drawing::Color::FromArgb(29, 53, 87);
			this->btnLogin->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnLogin->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnLogin->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
			this->btnLogin->ForeColor = System::Drawing::Color::White;
			this->btnLogin->Location = System::Drawing::Point(29, 275);
			this->btnLogin->Name = L"btnLogin";
			this->btnLogin->Size = System::Drawing::Size(350, 42);
			this->btnLogin->TabIndex = 8;
			this->btnLogin->Text = L"Sign In";
			this->btnLogin->UseVisualStyleBackColor = false;
			this->btnLogin->Click += gcnew System::EventHandler(this, &LoginForm::btnLogin_Click);

			// btnCancel: Secondary outline button
			this->btnCancel->BackColor = System::Drawing::Color::FromArgb(244, 246, 249);
			this->btnCancel->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnCancel->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnCancel->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5F));
			this->btnCancel->ForeColor = System::Drawing::Color::FromArgb(69, 123, 157);
			this->btnCancel->Location = System::Drawing::Point(29, 325);
			this->btnCancel->Name = L"btnCancel";
			this->btnCancel->Size = System::Drawing::Size(350, 36);
			this->btnCancel->TabIndex = 9;
			this->btnCancel->Text = L"Exit Application";
			this->btnCancel->UseVisualStyleBackColor = false;
			this->btnCancel->Click += gcnew System::EventHandler(this, &LoginForm::btnCancel_Click);

			// lblRolesHint
			this->lblRolesHint->AutoSize = false;
			this->lblRolesHint->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8));
			this->lblRolesHint->ForeColor = System::Drawing::Color::FromArgb(108, 117, 125);
			this->lblRolesHint->Location = System::Drawing::Point(26, 375);
			this->lblRolesHint->Name = L"lblRolesHint";
			this->lblRolesHint->Size = System::Drawing::Size(353, 30);
			this->lblRolesHint->TabIndex = 10;
			this->lblRolesHint->Text = L"System roles: Admin, Checker, Repairer. Accounts are authenticated securely against MySQL.";
			this->lblRolesHint->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;

			// LoginForm
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(244, 246, 249);
			this->ClientSize = System::Drawing::Size(480, 560);
			this->Controls->Add(this->pnlCard);
			this->Controls->Add(this->pnlHeader);
			this->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"LoginForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"FixSphere - Employee Sign In";
			this->pnlHeader->ResumeLayout(false);
			this->pnlHeader->PerformLayout();
			this->pnlCard->ResumeLayout(false);
			this->pnlCard->PerformLayout();
			this->ResumeLayout(false);
		}

	private:
		System::Void chkShowPass_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
		{
			this->txtPassword->PasswordChar = this->chkShowPass->Checked ? '\0' : '*';
		}

		System::Void txtInput_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e)
		{
			if (e->KeyCode == Keys::Enter)
			{
				e->SuppressKeyPress = true;
				btnLogin_Click(sender, e);
			}
		}

		System::Void btnCancel_Click(System::Object^ sender, System::EventArgs^ e)
		{
			this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->Close();
		}

		System::Void btnLogin_Click(System::Object^ sender, System::EventArgs^ e)
		{
			this->lblError->Text = L"";

			String^ username = this->txtUsername->Text->Trim();
			String^ password = this->txtPassword->Text;

			if (String::IsNullOrEmpty(username))
			{
				this->lblError->Text = L"Please enter your username.";
				this->txtUsername->Focus();
				return;
			}

			if (String::IsNullOrEmpty(password))
			{
				this->lblError->Text = L"Please enter your password.";
				this->txtPassword->Focus();
				return;
			}

			this->btnLogin->Enabled = false;
			this->btnLogin->Text = L"Authenticating...";

			try
			{
				MySqlConnection^ conn = DatabaseService::CreateConnection();
				try
				{
					// Prepared statement query targeting employees table
					String^ query = L"SELECT emp_id, name, role, is_active, password_hash FROM employees WHERE username = @username LIMIT 1;";
					MySqlCommand^ cmd = gcnew MySqlCommand(query, conn);
					cmd->Parameters->AddWithValue(L"@username", username);

					MySqlDataReader^ reader = cmd->ExecuteReader();

					if (reader->Read())
					{
						int empId = reader->GetInt32(0);
						String^ fullName = reader->GetString(1);
						String^ role = reader->GetString(2);
						int isActive = reader->GetInt32(3);
						String^ dbHash = reader->GetString(4);

						reader->Close();

						// Check if account is active
						if (isActive == 0)
						{
							this->lblError->Text = L"Your account has been deactivated. Please contact an Administrator.";
							return;
						}

						// Calculate SHA-256 hash of entered password
						String^ enteredHash = DatabaseService::HashPassword(password);

						if (String::Equals(enteredHash, dbHash, StringComparison::OrdinalIgnoreCase))
						{
							// Login successful: record authenticated session
							UserSession::CurrentUser = gcnew UserSession(empId, fullName, username, role);

							this->DialogResult = System::Windows::Forms::DialogResult::OK;
							this->Close();
							return;
						}
						else
						{
							this->lblError->Text = L"Invalid username or password.";
							this->txtPassword->Clear();
							this->txtPassword->Focus();
						}
					}
					else
					{
						reader->Close();
						this->lblError->Text = L"Invalid username or password.";
						this->txtPassword->Clear();
						this->txtPassword->Focus();
					}
				}
				finally
				{
					conn->Close();
				}
			}
			catch (Exception^ ex)
			{
				this->lblError->Text = String::Format(L"Database error: {0}", ex->Message);
			}
			finally
			{
				this->btnLogin->Enabled = true;
				this->btnLogin->Text = L"Sign In";
			}
		}
	};
}
