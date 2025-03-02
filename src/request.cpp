#include "../include/request.h"
#include <iostream>

Request::Request() {}

Request::Request(const std::string &msg,
                 const bsoncxx::oid &requester,
                 const bsoncxx::oid &house,
                 const bool &approval) : id(bsoncxx::oid()), msg(msg),
                                         requester(requester),
                                         house(house),
                                         approval(approval) {}

Request::Request(const Request &input) : id(input.id), msg(input.msg),
                                         requester(input.requester),
                                         house(input.house),
                                         approval(input.approval) {}

bsoncxx::oid Request::get_id() const {
    return id;
}

std::string Request::getMessage() const{
    return msg;
}

void Request::setMessage(const std::string &msg){
    this->msg = msg;
}

bsoncxx::oid Request::getRequester() const{
    return requester;
}

void Request::setRequester(const bsoncxx::oid &requester){
    this->requester = requester;
}

bsoncxx::oid Request::getHouse() const{
    return house;
}

void Request::setHouse(const bsoncxx::oid &house){
    this->house = house;
}    

bool Request::getApproval() const{
    return approval;
}

void Request::setApproval(const bool & approval){
    this->approval = approval;
}