#include <iostream>
#include <random>
#include <algorithm>
#include <string>
#include <chrono>



template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(const T& value) : data(value), prev(nullptr), next(nullptr) {}
    };
    Node* head;
    Node* tail;
    int size;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    ~DoublyLinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }

    bool empty() const {
        return size == 0;
    }

    int length() const {
        return size;
    }

    void push_front(const T& value) {
        Node* new_node = new Node(value);
        if (empty()) {
            head = tail = new_node;
        }
        else {
            new_node->next = head;
            head->prev = new_node;
            head = new_node;
        }
        ++size;
    }

    void push_back(const T& value) {
        Node* new_node = new Node(value);
        if (empty()) {
            head = tail = new_node;
        }
        else {
            new_node->prev = tail;
            tail->next = new_node;
            tail = new_node;
        }
        ++size;
    }

    void pop_front() {
        if (empty()) {
            throw std::out_of_range("list is empty");
        }
        Node* temp = head;
        if (head == tail) {
            head = tail = nullptr;
        }
        else {
            head = head->next;
            head->prev = nullptr;
        }
        delete temp;
        --size;
    }

    void pop_back() {
        if (empty()) {
            throw std::out_of_range("list is empty");
        }
        Node* temp = tail;
        if (head == tail) {
            head = tail = nullptr;
        }
        else {
            tail = tail->prev;
            tail->next = nullptr;
        }
        delete temp;
        --size;
    }

    const T& front() const {
        if (empty()) {
            throw std::out_of_range("list is empty");
        }
        return head->data;
    }

    const T& back() const {
        if (empty()) {
            throw std::out_of_range("list is empty");
        }
        return tail->data;
    }

    void insert(int index, const T& value) {
        if (index < 0 || index > size) {
            throw std::out_of_range("index is out of range");
        }
        if (index == 0) {
            push_front(value);
        }
        else if (index == size) {
            push_back(value);
        }
        else {
            Node* current = head;
            for (int i = 0; i < index; ++i) {
                current = current->next;
            }
            Node* new_node = new Node(value);
            new_node->prev = current->prev;
            new_node->next = current;
            current->prev->next = new_node;
            current->prev = new_node;
            ++size;
        }
    }

    void erase(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("index is out of range");
        }
        if (index == 0) {
            pop_front();
        }
        else if (index == size - 1) {
            pop_back();
        }
        else {
            Node* current = head;
            for (int i = 0; i < index; ++i) {
                current = current->next;
            }
            current->prev->next = current->next;
            current->next->prev = current->prev;
            delete current;
            --size;
        }
    }


    void selectionSort() {
        Node* current = head;
        while (current != nullptr) {
            Node* minNode = current;
            Node* searchNode = current->next;
            while (searchNode != nullptr) {
                if (searchNode->data < minNode->data) {
                    minNode = searchNode;
                }
                searchNode = searchNode->next;
            }
            if (minNode != current) {
                T temp = current->data;
                current->data = minNode->data;
                minNode->data = temp;
            }
            current = current->next;
        }
    }

    void shuffle() {
        // Use the current time as a seed for the random number generator
        std::default_random_engine rng(std::chrono::system_clock::now().time_since_epoch().count());

        // Get the size of the list
        int size = 0;
        Node* current = head;
        while (current != nullptr) {
            size++;
            current = current->next;
        }

        // Create an array to hold the elements of the list
        T* array = new T[size];
        int i = 0;
        current = head;
        while (current != nullptr) {
            array[i++] = current->data;
            current = current->next;
        }

        // Shuffle the array using the random number generator
        for (int i = size - 1; i > 0; i--) {
            std::uniform_int_distribution<int> distribution(0, i);
            int j = distribution(rng);
            T temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }

        // Put the shuffled elements back into the list
        i = 0;
        current = head;
        while (current != nullptr) {
            current->data = array[i++];
            current = current->next;
        }

        // Free the memory allocated for the array
        delete[] array;
    }

    void clear() {
        Node* current = head;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        head = tail = nullptr;
        size = 0;
    }

    // copy constructor
    DoublyLinkedList(const DoublyLinkedList<T>& other) : head(nullptr), tail(nullptr), size(0) {
        Node* current = other.head;
        while (current != nullptr) {
            push_back(current->data);
            current = current->next;
        }
    }

    // assignment operator
    DoublyLinkedList<T>& operator=(const DoublyLinkedList<T>& other) {
        if (this != &other) {
            clear();
            Node* current = other.head;
            while (current != nullptr) {
                push_back(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    // move constructor
    DoublyLinkedList(DoublyLinkedList<T>&& other) noexcept : head(other.head), tail(other.tail), size(other.size) {
        other.head = other.tail = nullptr;
        other.size = 0;
    }

    // move assignment operator
    DoublyLinkedList<T>& operator=(DoublyLinkedList<T>&& other) noexcept {
        if (this != &other) {
            clear();
            head = other.head;
            tail = other.tail;
            size = other.size;
            other.head = other.tail = nullptr;
            other.size = 0;
        }
        return *this;
    }

    class Iterator {
    private:
        Node* current;

    public:
        Iterator(Node* ptr) : current(ptr) {}

        Iterator& operator++() {
            current = current->next;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp(current);
            current = current->next;
            return temp;
        }

        Iterator& operator--() {
            current = current->prev;
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp(current);
            current = current->prev;
            return temp;
        }

        T& operator*() {
            return current->data;
        }

        bool operator==(const Iterator& other) const {
            return current == other.current;
        }

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };

    Iterator begin() {
        return Iterator(head);
    }

    Iterator end() {
        return Iterator(nullptr);
    }

    Iterator rbegin() {
        return Iterator(tail);
    }

    Iterator rend() {
        return Iterator(nullptr);
    }
};


void testDoublyLinkedList(int N) {
    // Initialize the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distr(-1000, 1000);

    // Create a new DoublyLinkedList
    DoublyLinkedList<int> mylist;

    // Fill the list with 1000 random integers
    for (int i = 0; i < N; ++i) {
        mylist.push_back(distr(gen));
    }

    // Compute the sum, average, minimum, and maximum of the list
    int sum = 0;
    int min = std::numeric_limits<int>::max();
    int max = std::numeric_limits<int>::min();

    for (auto it : mylist) {
        sum += it;
        if (it < min) {
            min = it;
        }
        if (it > max) {
            max = it;
        }
    }

    double avg = static_cast<double>(sum) / static_cast<double>(mylist.length());

    // Output the results
    std::cout << "Sum: " << sum << std::endl;
    std::cout << "Average: " << avg << std::endl;
    std::cout << "Minimum: " << min << std::endl;
    std::cout << "Maximum: " << max << std::endl;

    
}


void testString(int N) 
{
    DoublyLinkedList<std::string> mylist;

    for (int i = 0; i < N; i++) {
        mylist.push_back("old");
    }

    std::cout << "Our list before adding 'new' in 5 index : " ;
    for (auto it : mylist) {
        std::cout << it << " ";
    }
    std::cout << std::endl;

    mylist.insert(5, "new");
    std::cout << "Our list after adding 'new' in 5 index :  ";
    for (auto it : mylist) { 
        std::cout << it << " "; 
    }

    std::cout << std::endl;

    mylist.erase(5);
    std::cout << "Our list after erase 'new' in 5 index :   ";
    for (auto it : mylist) {
        std::cout << it << " ";
    }

    std::cout << std::endl;

}


void testSort(int N)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distr(-1000, 1000);

    // Create a new DoublyLinkedList
    DoublyLinkedList<int> mylist;
    std::vector<int> vec(1000);

    // Fill the list with 1000 random integers
    for (int i = 0; i < N; ++i) {
        mylist.push_back(distr(gen));
    }

    std::copy(mylist.begin(), mylist.end(), vec.begin());

    std::sort(vec.begin(), vec.end());

    mylist.selectionSort();

    int i = 0;
    int mistakes = 0;
    for (auto l : mylist) {

        if (l != vec[i]) {
            mistakes++;
        }
        std::cout << l << std::endl;
        i++;
    }

    std::cout << "_____________________________" << std::endl;
    std::cout << "Number of mistakes: " << mistakes << std::endl;
    
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "Shuffle list: " << std::endl;
    mylist.shuffle();
    for (auto l : mylist) {
        std::cout << l << std::endl;
    }
}


std::vector<std::string> lastNames = { "Ivanov", "Petrov", "Alexandrov", "Vladislavov", "Dmitriev", "Sergeev", "Davidov", "Maksimov", "Semenov", "Bogdanov" };
std::vector<std::string> firstNames = { "Ivan", "Petr", "Semen", "Alex", "Vladislav", "Dmitriy", "Sergey", "David", "Maksim", "Bogdan" };
std::vector<std::string> patronymics = { "Ivanovich", "Petrovich", "Alexandrovich", "Vladislavovich", "Dmitrievich", "Sergeevich", "Davidovich", "Maksimovich", "Semenovich", "Bogdanovich" };

// Structure for holding personal data
struct Person {
    std::string lastName;
    std::string firstName;
    std::string patronymic;
    std::string dob;
};

int randomInt(int min, int max) {
    return rand() % (max - min + 1) + min;
}

std::string randomDob() {
    int day = randomInt(1, 31);
    int month = randomInt(1, 12);
    int year = randomInt(1980, 2020);
    return std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year);
}

std::string randomName(const std::vector<std::string>& names) {
    return names[randomInt(0, names.size() - 1)];
}


void testStructure() 
{
    DoublyLinkedList<Person> people;
    for (int i = 0; i < 100; i++) {
        Person person;
        person.lastName = randomName(lastNames);
        person.firstName = randomName(firstNames);
        person.patronymic = randomName(patronymics);
        person.dob = randomDob();
        people.push_back(person);
    }

    DoublyLinkedList<Person> under20;
    DoublyLinkedList<Person> over30;
    DoublyLinkedList<Person> sortedpeople;
    for (const auto& person : people) {
        int year = std::stoi(person.dob.substr(6, 4));
        int age = 2023 - year;
        if (age < 20) {
            under20.push_back(person);
            sortedpeople.push_back(person);
        }
        else if (age > 30) {
            over30.push_back(person);
            sortedpeople.push_back(person);
        }
    }

    int mistakesCount = 0;
    for (const auto& person : sortedpeople) {
        int year = std::stoi(person.dob.substr(6, 4));
        int age = 2023 - year;
        if ((age > 20)  && (age < 30)) {
            mistakesCount++;
        }
    }
    std::cout << "Number of people under 20: " << under20.length() << std::endl;
    std::cout << "Number of people over 30: " << over30.length() << std::endl;
    std::cout << "Number of people under 20 and over 30: " << sortedpeople.length() << std::endl;
    std::cout << "Number of people between 20 and 30 in new list: " << mistakesCount << std::endl;

}


int main() 
{
    srand(time(nullptr));
    int len = 1000;
    testDoublyLinkedList(1000);
    len = 10;
    testString(len);
    len = 1000;
    testStructure();
    testSort(len);
    DoublyLinkedList<int> list;
    list.push_back(1);
    list.push_back(10);
    list.push_back(3);
    list.push_back(8);
    for (auto el : list) { std::cout << el << std::endl; }
    std::cout << "Length: " << list.length();

}