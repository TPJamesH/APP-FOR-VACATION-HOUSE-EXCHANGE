#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/oid.hpp>
#include <string>
class Account
{
private:
    bsoncxx::oid id;
    std::string userName;
    std::string fullName;
    std::string phoneNumber;
    std::string password;
    std::string role;

public:
    // default constructor
    Account();
    // parameterized constructor
    Account(
        const std::string &userName,
        const std::string &fullName,
        const std::string &phoneNumber,
        const std::string &password,
        const std::string &role);

    Account(
        const std::string &userName,
        const std::string &fullName,
        const std::string &phoneNumber,
        const std::string &role);

    Account(
        const std::string &userName,
        const std::string &password,
        const std::string &role);

    // copy constructor
    Account(const Account &input);

    //Functions
    bsoncxx::oid get_id() const;

    std::string getUserName() const;
    void setUserName(const std::string &userName);

    std::string getFullName() const;
    void setFullName(const std::string &fullName);

    std::string getphoneNumber() const;
    void setPhoneNumber(const std::string &phoneNumber);

    std::string getPassword() const;
    void setPassword(const std::string &password);

    std::string getRole() const;
    void setRole(const std::string &role);
    
    void toString();
    bsoncxx::document::value to_bson() const;
};

#endif