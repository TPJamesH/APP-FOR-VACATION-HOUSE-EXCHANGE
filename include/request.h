#ifndef REQUEST_H
#define REQUEST_H

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/oid.hpp>
#include <bsoncxx/types.hpp>
#include <string>

class Request
{
private:
    bsoncxx::oid id;
    std::string msg;
    bsoncxx::oid requester;
    bsoncxx::oid house;
    bool approval;

public:
    //Constructor
    Request();

    Request(const std::string &msg,
        const bsoncxx::oid &requester,
        const bsoncxx::oid &house,
        const bool &approval
    );

    Request(const Request &input);

    //Functions
    bsoncxx::oid get_id() const;

    std::string getMessage() const;
    void setMessage(const std::string &msg);

    bsoncxx::oid getRequester() const;
    void setRequester(const bsoncxx::oid &requester);

    bsoncxx::oid getHouse() const;
    void setHouse(const bsoncxx::oid &house);
    
    bool getApproval() const;
    void setApproval(const bool &approval);

};
#endif