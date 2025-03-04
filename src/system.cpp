#include "../include/system.h"

mongocxx::instance instance{};
mongocxx::client client(mongocxx::uri{});
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

auto db = client["DatabaseForHouseManagement"];
auto acc_collection = db["accountCollection"];
auto house_collection = db["houseCollection"];
auto request_collection = db["requestCollection"];

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
    printf("Option: -1. Stop the program 0. Clear screen 1. Enter as Guest  2. Enter as Member 3. Enter as Admin\n");
}
void init()
{
    try
    {
        acc_collection.drop();
        house_collection.drop();
        request_collection.drop();

        // Create and insert Account objects
        Account account_three("user123", "John Doe", "123-456-7890", "password", "Member");
        Account account_one("user1234", "Jane Doe", "123-456-7891", "password", "Member");
        Account account_two("user1235", "Jim Doe", "123-456-7892", "password", "Member");
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

//-----ADMIN-----//
void viewHouses_Admin()
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
        viewHouses_Admin();
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
void viewHouses_Member(const Account &currentUser)
{
    std::cout << "List of all houses  \n\n";
    std::vector<House> houses = getAllHouses();

    int choice = 0;
    std::string msg;
    for (House &house : houses)
    {

        Account houseOwner(getHouseOwner(house.getOwner()));

        std::cout << "Selection value: " << choice << "\n";
        std::cout << "---House information---" << "\n";
        std::cout << "Location: " << house.getLocation() << "\n";
        std::cout << "Description: " << house.getDescription() << "\n";
        std::cout << "Available: " << ((house.getAvailability()) ? "yes" : "no") << "\n";
        std::cout << "---Owner's contact information---" << "\n";
        houseOwner.toString();
        std::cout << "///////////////////////////" << "\n";
        choice++; // Lazy on this one
    }
    
    choice = 0;

    bool valid = false;
    while (valid == false)
    {
        std::cout << "Select a corresponding number for the house that you want to request for occupation: (enter -1 to return) ";
        std::cin >> choice;
        if (choice <= houses.size() - 1 && choice >= 0)
        {
            // Take message input WITH SPACE
            std::cout << "Enter a request message: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
            std::getline(std::cin, msg);

            ////////////////////////////////
            // QUERY TO GET THE ID OF THE HOUSE
            auto query_house = bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("location", houses.at(choice).getLocation()),
                bsoncxx::builder::basic::kvp("description", houses.at(choice).getDescription()),
                bsoncxx::builder::basic::kvp("available", houses.at(choice).getAvailability()));

            // GET THE ID OF THE HOUSE
            auto house_ob = house_collection.find_one(query_house.view());
            auto doc_house = house_ob->view();
            bsoncxx::oid house_id = doc_house["_id"].get_oid().value;
            /////////////////////////////////////
            // QUERY TO GET THE ID OF THE RENTER
            auto query_renter = bsoncxx::builder::basic::make_document(
                bsoncxx::builder::basic::kvp("userName", currentUser.getUserName()),
                bsoncxx::builder::basic::kvp("fullName", currentUser.getFullName()),
                bsoncxx::builder::basic::kvp("phoneNumber", currentUser.getphoneNumber()));

            // GET THE ID OF THE RENTER
            auto renter_ob = acc_collection.find_one(query_renter.view());
            auto doc_renter = renter_ob->view();
            bsoncxx::oid renter_id = doc_renter["_id"].get_oid().value;

            //////////////////////////////
            // Create a new request object
            Request newRequest(msg, renter_id, house_id, false);

            // Insert to the collection
            auto bsonDoc = newRequest.to_bson();
            request_collection.insert_one(bsonDoc.view());

            // Let user knows
            std::cout << "Request sent, please wait for the owner's response" << "\n";
            valid = true;
        }
        else if(choice == -1){
            break;
        }
        else
        {
            printf("Please enter a valid option\n");
        }
    }

    std::cout << "Back to member's menu\n";
}
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

    house_collection.find_one_and_update(query.view(), update.view());
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
        viewHouses_Member(currentUser);
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
void viewNotification(const Account &currentUser){

}

void getOwnHouse(const Account &currentUser){
    //Debug section
    std::cout << "Debug statement" <<currentUser.get_id().to_string() << "\n";
    ///////////////////////////////
    std::cout << "Here is a list of houses that you current owned\n";

    // QUERY TO GET HOUSES
    auto query_house = bsoncxx::builder::basic::make_document(
        bsoncxx::builder::basic::kvp("owner", currentUser.get_id())
    );
    //Get house(s)
    auto list_house = house_collection.find(query_house.view());

    std::vector<House> myHouses;

    for (const auto &doc: list_house){
        House temp;

        //extract the elements
        std::string location = std::string(doc["location"].get_string().value.data());
        std::string description = std::string(doc["description"].get_string().value.data());
        bool isAvailable = doc["available"].get_bool().value;

        temp.setLocation(location);
        temp.setDescription(description);
        temp.setAvailability(isAvailable);

        myHouses.push_back(temp);
    }
}

std::vector<Request> getMyRequest(const House &myHouse){

}
//----EXECUTE--//
void systemRun()
{
    welcome();
    int option;
    while (1)
    {

        printf("Enter your choice: ");
        std::cin >> option;
        ////////////////////////////////
        // IN CASE OF BAD INPUT (E.G., INPUT CHAR INSTEAD OF INT)
        if (!std::cin)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            printf("Please enter a valid option\n");
        }
        else if (option > 1 && option < 4)
        {
            std::string username;
            std::string password;
            while (true)
            {

                printf("Enter username (press exit to return): ");
                std::cin >> username;
                if (username.compare("exit") == 0)
                {
                    break;
                }
                else // if the user's input is not exit, proceed with the following:
                {
                    printf("Enter password: ");
                    std::cin >> password;

                    if (auth(username, password) == false)
                    {
                        printf("Authorization failed, please try again\n\n");
                    }
                    else // If authorization successfully
                    {

                        //////////////////////////////
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
                        break;
                    }
                }
            }
        }
        else if (option == 1)
        {
            clear();
            instruction_guest();
        }
        else if (option == 0)
        {
            clear();
            welcome();
        }
        else if (option == -1)
        {
            printf("See you again soon\n");
            break;
        }
        else
        {
            printf("Please enter a valid option\n");
        }
    }
}
