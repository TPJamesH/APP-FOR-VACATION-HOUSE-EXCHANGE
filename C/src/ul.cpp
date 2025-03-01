#include <string>
class Tuple
{
private:
    int integer;
    std::string str;

public:
    Tuple(){}
    Tuple(int integer, std::string str)
    {
        this->integer = integer;
        this->str = str;
    }

    int getInteger()
    {
        return this->integer;
    }
    void setInteger(int integer)
    {
        this->integer = integer;
    }

    std::string getString()
    {
        return this->str;
    }
    void setString(std::string str)
    {
        this->str = str;
    }
};



