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

#include <sstream>

#pragma once

class Predicate {
public:
    virtual operator bool() const = 0;
};

template <typename Left, typename Right> class BinaryPredicate : public Predicate {
public:
    BinaryPredicate(const Left &left, const Right &right) : _left(left), _right(right) {
    }

    virtual std::string name() const = 0;

    const Left &left() const {
        return _left;
    }
    const Right &right() const {
        return _right;
    }

private:
    const Left &_left;
    const Right &_right;
};

template <typename Left, typename Right> class AndT : public BinaryPredicate<Left, Right> {
public:
    AndT(const Left &left, const Right &right) : BinaryPredicate<Left, Right>(left, right) {
    }

    std::string name() const override {
        return "&&";
    }

    operator bool() const override {
        return static_cast<bool>(this->left()) && static_cast<bool>(this->right());
    }
};

template <typename Left, typename Right>
auto And(const Left &left, const Right &right) -> AndT<Left, Right> {
    return AndT<Left, Right>(left, right);
}

template <typename Left, typename Right>
std::ostream &operator<<(std::ostream &os, const BinaryPredicate<Left, Right> &pred) {
    os << "( " << pred.left() << " " << pred.name() << " " << pred.right() << " )";
    return os;
}

namespace details {
template <typename Left, typename Right>
void expect(const BinaryPredicate<Left, Right> &pred, const char *filename, int line_no) {
    if (!pred) {
        std::ostringstream os;
        os << "Predicate failure in " << filename << " at line " << line_no << ": " << pred;
        throw std::runtime_error(os.str());
    }
}
} // namespace details

#define luno_expect(pred) details::expect(pred, __FILE__, __LINE__)
