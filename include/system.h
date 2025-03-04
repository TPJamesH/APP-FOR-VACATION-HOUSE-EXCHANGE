#ifndef SYSTEM_H
#define SYSTEM_H

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/types.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <stdexcept>
#include "account.h"
#include "house.h"
#include "request.h"
#include <vector>
#include <iostream>

//Functions
bool auth(std::string username, std::string password);
bool verify(const std::string &input);
void welcome();
void init();
std::vector<House> getAllHouses();
std::vector<Account> getAllUsers();
Account getHouseOwner(const bsoncxx::oid &owner);
Account getCurrentSession(const std::string &username, const std::string &password);
void addAccount(Account newUser);

//---GENERAL---//
void viewInformation(Account currentUser);


//-----ADMIN-----//
void viewHouses_Admin();
void viewUsers();
void process_execute_admin(int choice,const Account &currentUser);
void instruction_admin(const Account &currentUser);

//---GUEST---//
void registration();
void viewHouses_Guest();
void process_execute_guest(int choice);
void instruction_guest();

//--MEMBER--//
void viewHouses_Member( const Account &currentUser);
void occupy(const bsoncxx::oid &houseID, const bsoncxx::oid &renterID);
void process_execute_member(int choice,const Account &currentUser);
void instruction_member(const Account &currentUser);
void viewNotification(const Account &currentUser);
std::vector<House> getOwnHouse(const Account &currentUser);
std::vector<Request> getMyRequest(const House &myHouse);
//--EXECUTE--//
void systemRun();

void clear();

#endif