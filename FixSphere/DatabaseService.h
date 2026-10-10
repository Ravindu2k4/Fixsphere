#pragma once

using namespace System;
using namespace System::Data;
using namespace System::IO;
using namespace System::Text;
using namespace System::Security::Cryptography;
using namespace MySql::Data::MySqlClient;

namespace FixSphere {

	public ref class DatabaseService
	{
	private:
		static String^ connectionString = nullptr;

		// Load connection parameters from local db_config.ini
		static void InitializeConnectionString()
		{
			if (!String::IsNullOrEmpty(connectionString))
			{
				return;
			}

			String^ appDir = AppDomain::CurrentDomain->BaseDirectory;
			String^ configPath = Path::Combine(appDir, L"db_config.ini");

			// Fallback: check parent directory if running inside x64/Debug
			if (!File::Exists(configPath))
			{
				String^ parentConfig = Path::Combine(appDir, L"..\\..\\db_config.ini");
				if (File::Exists(parentConfig))
				{
					configPath = parentConfig;
				}
				else
				{
					String^ siblingConfig = Path::Combine(appDir, L"..\\db_config.ini");
					if (File::Exists(siblingConfig))
					{
						configPath = siblingConfig;
					}
				}
			}

			String^ server = L"localhost";
			String^ port = L"3306";
			String^ database = L"fixsphere";
			String^ uid = L"root";
			String^ password = L"root";

			if (File::Exists(configPath))
			{
				array<String^>^ lines = File::ReadAllLines(configPath);
				for each (String^ line in lines)
				{
					String^ trimmed = line->Trim();
					if (trimmed->StartsWith(L"#") || trimmed->StartsWith(L";") || trimmed->StartsWith(L"["))
						continue;

					int eqIdx = trimmed->IndexOf(L'=');
					if (eqIdx > 0)
					{
						String^ key = trimmed->Substring(0, eqIdx)->Trim()->ToLower();
						String^ val = trimmed->Substring(eqIdx + 1)->Trim();

						if (key == L"server" || key == L"host") server = val;
						else if (key == L"port") port = val;
						else if (key == L"database" || key == L"db") database = val;
						else if (key == L"uid" || key == L"username" || key == L"user") uid = val;
						else if (key == L"password" || key == L"pwd") password = val;
					}
				}
			}

			connectionString = String::Format(
				L"Server={0};Port={1};Database={2};Uid={3};Pwd={4};Convert Zero Datetime=True;Charset=utf8mb4;",
				server, port, database, uid, password
			);
		}

	public:
		// Create and return an open MySQL connection
		static MySqlConnection^ CreateConnection()
		{
			InitializeConnectionString();
			MySqlConnection^ conn = gcnew MySqlConnection(connectionString);
			conn->Open();
			return conn;
		}

		// Test connection and retrieve database metadata safely
		static bool TestConnection(String^% outDetails, String^% outError)
		{
			outDetails = L"";
			outError = L"";
			try
			{
				MySqlConnection^ conn = CreateConnection();
				try
				{
					MySqlCommand^ cmd = gcnew MySqlCommand(L"SELECT DATABASE(), VERSION(), COUNT(*) FROM employees;", conn);
					MySqlDataReader^ reader = cmd->ExecuteReader();
					if (reader->Read())
					{
						String^ dbName = reader->IsDBNull(0) ? L"Unknown" : reader->GetString(0);
						String^ version = reader->IsDBNull(1) ? L"Unknown" : reader->GetString(1);
						int empCount = reader->GetInt32(2);

						outDetails = String::Format(
							L"Connected successfully!\nDatabase: {0}\nMySQL Version: {1}\nActive Employees: {2}",
							dbName, version, empCount
						);
						return true;
					}
					reader->Close();
				}
				finally
				{
					conn->Close();
				}
			}
			catch (Exception^ ex)
			{
				outError = ex->Message;
				return false;
			}
			return false;
		}

		// Secure SHA-256 password hash matching MySQL SHA2(password, 256)
		static String^ HashPassword(String^ plainPassword)
		{
			if (String::IsNullOrEmpty(plainPassword))
				return L"";

			SHA256^ sha = SHA256::Create();
			array<Byte>^ bytes = Encoding::UTF8->GetBytes(plainPassword);
			array<Byte>^ hashBytes = sha->ComputeHash(bytes);

			StringBuilder^ sb = gcnew StringBuilder();
			for (int i = 0; i < hashBytes->Length; i++)
			{
				sb->Append(hashBytes[i].ToString(L"x2"));
			}
			return sb->ToString();
		}
	};
}
