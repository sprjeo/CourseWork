#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <cmath>
#include <array>
#include <sstream>

class Interpretator
{
public:

    std::vector<int> arrays[26]; // A - Z

public:

    //1 загрузить целые числа из файла

    void load_array (const char array_name, const std::string file_name)
    {
        if (array_name < 'A' || array_name > 'Z')
        {
            throw std::invalid_argument("invalid array name");
        }
        std::ifstream file(file_name);
        if (file.is_open())
        {
            arrays[array_name - 'A'].clear();

            int num;
            while (file >> num)
            {
                arrays[array_name - 'A'].push_back(num);
            }
            file.close();
        }
        else
        {
            throw std::ifstream::failure("File couldn't be opened");
        } 
    }

    //2 сохраняет массив в файл

    void save_array(const char array_name, const std::string file_name)
    {
        if (array_name < 'A' || array_name > 'Z')
        {
            throw std::invalid_argument("invalid array name");
        }
        std::ofstream out_file;

        out_file.open(file_name);
        if (out_file.is_open())
        {
            for (auto iter = arrays[array_name - 'A'].cbegin(); iter != arrays[array_name - 'A'].cend(); ++iter)
            {
                out_file << *iter << " ";
            }
            out_file.close();
        }
        else
        {
            throw std::ofstream::failure("file couldn't be opened");
        }
    }

    //3 заполнить массив случайными элементами

    void random_elements_to_array(const char array_name, const int count, const int lb, const int rb)
    {
        if (array_name < 'A' || array_name > 'Z')
        {
            throw std::invalid_argument("invalid array name");
        }
       // if (lb > rb)
       // {
       //     throw std::invalid_argument("invalid interval");
       // }
        
        arrays[array_name - 'A'].clear();
        std::srand(std::time(nullptr));
        int range = rb - lb + 1;
        int rand_num;
        for (int i = 0; i < count; ++i)
        {
            rand_num = std::rand() % range + lb;
            arrays[array_name - 'A'].push_back(rand_num);
        }
        
    }

    //4 объединить 2 массива
    void concat_arrays(const char array_name_A, const char array_name_B)
    {
        if (array_name_A < 'A' || array_name_B > 'Z' || array_name_B < 'A' || array_name_A > 'Z')
        {
            throw std::invalid_argument("invalid array name");
        }
        
        for (auto iter = arrays[array_name_B - 'A'].cbegin(); iter != arrays[array_name_B - 'A'].cend(); ++iter)
        {
            arrays[array_name_A - 'A'].push_back(*iter);
        }    
    }

    //5 очистить массив
    void free_array(const char array_name)
    {
        if (array_name < 'A' || array_name > 'Z')
        {
            throw std::invalid_argument("invalid array name");
        }
        
        arrays[array_name - 'A'].clear();
    }

    //6 удалить из массива amount элементов начиная с элемента index
    void remove_elements_array(const char array_name, int index, int amount)
    {
        if (array_name < 'A' || array_name > 'Z')
        {
            throw std::invalid_argument("invalid array name");
        }
      
        arrays[array_name - 'A'].erase(arrays[array_name - 'A'].cbegin() + index, arrays[array_name - 'A'].cbegin() + index + amount);  
    }


    //7 копировать из массива A в B начиная с index_of_start заканчивая index_of_end
    void copy_from_array_to_array(const char array_name_A, int index_of_start, int index_of_end, const char array_name_B)
    {
        if (array_name_A < 'A' || array_name_B > 'Z' || array_name_B < 'A' || array_name_A > 'Z')
        {
            throw std::invalid_argument("invalid array name");
        }

        arrays[array_name_B - 'A'].assign(arrays[array_name_A - 'A'].cbegin() + index_of_start, arrays[array_name_A - 'A'].cbegin() + index_of_end + 1);   
    }

    //8 сортировать по возрастанию или убыванию
    void sort_array(const char array_name, const char how_to_sort)
    {
        if (array_name < 'A' || array_name > 'Z')
        {
            throw std::invalid_argument("invalid array name");
        }
        if (how_to_sort == '+')
        {
            std::sort(arrays[array_name - 'A'].begin(), arrays[array_name - 'A'].end());
        }
        else if (how_to_sort == '-')
        {
            std::sort(arrays[array_name - 'A'].rbegin(), arrays[array_name - 'A'].rend());
        }
        else
        {
            throw std::invalid_argument("invalid argument");
        }  
    }

    //9 переставить элементы в случайном порядке 
    void permute_array(const char array_name)
    {
        if (array_name < 'A' || array_name > 'Z')
        {
            throw std::invalid_argument("invalid array name");
        }
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(arrays[array_name - 'A'].begin(), arrays[array_name - 'A'].end(), g); 
    }

    //10 найти пересечение
    void intersect_arrays(const char array_name_A, const char array_name_B)
    {
        if (array_name_A < 'A' || array_name_B > 'Z' || array_name_B < 'A' || array_name_A > 'Z')
        {
            throw std::invalid_argument("invalid array name");
        }
        std::vector<int>& array_A = arrays[array_name_A - 'A'];
        std::vector<int>& array_B = arrays[array_name_A - 'A'];

        std::unordered_set<int> set(array_B.begin(), array_B.end());
        std::vector<int> result;

        for (int num : array_A) {
            if (set.find(num) != set.end()) {
                result.push_back(num);
            }
        }
        array_A = result;
    }

    //11 симметрическая разность
    void XOR_arrays(const char array_name_A, const char array_name_B)
    {
        if (array_name_A < 'A' || array_name_B > 'Z' || array_name_B < 'A' || array_name_A > 'Z')
        {
            throw std::invalid_argument("invalid array name");
        }

        std::vector<int> result;
        sort_array(array_name_A, '+');
        sort_array(array_name_B, '+');
        std::set_symmetric_difference(arrays[array_name_A - 'A'].cbegin(), arrays[array_name_A - 'A'].cend(), arrays[array_name_B - 'A'].cbegin(), arrays[array_name_B - 'A'].cend(), std::back_inserter(result));
        arrays[array_name_A - 'A'].clear();
        arrays[array_name_A - 'A'] = result;
    }

    //12 вывести инф о массиве
    void stats_of_array(const char array_name)
    {
        if (array_name < 'A' || array_name > 'Z')
        {
            throw std::invalid_argument("invalid array name");
        }
        
        std::cout << std::endl;
        std::cout << "size of array: " << arrays[array_name - 'A'].size() << std::endl;

        auto max_el = std::max_element(arrays[array_name - 'A'].cbegin(), arrays[array_name - 'A'].cend());
        auto max_el_index = max_el - arrays[array_name - 'A'].cbegin();
        std::cout << "array's biggest number: " << *max_el << " its index: " << max_el_index << std::endl;

        auto min_el = std::min_element(arrays[array_name - 'A'].cbegin(), arrays[array_name - 'A'].cend());
        auto min_el_index = min_el - arrays[array_name - 'A'].cbegin();
        std::cout << "array's smallest number: " << *min_el << " its index " << min_el_index << std::endl;

        std::unordered_map<int, int> elementCount;
        for (auto i : arrays[array_name - 'A'])
        {
            elementCount[i]++;
        }
        auto maxFrequentElementIt = std::max_element(elementCount.begin(), elementCount.end(), [](const auto& a, const auto& b) {return a.second > b.second; });
        std::cout << "array's most common number: " << maxFrequentElementIt->second << std::endl;

        double sum = 0;
        for (auto iter = arrays[array_name - 'A'].cbegin(); iter != arrays[array_name - 'A'].cend(); ++iter)
        {
            sum += *iter;
        }
        double mean = sum / arrays[array_name - 'A'].size();
        std::cout << "array's mean: " << mean << std::endl;

        double most_diff;
        double diff_1 = abs(*max_el - mean);
        double diff_2 = abs(*min_el - mean);

        if (diff_1 > diff_2)
        {
            most_diff = diff_1;
        }
        else
        {
            most_diff = diff_2;
        }
        std::cout << "array's biggest diffeerence from mean: " << most_diff << std::endl;
        
    }

    //13 вывести элементы массива от start_index до end_index
    void print_elements_from_to_indeces(const char array_name, int start_index, int end_index)
    {
        if (array_name < 'A' || array_name > 'Z')
        {
            throw std::invalid_argument("invalid array name");
        }
        else if (arrays[array_name - 'A'].cend() - arrays[array_name - 'A'].cbegin() + end_index < 0)
        {
            throw std::out_of_range("invalid array indices");
        }
        
        std::cout << std::endl;
        for (auto i = start_index; i <= end_index; ++i)
        {
            std::cout << arrays[array_name - 'A'][i] << " ";
        }
        std::cout << std::endl;
        
    }

    //14 вывести все элемента массива
    void print_all_from_array(const char array_name)
    {
        if (array_name < 'A' || array_name > 'Z')
        {
            throw std::invalid_argument("invalid array name");
        }
        
        std::cout << std::endl;
        for (auto i = arrays[array_name - 'A'].cbegin(); i != arrays[array_name - 'A'].cend(); ++i)
        {
            std::cout << *i << " ";
        }
        std::cout << std::endl;
        
    }
};

void read_file(std::string file_name, std::vector<std::string>& command_strings)
{
    if (file_name.empty())
    {
        throw std::invalid_argument("invalid file name");
    }

    std::ifstream file;
    file.open(file_name);

    if (!file.is_open())
    {
        throw std::ifstream::failure("File couldn't be opened");
    }

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
        command_strings.push_back(line.substr(0, line.length() - 1));
    }
    file.close();
}

void parse_command_to_int(std::string& holder, int& result)
{
    holder = holder.substr(0, holder.length() - 1);
    result = stoi(holder);
}

int main()
{
    std::string command;
    Interpretator interpretator;
    std::vector<std::string> command_strings;

    try
    {
        read_file("data2.txt", command_strings);

        for (auto i = command_strings.cbegin(); i != command_strings.cend(); ++i)
        {
            std::string holder;
            std::stringstream ss(*i);
            std::string operation, array_name, file_name;
            ss >> operation >> array_name;

            char array = toupper(array_name[0]);

            if (operation == "Load")
            {
                ss >> file_name;
                interpretator.load_array(array, file_name);
                std::cout << "Array " << array << " loaded from " << file_name << std::endl;

            }
            else if (operation == "Save")
            {
                ss >> file_name;
                interpretator.save_array(array, file_name);
                std::cout << "Array " << array << " saved to " << file_name << std::endl;
            }
            else if (operation == "Rand")
            {
                int count, lb, rb;
                ss >> count >> lb >> rb;
                interpretator.random_elements_to_array(array, count, lb, rb);
                std::cout << "Array " << array << " filled with random numbers." << std::endl;
            }
            else if (operation == "Concat")
            {
                char array_name2;
                ss >> holder;
                array_name2 = holder[0];
                interpretator.concat_arrays(array, toupper(array_name2));
                std::cout << "Arrays " << array << " and " << char(toupper(array_name2)) << " concatenated." << std::endl;
            }
            else if (operation == "Free")
            {
                interpretator.free_array(array);
                std::cout << "Array " << array << " cleared." << std::endl;
            }
            else if (operation == "Remove")
            {
                int amount, index;
                ss >> holder;
                parse_command_to_int(holder, index);
                ss >> holder;
                amount = stoi(holder);

                interpretator.remove_elements_array(array, amount, index);
                std::cout << "Elements from array " << array << " are removed." << std::endl;
            }
            else if (operation == "Copy")
            {
                char array_name2;
                int start_index, end_index;
                ss >> holder;
                parse_command_to_int(holder, start_index);
                ss >> holder;
                parse_command_to_int(holder, end_index);
                ss >> holder;
                array_name2 = holder[0];

                interpretator.copy_from_array_to_array(array, start_index, end_index, toupper(array_name2));
                std::cout << "Elements in array " << char(toupper(array_name2)) << " are copied from " << array << std::endl;
            }
            else if (operation == "Sort")
            {
                char how_to_sort;
                how_to_sort = array_name[1];

                interpretator.sort_array(array, how_to_sort);
                std::cout << "Array " << array << " is sorted " << std::endl;
            }
            else if (operation == "Permute")
            {
                interpretator.permute_array(array);
                std::cout << "Array " << array << " is permuted." << std::endl;
            }
            else if (operation == "Intersect")
            {
                char array_name2;
                ss >> holder;
                array_name2 = holder[0];

                interpretator.intersect_arrays(array, toupper(array_name2));
                std::cout << "Arrays " << array << " and " << char(toupper(array_name2)) << " are intersected." << std::endl;
            }
            else if (operation == "Xor")
            {
                char array_name2;
                ss >> holder;
                array_name2 = holder[0];
                interpretator.XOR_arrays(array, toupper(array_name2));
                std::cout << "XOR from " << array << " and " << char(toupper(array_name2)) << " is saved to " << array << std::endl;
            }
            else if (operation == "Stat")
            {   

                std::cout << std::endl << "Stats array " << array << ":";
                interpretator.stats_of_array(array);
                 std::cout << std::endl;
            }
            else if (operation == "Print")
            {
                ss >> holder;
                if (!(holder == "all"))
                {
                    int start_index, end_index;

                    parse_command_to_int(holder, start_index);
                    ss >> holder;
                    end_index = stoi(holder);

                    interpretator.print_elements_from_to_indeces(array, start_index, end_index);
                    std::cout << "Array's " << array << " elements from index " << start_index << " to " << end_index << std::endl;
                }
                else
                {
                    interpretator.print_all_from_array(array);
                    std::cout << "Array's " << array << " elements." << std::endl;
                }
            }
            else
            {
                std::cout << "Invalid command. Please try again." << std::endl;
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }

    return 0;
}