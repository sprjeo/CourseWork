#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <chrono>
#include <ctime>

struct Message
{
    std::string userName;
    std::string time;
    std::string message;

    bool operator<(const Message& other) const
    {
        if (userName == other.userName)
        {
            return time < other.time;
        }
        return userName < other.userName;
    }
};

std::time_t parse_time(const std::string& time)
{
    std::tm timeinfo = {};
    std::istringstream iss(time);
    iss >> std::get_time(&timeinfo, "%d:%m:%Y:%H:%M:%S");

    if (iss.fail())
    {
        throw std::invalid_argument("Failed to parse time string.");
    }
    else
    {
        std::time_t time_time_t = std::mktime(&timeinfo);
        return time_time_t;
    }
}


void readDataFromFile(const std::string& filename, std::set<Message>& messages)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::ifstream::failure("File couldn't be opened");
    }
    else
    {
        std::string line;
        while (std::getline(file, line))
        {
            Message msg;
            char dotdot;
            std::istringstream iss(line);
            iss >> msg.userName >> msg.time >> dotdot;
            std::getline(iss, msg.message);
            msg.userName = msg.userName.substr(1, msg.userName.length() - 2);
            msg.time = msg.time.substr(1, msg.time.length() - 3);
            msg.message = msg.message.substr(0, msg.message.length() - 1);

            messages.insert(msg);
        }
        file.close();
    }
}

class WorkWithMessages
{
public:
    static void printAllMessagesFromPerson(const std::string& fullName, std::set<Message>& messages)
    {

        if (fullName.empty() || messages.empty())
        {
            throw std::invalid_argument("invalid argument detected in printAllMessagesFromPerson() functon!");
        }
        else
        {
            for (const auto& msg : messages)
            {
                if (msg.userName == fullName)
                {
                    std::cout << msg.userName << " " << msg.time << " " << msg.message << std::endl;
                }
            }
            std::cout << std::endl;
        }

    }

    static void printAllMessagesFromPerson_InTimeRange(const std::string& fullName, std::string startTime, std::string endTime, std::set<Message>& messages)
    {
        if (fullName.empty() || messages.empty() || startTime.empty() || endTime.empty())
        {
            throw std::invalid_argument("invalid argument detected in printAllMessagesFromPerson_InTimeRange() functon!");
        }
        else
        {
            for (const auto& msg : messages)
            {
                std::time_t start_time_t, end_time_t, mes_time_t;
                try
                {
                    if (msg.userName == fullName)
                    {
                        start_time_t = parse_time(startTime);
                        end_time_t = parse_time(endTime);
                        mes_time_t = parse_time(msg.time);

                        if (mes_time_t >= start_time_t && mes_time_t <= end_time_t)
                        {
                            std::cout << msg.userName << " " << msg.time << " : " << msg.message << std::endl;
                        }
                    }
                }
                catch (const std::invalid_argument& e)
                {
                    throw;
                }
            }
            std::cout << std::endl;
        }
    }

    static void printAllMessages_InTimeRange(std::string startTime, std::string endTime, std::set<Message>& messages)
    {
        if (messages.empty() || startTime.empty() || endTime.empty())
        {
            throw std::invalid_argument("invalid argument detected in printAllMessages_InTimeRange() functon!");
        }
        else
        {
            for (const auto& msg : messages)
            {
                std::time_t start_time_t, end_time_t, mes_time_t;
                try
                {
                    start_time_t = parse_time(startTime);
                    end_time_t = parse_time(endTime);
                    mes_time_t = parse_time(msg.time);

                    if (mes_time_t >= start_time_t && mes_time_t <= end_time_t)
                    {
                        std::cout << msg.userName << " " << msg.time << " : " << msg.message << std::endl;
                    }
                }
                catch (const std::invalid_argument& e)
                {
                    throw;
                }
            }
            std::cout << std::endl;
        }
    }

    static void delete_the_message(std::string text, std::set<Message>& messages)
    {
        if (messages.empty() || text.empty())
        {
            throw std::invalid_argument("invalid argument detected in delete_the_message() functon!");
        }
        else
        {
            for (const auto& msg : messages)
            {
                if (msg.message == text)
                {
                    messages.erase(msg);
                }
            }
        }
    }

    static void delete_messages_from_person(std::string fullName, std::set<Message>& messages)
    {
        if (messages.empty() || fullName.empty())
        {
            throw std::invalid_argument("invalid argument detected in delete_the_message() functon!");
        }
        else
        {
            for (auto i = messages.begin(); i != messages.end();)
            {
                if ((*i).userName == fullName)
                {
                    i = messages.erase(i);
                }
                else
                {
                    ++i;
                }
            }
        }
    }

    static void rewrite_the_file(const std::string& filename, std::set<Message>& messages)
    {
        std::string tempFilename = "temp.txt";
        std::ofstream tempFile(tempFilename);
        if (!tempFile.is_open())
        {
            throw std::ifstream::failure("file couldn't be opened.");
        }
        for (const auto& msg : messages)
        {
            tempFile << "<" << msg.userName << "> <" << msg.time << ">: <" << msg.message << ">" << std::endl;
        }

        tempFile.close();
        std::remove(filename.c_str());
        std::rename(tempFilename.c_str(), filename.c_str());
    }
};



int main()
{
    try
    {
        std::set<Message> messages;
        readDataFromFile("data12.txt", messages);

        std::cout << "printAllMessagesFromPerson(): " << std::endl;
        WorkWithMessages::printAllMessagesFromPerson("John", messages);

        std::cout << "printAllMessagesFromPerson_InTimeRange(): " << std::endl;
        WorkWithMessages::printAllMessagesFromPerson_InTimeRange("Alice", "09:10:2022:18:30:00:00", "16:08:2023:12:00:00:00", messages);

        std::cout << "printAllMessages_InTimeRange(): " << std::endl;
        WorkWithMessages::printAllMessages_InTimeRange("09:10:2022:18:30:00:00", "16:08:2023:12:00:00:00", messages);

        std::cout << "delete_the_message() was executed. " << std::endl;
        WorkWithMessages::delete_the_message("See you later!", messages);

        std::cout << "delete_messages_from_person() was executed. " << std::endl;
        WorkWithMessages::delete_messages_from_person("Alice", messages);

        std::cout << "rewrite_the_file() was executed. " << std::endl;
        WorkWithMessages::rewrite_the_file("data12.txt", messages);

    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}