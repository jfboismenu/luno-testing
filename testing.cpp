// MIT License
//
// Copyright (c) 2020 Jean-François Boismenu
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <array>
#include <iostream>
#include <luno-testing/fixture.hpp>
#include <luno-testing/parametrized.hpp>
#include <luno-testing/predicates.hpp>
#include <luno-testing/test.hpp>
#include <vector>
using namespace luno::testing;

struct myfunctor {
    void operator()() {
        std::cout << "from myfunctor" << std::endl;
    }
};

int main(int argc, char **argv) {
    luno_expect(And(1, 2));
    luno_expect(0);
    luno_expect(And(0, 2));

    {
        Fixture<int> fixture([]() { return 1; },
                             [](int &integer) { std::cout << integer << std::endl; });

        fixture() = 3;
        std::cout << "end of scope" << std::endl;
    }
    {
        Fixture<int> fixture([]() { return 1; },
                             [](int &integer) { std::cout << integer << std::endl; });

        std::cout << "Testing test that modifies fixture value" << std::endl;

        Test("something", [fixture]() {
            std::cout << fixture() << std::endl;
            fixture() = 3;
        }).execute();

        std::cout << "Testing test that follows test that modified value" << std::endl;
        Test("something_else", [fixture]() { std::cout << fixture() << std::endl; }).execute();
    }
    {
        std::cout << "Testing fixture with constant." << std::endl;
        Fixture<int> fixture(2);
        Test("someting_else", [fixture]() { std::cout << fixture() << std::endl; }).execute();
    }

    {
        std::cout << "Testing fixture with constant array." << std::endl;
        Fixture<std::array<int, 4>> fixture({1, 2, 3, 4});
        Test("someting_else", [fixture]() {
            for (const auto &value : fixture()) {
                std::cout << value << std::endl;
            }
        }).execute();
    }

    {
        std::cout << "Testing fixture with constant vector." << std::endl;
        Fixture<std::vector<int>> fixture({1, 2, 3, 4});
        Test("someting_else", [fixture]() {
            for (const auto &value : fixture()) {
                std::cout << value << std::endl;
            }
        }).execute();
    }
    std::function<void()> func((myfunctor()));
    func();

    return 0;
}
