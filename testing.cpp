#include <functional>
#include <optional>
#include <array>
#include <vector>
#include <iostream>

template<typename T>
class Fixture
{
public:
    Fixture(
        std::function<T()> pre
    ) : Fixture(pre, [](T&) {})
    {}

    Fixture(
        T&& value
    ) : Fixture([value]() { return value; })
    {}

    Fixture(
        std::function<T()> pre,
        std::function<void(T&)> post
    ) : _pre(pre), _post(post)
    {}

    Fixture(const Fixture<T>& other) : Fixture(other._pre, other._post)
    {
    }

    ~Fixture()
    {
        if (_value) {
            _post(*_value);
        }
    }

    T& operator()() const
    {
        if (!_value) {
            _value = _pre();
        }
        return *_value;
    }

private:
    mutable std::optional<T> _value;
    std::function<T()> _pre;
    std::function<void(T&)> _post;
};

class Test
{
public:
    Test(const std::string& name, std::function<void()> test) : _name(name), _test(test)
    {
    }
    void execute()
    {
        _test();
    }
private:
    std::function<void()> _test;
    std::string _name;
};

template<typename T>
class Parametrized
{
public:
    Parametrized(
        const std::string& name, std::initializer_list<T>&& list, std::function<void(T&)> test
    ) : _name(name), _list(list), _test(test)
    {
    }

    void operator()()
    {
        for(auto& value: _list) {
            _test(value);
        }
    }
private:
    std::string _name;
    std::function<void(T&)> _test;
    std::vector<T> _list;
};

struct myfunctor
{
    void operator()()
    {
        std::cout << "from myfunctor" << std::endl;
    }
};

int main(int argc, char** argv)
{
    {
        Fixture<int> fixture(
            [](){ return 1; },
            [](int& integer) { std::cout << integer << std::endl; }
        );

        fixture() = 3;
        std::cout << "end of scope" << std::endl;
    }
    {
        Fixture<int> fixture(
                [](){ return 1; },
                [](int& integer) { std::cout << integer << std::endl; }
            );

        std::cout << "Testing test that modifies fixture value" << std::endl;

        Test("something", [fixture]() { 
            std::cout << fixture() << std::endl; 
            fixture() = 3;
        }).execute();

        std::cout << "Testing test that follows test that modified value" << std::endl;
        Test("something_else", [fixture]() { 
            std::cout << fixture() << std::endl; 
        }).execute();
    }
    {
        std::cout << "Testing fixture with constant." << std::endl;
        Fixture<int> fixture(2);
        Test("someting_else", [fixture]() {
            std::cout << fixture() << std::endl;
        }).execute();
    }

    {
        std::cout << "Testing fixture with constant array." << std::endl;
        Fixture<std::array<int, 4>> fixture({1,2,3,4});
        Test("someting_else", [fixture]() {
            for(const auto& value: fixture()) {
                std::cout << value << std::endl;
            }
        }).execute();
    }

    {
        std::cout << "Testing fixture with constant vector." << std::endl;
        Fixture<std::vector<int>> fixture({1,2,3,4});
        Test("someting_else", [fixture]() {
            for(const auto& value: fixture()) {
                std::cout << value << std::endl;
            }
        }).execute();
    }

    {
        std::cout << "Parametrized" << std::endl;
        Test(
            "parametrized", 
            [](int& value) { std::cout << value << std::endl; }
        ).execute();
    }

    std::function<void()> func((myfunctor()));
    func();
    
    return 0;
}

Test("Something") << []() {
};

Skip() << Test("something") << Parametrized<int>({1,2 3}) << [](const int& value) {
};

SkipIf([](){return true;}) << Test("Something") << []() {};

Fail() << Test("Something", []() {

};