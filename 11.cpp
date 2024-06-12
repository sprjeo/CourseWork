#include <iostream>
#include <vector>
#include <forward_list>
#include <cstdarg>
#include <algorithm>
#include <random>
#include <set>
#include <utility>



template<class T>
class MyQueue
{
public:
    class q
    {
    public:
        T value;
        q* next;
    };

    q* f = nullptr; // указатель на первый элемент очереди
    q* last = nullptr; // указатель на последний элемент очереди

public:
    void push(T a)
    { 
        q* p = new q();
        p->value = a;
        p->next = nullptr;
        if (f == nullptr)
        {
            f = p;
            last = p;
        }
        else
        {
            last->next = p;
            last = p;
        }
    }
    bool pop(T& a)
    {
        q* p;
        if (f == nullptr)
            return false;
        else
        {
            a = f->value;
            p = f;
            f = f->next;
            delete p;
            return true;
        }
    }

    class Iterator
    {
    private:
        q* current;

    public:
        Iterator(q* cell) : current(cell) {}

        Iterator& operator++()
        {
            current = current->next;
            return *this;
        }

        bool operator!=(const Iterator& other) const
        {
            return current != other.current;
        }

        bool operator==(const Iterator& other) const
        {
            return current == other.current;
        }

        T& operator*()
        {
            return current->value;
        }
    };

    Iterator begin() 
    {
        return Iterator(f);
    }

    Iterator end() 
    {
        return Iterator(nullptr);
    }

    bool empty()
    {
        return begin() == end();
    }
};


template<typename T> 
class MyStack
{
public:
    struct Cell 
    {
        T value;
        Cell *next;
    };
    
    Cell *beginning = nullptr;
 
    void output(Cell *a)
    {
        if (a==NULL) return;
        output(a->next);
        std::cout << a->value << " ";
    }

public:
    void push(T item)
    { 
        Cell *c = new Cell();
        c->value=item;
        c->next=beginning;
        beginning=c;
    } 
      
    T pop()
    {
        if (beginning != NULL) 
        {
            Cell *a = beginning;
            beginning = a->next;
            T value = a->value;
            delete a;
            return value;
        }
        else
        {
            throw std::out_of_range("Stack is empty!");
        }
    }

    class Iterator
    {
    private:
        Cell* current;
        Cell* prev;

    public:
        Iterator(Cell* cell) : current(cell) {}

        Iterator& operator++()
        {
            prev = current;
            current = current->next;
            return *this;
        }

        bool operator!=(const Iterator& other) const
        {
            return current != other.current;
        }

        T& operator*()
        {
            return current->value;
        }
    };

    Iterator begin() 
    {
        return Iterator(beginning);
    }

    Iterator end() 
    {
        return Iterator(nullptr);
    }

    bool empty()
    {
        return !(begin() != end());
    }
};

template<typename T>
class MyVector {
public:
    void push(const T& value) 
    {
        vector.push_back(value);
    }

    typename std::vector<T>::iterator begin() 
    {
        return vector.begin();
    }

    typename std::vector<T>::iterator end()
    {
        return vector.end();
    }
public:
    std::vector<T> vector;

    bool empty()
    {
        return begin() == end();
    }
};

template<typename T>
class MyForwardList {
public:
    void push(const T& value) 
    {
        list.push_front(value);
    }

    typename std::forward_list<T>::iterator begin() 
    {
        return list.begin();
    }

    typename std::forward_list<T>::iterator end() 
    {
        return list.end();
    }

    bool empty()
    {
        return begin() == end();
    }

private:
    std::forward_list<T> list;
};


class Ticket 
{
public:
    std::pair<int, int> num_prize;

    bool operator<(const Ticket& other) const 
    {
        if (num_prize.first != other.num_prize.first) 
        {
            return num_prize.first <other.num_prize.first;
        }
        return num_prize.second < other.num_prize.second;
    }

    Ticket(int num, int p) : num_prize{num, p} {}
    Ticket() {}

    void swap(Ticket& t)
    {
        std::swap( num_prize, t.num_prize);
    }

    friend void swap(Ticket& t1, Ticket& t2);
};

void swap(Ticket& t1, Ticket& t2)
{
    std::swap( t1.num_prize, t2.num_prize);
}

template<typename T>
class Edition
{
    public:
    int amount_of_all_tickets;
    int amount_of_sold_tickets;
    T winners;

    Edition(int all, int sold): amount_of_all_tickets(all), amount_of_sold_tickets(sold) {}
};


template <typename T>
void calculate_the_winners(T& winners, int amount_of_sold_tickets, T tickets)
{
    if (amount_of_sold_tickets <= 0 || tickets.empty())
    {
        throw std::invalid_argument("invalid argument in calculate_the_winners()");
    }
    int holder = amount_of_sold_tickets;
    for (auto i = tickets.begin(); i != tickets.end(); ++i)
    {
        if (holder == 0)
        {
            break;
        }
        else if ((*i).num_prize.second == 0)
        {
            continue;
        }
        else 
        {
            winners.push(*i);
        }
        holder--;
    }
}

template <typename T>
void genetrate_the_tickets(std::mt19937 g, int amount_of_tickets, T& tickets, int amount_of_prizes_types, ...)
{
    if (amount_of_tickets <= 0 || amount_of_prizes_types <= 0 )
    {
        throw std::invalid_argument("invalid argument in genetrate_the_tickets()");
    }
    std::vector<int> prize_money, prize_type_amount_of_tickets;
    va_list args;
    va_start(args, amount_of_prizes_types);
    
    for (int i=0; i<amount_of_prizes_types; ++i)
    {
        int a = va_arg(args, int);
        prize_type_amount_of_tickets.push_back(a);  
        int hold = va_arg(args, int);
        for (auto i = 0; i < a; ++i)
        {
            prize_money.push_back(hold);
        }
    }
    va_end(args);

    std::vector<int> sh_numbers;
    for (auto i  = 1; i <= amount_of_tickets; ++i )
    {
        sh_numbers.push_back(i);
    }
    
    int amount_of_prize_tickets = 0;
    for (auto i = prize_type_amount_of_tickets.cbegin(); i != prize_type_amount_of_tickets.cend(); ++i)
    {
        amount_of_prize_tickets = amount_of_prize_tickets + *i;
    }

    std::set<int> uniqueNumbers;
    while (uniqueNumbers.size() < amount_of_prize_tickets) 
    {
        std::uniform_int_distribution<int> distribution(1, amount_of_tickets);
        int random = distribution(g);
        uniqueNumbers.insert(random);
    }

    int counter = 0;
    for (auto i = 1; i <= amount_of_tickets; ++i)
    {
        auto it = uniqueNumbers.find(sh_numbers[i]);
       
        if (it != uniqueNumbers.end())
        { 
            Ticket ticket(sh_numbers[i], prize_money[counter]);
            tickets.push(ticket);
            counter++;
        }
        else
        {
            Ticket ticket(sh_numbers[i], 0);
            tickets.push(ticket);
        } 
    }
}


int main() 
{
    try
    {
        std::random_device rd;
        std::mt19937 g(rd());
        int amount_of_tickets = 30000000;
        int sold = 27000000;
        if (sold > amount_of_tickets)
        {
            throw std::invalid_argument("too many sold tickets");
        }

        MyVector<Ticket> tickets;
        MyVector<Ticket> winners;
        Edition< MyVector<Ticket>> edition1(amount_of_tickets, sold);

        genetrate_the_tickets(g, edition1.amount_of_all_tickets, tickets, 4, 1, 1000000, 2, 500000, 3, 300000, 4, 25000);
        
        calculate_the_winners(winners, edition1.amount_of_sold_tickets, tickets);

        for (auto i = winners.begin(); i != winners.end(); ++i)
        {
            std::cout<<"winning ticket number is " <<(*i).num_prize.first << ", the prize is "<< (*i).num_prize.second << " rubles"<<std::endl;
        }
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << '\n'; 
    }
    return 0;
}