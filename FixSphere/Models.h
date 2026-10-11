#pragma once

using namespace System;

namespace FixSphere {

	// ==============================================================================
	// 1. CLASS & ABSTRACTION & ENCAPSULATION
	// Person is an abstract base class representing any human entity in FixSphere.
	// Demonstrates:
	//   - Abstraction: Declares abstract method GetRoleDescription() with no body.
	//   - Encapsulation: Private backing fields accessed via controlled properties.
	// ==============================================================================
	public ref class Person abstract
	{
	private:
		int id;
		String^ name;
		String^ contact;
		String^ nic;

	public:
		// Constructor
		Person(int id, String^ name, String^ contact, String^ nic)
		{
			this->id = id;
			this->name = name;
			this->contact = contact;
			this->nic = nic;
		}

		// Encapsulated properties (Getters & Setters)
		property int Id
		{
			int get() { return id; }
			void set(int value) { id = value; }
		}

		property String^ Name
		{
			String^ get() { return name; }
			void set(String^ value) { name = value; }
		}

		property String^ Contact
		{
			String^ get() { return contact; }
			void set(String^ value) { contact = value; }
		}

		property String^ Nic
		{
			String^ get() { return nic; }
			void set(String^ value) { nic = value; }
		}

		// Pure abstract method enforcing implementation in derived classes (Abstraction)
		virtual String^ GetRoleDescription() abstract;

		// Virtual method demonstrating base display behavior
		virtual String^ GetSummary()
		{
			return String::Format(L"ID: {0} | Name: {1} | Contact: {2}", id, name, contact);
		}
	};

	// ==============================================================================
	// 2. INHERITANCE & POLYMORPHISM (Derived Class 1: Customer)
	// Customer inherits all fields and methods from Person.
	// Demonstrates:
	//   - Inheritance: ": public Person"
	//   - Polymorphism: Overrides GetRoleDescription() and GetSummary().
	// ==============================================================================
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

		property String^ Address
		{
			String^ get() { return address; }
			void set(String^ value) { address = value; }
		}

		// Polymorphic method override
		virtual String^ GetRoleDescription() override
		{
			return L"Client / Device Owner requiring phone hardware or software servicing.";
		}

		virtual String^ GetSummary() override
		{
			return String::Format(L"{0} | Address: {1}", Person::GetSummary(), address);
		}
	};

	// ==============================================================================
	// 3. INHERITANCE & POLYMORPHISM (Derived Class 2: Employee)
	// Employee inherits all fields and methods from Person.
	// Demonstrates:
	//   - Inheritance: ": public Person"
	//   - Polymorphism: Provides its own specific override of GetRoleDescription().
	// ==============================================================================
	public ref class Employee : public Person
	{
	private:
		String^ role;
		String^ username;
		bool isActive;

	public:
		Employee(int id, String^ name, String^ contact, String^ nic, String^ role, String^ username, bool isActive)
			: Person(id, name, contact, nic)
		{
			this->role = role;
			this->username = username;
			this->isActive = isActive;
		}

		property String^ Role
		{
			String^ get() { return role; }
			void set(String^ value) { role = value; }
		}

		property String^ Username
		{
			String^ get() { return username; }
			void set(String^ value) { username = value; }
		}

		property bool IsActive
		{
			bool get() { return isActive; }
			void set(bool value) { isActive = value; }
		}

		// Polymorphic method override
		virtual String^ GetRoleDescription() override
		{
			return String::Format(L"Internal Staff Member operating under role: {0}", role);
		}

		virtual String^ GetSummary() override
		{
			return String::Format(L"{0} | Role: {1} | Username: {2}", Person::GetSummary(), role, username);
		}
	};

}
