#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <ctime>

class Contract
{
public:
    int number;
    std::string start_date;
    std::string end_date;
    int cost;
};

class Employee
{
public:
    std::string full_name;
    std::vector<Contract> contracts;

    void calculate_total_cost(const std::string& full_name, std::map<std::string, Employee>& employees)
    {
        if (full_name.empty() || employees.empty() || employees.find(full_name) == employees.end())
        {
            throw std::invalid_argument("invalid argument");
        }
        
        int totalCost = 0;
        for (const auto& contract : employees[full_name].contracts)
        {
            totalCost += contract.cost;
        }
        std::cout << "Total cost for " << full_name << ": " << totalCost << std::endl << std::endl;
        
    }

    void list_contracts(const std::string& full_name, std::map<std::string, Employee>& employees)
    {
        if (full_name.empty() || employees.empty() || employees.find(full_name) == employees.end())
        {
            throw std::invalid_argument("invalid argument");
        }
        
        std::cout << "Contracts for " << full_name << ":" << std::endl;
        for (const auto& contract : employees[full_name].contracts)
        {
            std::cout << "Contract " << contract.number << " - Cost: " << contract.cost << std::endl;
        }
        std::cout << std::endl;
        
    }

    void find_longest_contract(const std::string& full_name, std::map<std::string, Employee>& employees)
    {
        if (full_name.empty() || employees.empty() || employees.find(full_name) == employees.end())
        {
            throw std::invalid_argument("invalid argument");
        }
        
        int longest_contract_number = 0;
        std::time_t start_date;
        std::time_t end_date;
        std::time_t longest_holder = 0;
        for (const auto& contract : employees[full_name].contracts)
        {
            parse_date(start_date, contract.start_date);
            parse_date(end_date, contract.end_date);
            if (end_date - start_date > longest_holder)
            {
                longest_holder = end_date - start_date;
                longest_contract_number = contract.number;
            }
        }
        std::cout << "For " << full_name << " contract " << longest_contract_number << " is the longest contract." << std::endl << std::endl;  
    }

    void find_most_expensive_contract(const std::string& full_name, std::map<std::string, Employee>& employees)
    {
        if (full_name.empty() || employees.empty() || employees.find(full_name) == employees.end())
        {
            throw std::invalid_argument("invalid argument");
        }

        int most_expensive_contract_number = 0;
        int most_expensive_contract_cost = 0;
        for (const auto& contract : employees[full_name].contracts)
        {
            if (contract.cost > most_expensive_contract_cost)
            {
                most_expensive_contract_number = contract.number;
                most_expensive_contract_cost = contract.cost;
            }
        }

        std::cout << "For " << full_name << " contract " << most_expensive_contract_number << " is the most expensive. Cost: " << most_expensive_contract_cost << std::endl << std::endl;  
    }

private:
    void parse_date(std::time_t& date_to, const std::string& date_from)
    {
        int date_day, date_month, date_year;
        char dot;

        std::stringstream ss(date_from);
        ss >> date_day >> dot >> date_month >> dot >> date_year;

        std::tm timeinfo = {};
        timeinfo.tm_year = date_year - 1900;
        timeinfo.tm_mon = date_month - 1;
        timeinfo.tm_mday = date_day;

        date_to = std::mktime(&timeinfo);
    }
};

void read_data_from_file(std::string file_name, std::map<std::string, Employee>& employees)
{
    std::ifstream file(file_name);
    if (!file.is_open())
    {
        throw std::ifstream::failure("File couldn't be opened");
    }
    
    std::string line;
    std::string prev_line;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        if (line.find('{') != std::string::npos)
        {
            Employee emp;
            emp.full_name = prev_line;
            while (std::getline(file, line))
            {
                if (line.find('}') != std::string::npos)
                    break;

                Contract contract;
                std::string trash;
                std::istringstream iss(line);
                iss >> trash >> contract.number >> trash >> contract.start_date >> trash >> contract.end_date >> trash >> trash >> trash >> contract.cost;
                emp.contracts.push_back(contract);
            }
            employees[emp.full_name] = emp;
        }
        prev_line = line;
    }
    file.close();   
}

int main()
{
    try
    {
        Employee employee;
        std::map<std::string, Employee> employees;
        read_data_from_file("data5.txt", employees);

        employee.calculate_total_cost("Ivanov Ivan Ivanovich", employees);
        employee.list_contracts("Nikolaev Nikolai Nikolaevich", employees);
        employee.find_most_expensive_contract("Petrov Petr Petrovich", employees);
        employee.find_longest_contract("Evgenev Evreni Evgenevich", employees);
    }
    catch (const std::invalid_argument& e)
    {
        std::cout << e.what() << '\n';
    }

    return 0;
}