#include "stdafx.h"

#include "Algorithms.hpp"

class Person {
public:
    std::string name;
    int age;

    Person(const std::string& name = "", int age = 0) : name(name), age(age) {}
    Person(Person const& other) = default;
    Person(Person&& other) = default;
    Person& operator=(Person const& other) = default;
    Person& operator=(Person&& other) = default;

    // Overload the + operator
    Person operator+(const Person& other) const {
        return Person(this->name + " " + other.name, this->age + other.age);
    }

    std::strong_ordering operator<=>(Person const& other) const = default;
};

static void test()
{
    std::vector arr{ 1, 3, 4, 5, 98, 2, 4, 5, 7, 8, 12, 45, 67, 89 };

    #pragma region Find
    {
        auto find_it = std::find(arr.begin(), arr.end(), 45);
        assert(find_it != arr.end() && std::distance(arr.begin(), find_it) == 11);

        find_it = alg::find(arr.begin(), arr.end(), 100);
        assert(find_it == arr.end());
    }
#pragma endregion 

    #pragma region Find_last
    {
        auto find_last_it = alg::find_last(arr.begin(), arr.end(), 5);
        assert(find_last_it != arr.end() && std::distance(arr.begin(), find_last_it) == 7);

        find_last_it = alg::find_last(arr.begin(), arr.end(), 4);
        assert(find_last_it != arr.end() && std::distance(arr.begin(), find_last_it) == 6);

        find_last_it = alg::find_last(arr.begin(), arr.end(), 100);
        assert(find_last_it == arr.end());
    }
#pragma endregion

    #pragma region Find_if
    {
        // Find the first element greater than 10
        auto find_if = alg::find_if(arr.begin(), arr.end(), [](int n) { return n > 10; });
        assert(find_if != arr.end() && *find_if == 98);

        // Find the first even number
        find_if = alg::find_if(arr.begin(), arr.end(), [](int n) { return n % 2 == 0; });
        assert(find_if != arr.end() && *find_if == 4);

        // Find an element greater than 100 (should not be found)
        find_if = alg::find_if(arr.begin(), arr.end(), [](int n) { return n > 100; });
        assert(find_if == arr.end());

        // Test with custom Person objects
        std::vector<Person> people = {
            Person("Alice", 30),
            Person("Bob", 25),
            Person("Charlie", 35)
        };

        // Find the first person older than 30
        auto person_it = alg::find_if(people.begin(), people.end(), [](const Person& p) { return p.age > 30; });
        assert(person_it != people.end() && person_it->name == "Charlie");

        // Find the first person named "Bob"
        person_it = alg::find_if(people.begin(), people.end(), [](const Person& p) { return p.name == "Bob"; });
        assert(person_it != people.end() && person_it->name == "Bob");

        // Find a person older than 40 (should not be found)
        person_it = alg::find_if(people.begin(), people.end(), [](const Person& p) { return p.age > 40; });
        assert(person_it == people.end());
    }
#pragma endregion

    #pragma region Find_first_of
    {
        std::vector<int> numbers = { 1, 3, 4, 5, 98, 2, 4, 5, 7, 8, 12, 45, 67, 89 };
        std::vector<int> targets = { 5, 12, 99 };

        numbers.begin() = targets.begin();

        // Find the first element in numbers that matches any element in targets
        auto find_it = alg::find_first_of(numbers.begin(), numbers.end(), targets.begin(), targets.end());
        assert(find_it != numbers.end() && *find_it == 5);

        // Test with different targets
        targets = { 100, 200, 300 };
        find_it = alg::find_first_of(numbers.begin(), numbers.end(), targets.begin(), targets.end());
        assert(find_it == numbers.end());

        //// Test with custom Person objects
        //std::vector<Person> people = {
        //    Person("Alice", 30),
        //    Person("Bob", 25),
        //    Person("Charlie", 35)
        //};

        //std::vector<Person> target_people = { Person("Charlie", 35), Person("Eve", 40) };

        //// Find the first person in people that matches any person in target_people
        //auto person_it = alg::find_first_of(people.begin(), people.end(), target_people.begin(), target_people.end());
        //assert(person_it != people.end() && person_it->name == "Charlie");

        //// Test with non-matching targets
        //target_people = { Person("Eve", 40), Person("Frank", 50) };
        //person_it = alg::find_first_of(people.begin(), people.end(), target_people.begin(), target_people.end());
        //assert(person_it == people.end());
    }
#pragma endregion

    #pragma region Replace
    {
        // Test with integers for replace
        std::vector<int> numbers = { 1, 3, 4, 5, 98, 2, 4, 5, 7, 8, 12, 45, 67, 89 };

        // Replace all occurrences of 4 with 100
        alg::replace(numbers.begin(), numbers.end(), 4, 100);
        assert(alg::find(numbers.begin(), numbers.end(), 4) == numbers.end());
        assert(std::count(numbers.begin(), numbers.end(), 100) == 2);

        // Test with custom Person objects for replace
        std::vector<Person> people = {
            Person("Alice", 30),
            Person("Bob", 25),
            Person("Charlie", 35),
            Person("Alice", 30)
        };

        // Replace all occurrences of Alice with Eve
        alg::replace(people.begin(), people.end(), Person("Alice", 30), Person("Eve", 40));
        assert(alg::find(people.begin(), people.end(), Person("Alice", 30)) == people.end());
        assert(std::count(people.begin(), people.end(), Person("Eve", 40)) == 2);
    }
#pragma endregion

    #pragma region Replace_if
    {
        // Test with integers
        std::vector<int> numbers = { 1, 3, 4, 5, 98, 2, 4, 5, 7, 8, 12, 45, 67, 89 };

        // Replace elements greater than 10 with 0
        std::vector<int> expected_numbers = { 1, 3, 4, 5, 0, 2, 4, 5, 7, 8, 0, 0, 0, 0 };
        alg::replace_if(numbers.begin(), numbers.end(), [](int n) { return n > 10; }, 0);
        assert(alg::equal(numbers.begin(), numbers.end(), expected_numbers.begin()));

        // Replace even numbers with -1
        expected_numbers = { 1, 3, -1, 5, 0, -1, -1, 5, 7, -1, 0, 0, 0, 0 };
        alg::replace_if(numbers.begin(), numbers.end(), [](int n) { return n != 0 && n % 2 == 0; }, -1);
        assert(alg::equal(numbers.begin(), numbers.end(), expected_numbers.begin()));

        // Replace numbers less than 5 with 99
        expected_numbers = { 99, 99, -1, 5, 0, -1, -1, 5, 7, -1, 0, 0, 0, 0 };
        alg::replace_if(numbers.begin(), numbers.end(), [](int n) { return n < 5 && n > 0; }, 99);
        assert(alg::equal(numbers.begin(), numbers.end(), expected_numbers.begin()));

        // Test with custom Person objects
        std::vector<Person> people = {
            Person("Alice", 30),
            Person("Bob", 25),
            Person("Charlie", 35),
            Person("Alice", 40)
        };

        // Replace persons named "Alice" with a new person
        std::vector<Person> expected_people = {
            Person("John", 50),
            Person("Bob", 25),
            Person("Charlie", 35),
            Person("John", 50)
        };
        alg::replace_if(people.begin(), people.end(), [](const Person& p) { return p.name == "Alice"; }, Person("John", 50));
        assert(alg::equal(people.begin(), people.end(), expected_people.begin()));

        //// Replace persons older than 30 with "Senior"
        //expected_people = {
        //    Person("John", 50),
        //    Person("Bob", 25),
        //    Person("Senior", 65),
        //    Person("John", 50)
        //};
        //alg::replace_if(people.begin(), people.end(), [](const Person& p) { return p.age > 30 && p.name != "John"; }, Person("Senior", 65));
        //assert(alg::equal(people.begin(), people.end(), expected_people.begin()));

        //// Replace persons named "Bob" with a new person
        //expected_people = {
        //    Person("John", 50),
        //    Person("New Bob", 99),
        //    Person("Senior", 65),
        //    Person("John", 50)
        //};
        //alg::replace_if(people.begin(), people.end(), [](const Person& p) { return p.name == "Bob" && p.name != "Senior" && p.name != "John"; }, Person("New Bob", 99));
        //assert(alg::equal(people.begin(), people.end(), expected_people.begin()));
    }
#pragma endregion

    #pragma region Move
    {
        // Test with integers
        std::vector<int> numbers = { 1, 3, 4, 5, 98, 2, 4, 5, 7, 8, 12, 45, 67, 89 };
        std::vector<int> target(numbers.size());

        // Move elements from numbers to target
        alg::move(numbers.begin(), numbers.end(), target.begin());

        std::vector<int> expected_target = { 1, 3, 4, 5, 98, 2, 4, 5, 7, 8, 12, 45, 67, 89 };
        assert(alg::equal(target.begin(), target.end(), expected_target.begin()));

        // Test with custom Person objects
        std::vector<Person> people = {
            { "Alice", 30 },
            { "Bob", 25 },
            { "Charlie", 35 },
            { "Alice", 40 }
        };
        std::vector<Person> target_people(people.size());

        // Move elements from people to target_people
        alg::move(people.begin(), people.end(), target_people.begin());

        std::vector<Person> expected_target_people = {
            { "Alice", 30 },
            { "Bob", 25 },
            { "Charlie", 35 },
            { "Alice", 40 }
        };
        assert(alg::equal(target_people.begin(), target_people.end(), expected_target_people.begin()));

        // Ensure the source vector is now in a valid but unspecified state
        for (const auto& p : people) {
            assert(p.name.empty());
        }
    }
#pragma endregion

    #pragma region Accumulate
    {
        auto sum = alg::accumulate(arr.begin(), arr.end(), 0);
        assert(sum == std::accumulate(arr.begin(), arr.end(), 0));

        // Test with initial value 100
        sum = alg::accumulate(arr.begin(), arr.end(), 100);
        assert(sum == std::accumulate(arr.begin(), arr.end(), 100));

        // Test with a different dataset
        std::vector<int> arr2{ 10, 20, 30, 40, 50 };
        sum = alg::accumulate(arr2.begin(), arr2.end(), 0);
        assert(sum == std::accumulate(arr2.begin(), arr2.end(), 0));

        sum = alg::accumulate(arr2.begin(), arr2.end(), 100);
        assert(sum == std::accumulate(arr2.begin(), arr2.end(), 100));

        std::vector<Person> people = { Person("Alice"), Person("Bob"), Person("Charlie") };

        // Initial value as an empty Person
        Person initial("");
        Person result = alg::accumulate(people.begin(), people.end(), initial);

        // Expected concatenated name
        Person expected(" Alice Bob Charlie");
        assert(result.name == expected.name);

        // Test with a different initial value
        Person initial2("Names:");
        Person result2 = alg::accumulate(people.begin(), people.end(), initial2);

        // Expected concatenated name with initial value
        Person expected2("Names: Alice Bob Charlie");
        assert(result2.name == expected2.name);

        /*int sum = alg::accumulate(arr.begin(), arr.end(), 0, std::plus<int>());
        assert(sum == std::accumulate(arr.begin(), arr.end(), 0));

        int product = alg::accumulate(arr.begin(), arr.end(), 1, std::multiplies<int>());
        assert(product == std::accumulate(arr.begin(), arr.end(), 1, std::multiplies<int>()));

        int sum_with_initial = alg::accumulate(arr.begin(), arr.end(), 100, std::plus<int>());
        assert(sum_with_initial == std::accumulate(arr.begin(), arr.end(), 100));*/
    }
#pragma endregion

    #pragma region Adjacent_Find
    {
        // Test case 1: Standard adjacent_find for equal elements
        auto std_adj_it = std::adjacent_find(arr.begin(), arr.end());
        auto custom_adj_it = alg::adjacent_find(arr.begin(), arr.end());
        assert(std_adj_it == custom_adj_it);

        // Test case 2: Custom adjacent_find for equal elements
        std_adj_it = std::adjacent_find(arr.begin(), arr.end(), alg::equal_to());
        custom_adj_it = alg::adjacent_find(arr.begin(), arr.end(), alg::equal_to());
        assert(std_adj_it == custom_adj_it);

        // Test case 3: No adjacent equal elements
        std::vector<int> no_adj_arr{ 1, 2, 3, 4, 5, 6, 7, 8 };
        std_adj_it = std::adjacent_find(no_adj_arr.begin(), no_adj_arr.end());
        custom_adj_it = alg::adjacent_find(no_adj_arr.begin(), no_adj_arr.end());
        assert(std_adj_it == custom_adj_it);
    }
    #pragma endregion

}