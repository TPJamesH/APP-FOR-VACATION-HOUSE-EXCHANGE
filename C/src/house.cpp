#include "../include/house.h"
#include <iostream>
/*
Key Differences
Initializer Lists: Ensure direct initialization, maintain the declared order, and are generally more efficient for complex types and const members.

Assignment in Constructor Body: May lead to double initialization (default then assignment) and potential misordering. (e.g., location = location)
*/
House::House()  : id(bsoncxx::oid()) {}

House::House(const std::string &location, const std::string &description, const bool &isAvailable, const bsoncxx::oid &ownerID)
    : id(bsoncxx::oid()), location(location), description(description), isAvailable(isAvailable), ownerID(ownerID) {}

House::House(const std::string &location, const std::string &description, const bool &isAvailable)
    : id(bsoncxx::oid()), location(location), description(description), isAvailable(isAvailable) {}

House::House(const House &input)
    : id(input.id), location(input.location), description(input.description), isAvailable(input.isAvailable), ownerID(input.ownerID) {}

bsoncxx::oid House::get_id() const
{
    return id;
}
std::string House::getLocation() const
{
    return location;
}

void House::setLocation(const std::string &loc)
{
    location = loc;
}

std::string House::getDescription() const
{
    return description;
}

void House::setDescription(const std::string &desc)
{
    description = desc;
}

bool House::getAvailability() const
{
    return isAvailable;
}

void House::setAvailability(const bool &status)
{
    isAvailable = status;
}

bsoncxx::oid House::getOwner() const
{
    return ownerID;
}

void House::setOwner(const bsoncxx::oid &newOwner)
{
    ownerID = newOwner;
}

bsoncxx::oid House::getRenter() const
{
    return ownerID;
}

void House::setRenter(const bsoncxx::oid &newRenter)
{
    renterID = newRenter;
}

void House::toString_Guest()
{
    std::cout << "---House information---" << "\n";
    std::cout << "Location: " << location << "\n";
    std::cout << "Description: " << description << "\n";
    std::cout << "\n";
}

bsoncxx::document::value House::to_bson() const
{
    bsoncxx::builder::basic::document doc{};
    doc.append(bsoncxx::builder::basic::kvp("_id", id));
    doc.append(bsoncxx::builder::basic::kvp("location", location));
    doc.append(bsoncxx::builder::basic::kvp("description", description));
    doc.append(bsoncxx::builder::basic::kvp("available", isAvailable));
    doc.append(bsoncxx::builder::basic::kvp("owner", ownerID));
    doc.append(bsoncxx::builder::basic::kvp("renter",renterID));
    
    return doc.extract();
}
