#include "LoginForm.h"
#include "MainForm.h"
#include "UserSession.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<System::String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	// Main application loop: Login -> Work in Main Window -> Sign Out returns to Login
	while (true)
	{
		FixSphere::LoginForm^ loginForm = gcnew FixSphere::LoginForm();
		if (loginForm->ShowDialog() == DialogResult::OK)
		{
			FixSphere::MainForm^ mainForm = gcnew FixSphere::MainForm();
			Application::Run(mainForm);

			// If the user signed out, UserSession::CurrentUser is reset to nullptr -> loop back to login form
			if (!FixSphere::UserSession::IsLoggedIn())
			{
				continue;
			}
			else
			{
				// Main window closed directly -> exit cleanly
				break;
			}
		}
		else
		{
			// User cancelled or exited login screen
			break;
		}
	}

	return 0;
}
