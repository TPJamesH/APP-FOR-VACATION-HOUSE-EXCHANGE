#ifndef HOUSE_H
#define HOUSE_H

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/oid.hpp>
#include <bsoncxx/types.hpp>
#include <string>

class House
{
private:
    bsoncxx::oid id;
    std::string location;
    std::string description;
    bool isAvailable;
    bsoncxx::oid ownerID;
    bsoncxx::oid renterID;

public:
    House();

    House(const std::string &location, 
        const std::string &description, 
        const bool &isAvailable, 
        const bsoncxx::oid &ownerID);

    House(const std::string &location,
         const std::string &description, 
         const bool &isAvailable);

    House(const House &input);

    //Functions
    bsoncxx::oid get_id() const;
    
void setID(const bsoncxx::oid &id);

    std::string getLocation() const;
    void setLocation(const std::string &location);

    std::string getDescription() const;
    void setDescription(const std::string &description);

    bool getAvailability() const;
    void setAvailability(const bool &status);

    bsoncxx::oid getOwner() const;
    void setOwner(const bsoncxx::oid &newOwner);

    bsoncxx::oid getRenter() const;
    void setRenter(const bsoncxx::oid &newRenter);
    
    void toString_Guest();
    bsoncxx::document::value to_bson() const;
};

#endif