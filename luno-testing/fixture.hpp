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

#include <functional>
#include <optional>

template <typename T> class Fixture {
public:
  Fixture(std::function<T()> pre) : Fixture(pre, [](T &) {}) {}

  Fixture(T &&value) : Fixture([value]() { return value; }) {}

  Fixture(std::function<T()> pre, std::function<void(T &)> post)
      : _pre(pre), _post(post) {}

  Fixture(const Fixture<T> &other) : Fixture(other._pre, other._post) {}

  ~Fixture() {
    if (_value) {
      _post(*_value);
    }
  }

  T &operator()() const {
    if (!_value) {
      _value = _pre();
    }
    return *_value;
  }

private:
  mutable std::optional<T> _value;
  std::function<T()> _pre;
  std::function<void(T &)> _post;
};
