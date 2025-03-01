#include "../include/system.h"

mongocxx::instance instance{};
mongocxx::client client(mongocxx::uri{});
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

auto db = client["DatabaseForHouseManagement"];
auto acc_collection = db["accountCollection"];
auto house_collection = db["houseCollection"];

bool auth(std::string username, std::string password)

{
    auto cursor = acc_collection.find({});

    for (const auto &doc : cursor)
    {
        if (username.compare(doc["userName"].get_string().value.data()) == 0 && password.compare(doc["password"].get_string().value.data()) == 0)
        {

            return true;
        }
    }

    return false;
}
bool verify(const std::string &input)
{

    for (char c : input)
    {
        if (!std::isalnum(static_cast<unsigned char>(c)))
        {
            printf("Please follow the provided instruction");
            return false;
        }
    }
    return true;
}

void clear()
{
    printf("\e[1;1H\e[2J"); // clear
}
void welcome()
{
    clear();
    printf("SYSTEM INITIALIZED, WELCOME\n");
    printf("VACATION HOUSE EXCHANGE APPLICATION\n");
    printf("-----------------------------------------\n");
    printf("Developed by: Huynh Tan Phat\n");
    printf("-----------------------------------------\n");
    printf("Option: 0. Stop the program 1. Enter as Guest  2. Enter as Member 3. Enter as Admin\n");
}
void init()
{
    try
    {
        acc_collection.drop();
        house_collection.drop();

        // Create and insert Account objects
        Account account_three("user123", "John Doe", "123-456-7890", "password", "Member");
        Account account_one("user1234", "Jane Doe", "123-456-7891", "password", "Guest");
        Account account_two("user1235", "Jim Doe", "123-456-7892", "password", "Admin");
        Account account_four("user1236", "Janet Doe", "123-456-7893", "password", "Member");
        Account admin_account("admin123", "adminstrator", "Admin");

        // Put everything in the list and add them in
        std::vector<Account> acc_list = {account_one, account_two, account_three, account_four, admin_account};

        for (const auto &account : acc_list)
        {
            auto bsonDoc = account.to_bson();
            acc_collection.insert_one(bsonDoc.view());
        }
        /////////////////////////
        // PROPER WAY TO GET THE ID
        auto query = bsoncxx::builder::basic::make_document(
            bsoncxx::builder::basic::kvp("userName", "user123"));

        auto acc_three = acc_collection.find_one(query.view());

        auto doc_three = acc_three->view();
        bsoncxx::oid existing_id = doc_three["_id"].get_oid().value;

        House house_one("Ha Noi", "house one", true, existing_id);
        ///////////////////////////////////////////////////////

        House house_two("Ho Chi Minh city", "house two", false);
        std::vector<House> house_list = {house_one, house_two};

        /*
            Copying can be expensive, depends on object's scale.
            Thus raise the need for iterating through it by using a reference (&),
            where each one points to the correspond original object in the container.


             The const keyword ensures that the objects being referenced are not modified during the iteration.
             This enforces read-only access to the objects, which can help prevent accidental modifications and improve code safety.*/
        for (const auto &house : house_list)
        {
            auto bsonDoc = house.to_bson();
            house_collection.insert_one(bsonDoc.view());
        }

        // // Ping the database.
        // const auto ping_cmd = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("ping", 1));
        // db.run_command(ping_cmd.view());
        // std::cout << "Pinged your deployment. You successfully connected to MongoDB!" << std::endl;
    }
    catch (const std::exception &e)
    {
        printf("Exception: ");
        printf("%s", e.what());
        printf("\n");
    }

    welcome();
}

std::vector<House> getAllHouses()
{
    auto cursor = house_collection.find({});
    std::vector<House> houses;

    for (const auto &doc : cursor)
    {

        House temp; // temporary object

        // extract the elements
        std::string location = std::string(doc["location"].get_string().value.data());
        std::string description = std::string(doc["description"].get_string().value.data());
        bool isAvailable = doc["available"].get_bool().value;

        temp.setLocation(location);
        temp.setDescription(description);
        temp.setAvailability(isAvailable);

        if (doc["owner"])
        {

            bsoncxx::oid owner = doc["owner"].get_oid().value;
            temp.setOwner(owner);
        }
        houses.push_back(temp);
    }

    return houses;
}

std::vector<Account> getAllUsers()
{
    auto cursor = acc_collection.find({});
    std::vector<Account> accounts;

    for (const auto &doc : cursor)
    {
        if (doc["role"] && std::string(doc["role"].get_string().value.data()).compare("Admin") == 0)
        {
            continue;
        }
        else
        {
            if (doc["userName"] && doc["fullName"] && doc["phoneNumber"])
            {
                Account temp; // temporary object

                // extract the elements
                std::string userName = doc["userName"].get_string().value.data();
                std::string fullName = doc["fullName"].get_string().value.data();

                std::string phoneNumber = doc["phoneNumber"].get_string().value.data();

                temp.setUserName(userName);
                temp.setFullName(fullName);
                temp.setPhoneNumber(phoneNumber);

                accounts.push_back(temp);
            }
        }
    }

    return accounts;
}

Account getHouseOwner(const bsoncxx::oid &owner)
{
    auto query = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("_id", owner));

    auto cursor = acc_collection.find_one(query.view());
    /*
    std::string userName;
    std::string fullName;
    std::string phoneNumber;
*/
    if (cursor) // Check for Valid Document
    {
        auto doc = *cursor;
        std::string userName = doc["userName"].get_string().value.data();
        std::string fullName = doc["fullName"].get_string().value.data();
        std::string phoneNumber = doc["phoneNumber"].get_string().value.data();
        Account temp(userName, fullName, phoneNumber, "Member");
        return temp;
    }
    return Account();
}

Account getCurrentSession(const std::string &username, const std::string &password)
{
    auto query = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("userName", username),
        bsoncxx::builder::basic::kvp("password", password));
    auto cursor = acc_collection.find_one(query.view());

    if (cursor) // Check for Valid Document
    {
        auto doc = cursor->view();
        std::string role = doc["role"].get_string().value.data();

        if (role.compare("Member") == 0)
        {
            std::string fullName = doc["fullName"].get_string().value.data();
            std::string phoneNumber = doc["phoneNumber"].get_string().value.data();
            Account temp(username, fullName, phoneNumber, role);
            return temp;
        }

        else if (role.compare("Admin") == 0)
        {
            Account temp(username, password, role);
            return temp;
        }
    }

    throw std::runtime_error("User not found");
}
void addAccount(Account newUser)
{
    auto bsonDoc = newUser.to_bson();
    acc_collection.insert_one(bsonDoc.view());
}

//----GENERAL---//
void viewInformation(Account currentUser)
{
    currentUser.toString();
}
void viewHouses_NonGuest()
{
    std::cout << "List of all houses: \n";
    std::vector<House> houses = getAllHouses();

    for (House &house : houses)
    {
        Account houseOwner(getHouseOwner(house.getOwner()));
        std::cout << "---House information---" << "\n";
        std::cout << "Location: " << house.getLocation() << "\n";
        std::cout << "Description: " << house.getDescription() << "\n";
        std::cout << "Available: " << ((house.getAvailability()) ? "yes" : "no") << "\n";
        std::cout << "---Owner's contact information---" << "\n";
        houseOwner.toString();
        std::cout << "\n";
    }
}
//-----ADMIN-----//
void viewUsers()
{
    printf("List of all members: ");
    std::vector<Account> accounts = getAllUsers();

    for (Account &account : accounts)
    {

        account.toString();
        std::cout << "\n";
    }
}
void process_execute_admin(int choice, const Account &currentUser)
{

    switch (choice)
    {
    case 0:
        clear();
        printf("This is your menu: \n");
        printf("-1. Log out\n");
        printf("0. Clear screen\n");
        printf("1. View information\n");
        printf("2. View houses\n");
        printf("3. View member\n");

        printf("...................\n");
        break;

    case 1:
        std::cout << "---YOUR INFORMATION---" << "\n";
        viewInformation(currentUser);
        break;

    case 2:
        viewHouses_NonGuest();
        break;

    case 3:
        viewUsers();
        break;

    default:
        printf("Please enter a valid option\n");
    }
}
void instruction_admin(const Account &currentUser)
{
    printf("This is your menu: \n");
    printf("-1. Log out\n");
    printf("0. Clear screen\n");
    printf("1. View information\n");
    printf("2. View houses\n");
    printf("3. View member\n");

    printf("...................\n");

    int choice;
    while (1)
    {
        printf("Enter your choice: ");

        std::cin >> choice;
        if (choice == -1)
        {
            clear();
            welcome();
            break;
        }
        else
        {
            process_execute_admin(choice, currentUser);
        }
    }
}

//-----GUEST----//
void registration()
{

    std::string userName, fullName, phoneNumber, password;

    while (verify(fullName))
    {
        printf("Enter your full name (no special character)");
        std::cin >> userName;
    }
    while (verify(phoneNumber))
    {
        printf("Enter your phone number (no special character)");
        std::cin >> userName;
    }

    while (verify(userName))
    {
        printf("Enter an username (no special character)");
        std::cin >> userName;
    }

    while (verify(password))
    {
        printf("Enter a password (no special character)");
        std::cin >> password;
    }

    Account newUser(userName, fullName, phoneNumber, password);
    addAccount(newUser);
}
void viewHouses_Guest()
{
    std::cout << "List of all houses\n";
    std::vector<House> houses = getAllHouses();

    for (House &house : houses)
    {
        house.toString_Guest();
    }
}
void process_execute_guest(int choice)
{

    switch (choice)
    {
    case 0:
        clear();
        printf("This is your menu: \n");
        printf("-1. Log out\n");
        printf("0. clear screen\n");
        // printf("1. View Information\n");
        printf("1. Register as a member\n");
        printf("2. View house\n");
        printf("...................\n");

        break;

    case 1:
        registration();
        break;

    case 2:
        viewHouses_Guest();
        break;

    default:
        printf("Please enter a valid option");
    }
}
void instruction_guest()
{
    std::cout << "Welcome, Dear Guest " << "\n";
    printf("This is your menu: \n");
    printf("-1. Log out\n");
    printf("0. clear screen\n");
    // printf("1. View Information\n");
    printf("1. Register as a member\n");
    printf("2. View house\n");
    printf("...................\n");

    int choice;
    while (true)
    {
        printf("Enter your choice:");

        std::cin >> choice;
        if (choice == -1)
        {
            clear();
            welcome();
            break;
        }
        else
        {
            process_execute_guest(choice);
        }
    }
}

//-----MEMBER--//
void occupy(const bsoncxx::oid &houseID, const bsoncxx::oid &renterID)
{

    // define the house's query
    auto query = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("_id", houseID));

    // Define the updated document (update renter's ID)
    auto update = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("$set",
                                     bsoncxx::builder::basic::make_document(
                                         bsoncxx::builder::basic::kvp("renter", renterID))));

    house_collection.find_one_and_update(query.view(),update.view());
}
void process_execute_member(int choice, const Account &currentUser)
{
    switch (choice)
    {
    case 0:
        clear();
        printf("This is your menu: \n");
        printf("-1. Log out\n");
        printf("0. Clear screen\n");
        printf("1. View information\n");
        printf("2. View houses\n");
        printf("3. View notification\n");
        printf("4. Occupation listing/unlisting \n");

        printf("...................\n");
        break;

    case 1:
        std::cout << "---YOUR INFORMATION---" << "\n";
        viewInformation(currentUser);
        break;

    case 2:
        viewHouses_NonGuest();
        break;

    default:
        printf("Please enter a valid option");
    }
}
void instruction_member(const Account &currentUser)
{

    printf("This is your menu: \n");
    printf("-1. Log out\n");
    printf("0. Clear screen\n");
    printf("1. View information\n");
    printf("2. View houses\n");
    printf("3. View notification\n");
    printf("4. Occupation listing/unlisting \n");

    printf("...................\n");

    int choice;
    while (1)
    {
        printf("Enter your choice:");
        std::cin >> choice;
        if (choice == -1)
        {
            clear();
            welcome();
            break;
        }
        else
        {
            process_execute_member(choice, currentUser);
        }
    }
}

//----EXECUTE--//
void systemRun()
{
    while (1)
    {
        int option;
        printf("Enter your choice: ");
        std::cin >> option;
        ////////////////////////////////
        if (option > 1)
        {
            std::string username;
            std::string password;
            while (true)
            {

                printf("Enter username: ");
                std::cin >> username;

                printf("Enter password: ");
                std::cin >> password;

                if (auth(username, password) == false)
                {
                    printf("Authorization failed, please try again\n\n");
                }
                else
                {
                    break;
                }
            }

            //////////////////////////////
            // Tuple input(role, username);
            printf("..................\n");

            Account temp = getCurrentSession(username, password);
            clear();
            printf("Authorization successfully\n\n");
            std::cout << "Welcome, " << username << "\n";

            if (temp.getRole().compare("Member") == 0)
            {

                instruction_member(temp);
            }
            else
            {
                instruction_admin(temp);
            }
        }
        else if (option == 1)
        {
            clear();
            instruction_guest();
        }
        else
        {
            printf("See you again soon\n");
            break;
        }
    }
}
