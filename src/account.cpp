#include "../include/account.h"
#include <iostream>

// Default constructor
Account::Account() : id(bsoncxx::oid()) {}
// Parameterized constructor
Account::Account(const std::string &userName, const std::string &fullName, const std::string &phoneNumber, const std::string &password, const std::string &role)
    : id(bsoncxx::oid()),
      userName(userName),
      fullName(fullName),
      phoneNumber(phoneNumber),
      password(password),
      role(role) {}

// Constructor without password
Account::Account(const std::string &userName, const std::string &fullName, const std::string &phoneNumber, const std::string &role)
    : id(bsoncxx::oid()),
      userName(userName),
      fullName(fullName),
      phoneNumber(phoneNumber),
      role(role) {}

// Constructor with only username and role
Account::Account(const std::string &userName, const std::string &password, const std::string &role)
    : id(bsoncxx::oid()),
      userName(userName),
      password(password),
      role(role) {}

// Copy constructor
Account::Account(const Account &input)
    : id(input.get_id()),
      userName(input.getUserName()),
      fullName(input.getFullName()),
      phoneNumber(input.getphoneNumber()),
      password(input.getPassword()),
      role(input.getRole()) {}

bsoncxx::oid Account::get_id() const
{
    return id;
}

void Account::setID(const bsoncxx::oid &id){
    this->id =id ;
}
std::string Account::getUserName() const
{
    return userName;
}
void Account::setUserName(const std::string &username)
{
    userName = username;
}

std::string Account::getFullName() const
{
    return fullName;
}

void Account::setFullName(const std::string &fullname)
{
    fullName = fullname;
}

std::string Account::getphoneNumber() const
{
    return phoneNumber;
}

void Account::setPhoneNumber(const std::string &PhoneNumber)
{
    phoneNumber = PhoneNumber;
}

std::string Account::getPassword() const
{
    return password;
}

void Account::setPassword(const std::string &Password)
{
    password = Password;
}

std::string Account::getRole() const
{
    return role;
}

void Account::setRole(const std::string &Role)
{
    role = Role;
}

void Account::toString()
{
    std::string none = "none";
    std::cout << "Username: " << std::string(userName.empty() == false ? userName : none) << "\n";
    std::cout << "Full Name: " << std::string(fullName.empty() == false ? fullName : none) << "\n";
    std::cout << "Phone Number: " << std::string(phoneNumber.empty() == false ? phoneNumber : none) << "\n";
}

bsoncxx::document::value Account::to_bson() const
{
    bsoncxx::builder::basic::document doc{};
    doc.append(bsoncxx::builder::basic::kvp("_id", id));
    doc.append(bsoncxx::builder::basic::kvp("userName", userName));
    doc.append(bsoncxx::builder::basic::kvp("fullName", fullName));
    doc.append(bsoncxx::builder::basic::kvp("phoneNumber", phoneNumber));
    doc.append(bsoncxx::builder::basic::kvp("password", password));
    doc.append(bsoncxx::builder::basic::kvp("role", role));
    return doc.extract();
}
