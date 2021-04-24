// Copyright (c) 2021, Benjamin Shropshire,
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. Neither the name of the copyright holder nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef REGISTRATION_REGISTRATION_H_
#define REGISTRATION_REGISTRATION_H_

#include <functional>
#include <list>
#include <memory>

namespace registration {

// This library provide a mean to collect instances of classes derived from a
// given base class. A distinct collection is maintained per base class. The
// items included in the collection can be defined across multiple translation
// units (i.e. libraries). This library has no initialization time dependences
// other than the availability of the standard library.

// Example Usage:
/*
// Define the abstract type to register.
struct AbstractBaseClass { ... };

// Define and register specific examples.
// (These should by default use static linkage and the `Register<>` must be at
// global scope. Both of these can be assured by placing both the type and the
// `Register<>` in an anon namespace.)
namespace {
struct ConcreteDerivedClass : public AbstractBaseClass { ... };
Register<AbstractBaseClass, ConcreteDerivedClass> unique_name;
}  // namespace


// Get a collection of object instances.
std::vector<std::unique_ptr<AbstractBaseClass>> made =
    Registrar<AbstractBaseClass>::Make();
*/

template <class Base>
class Registrar {
 public:
  // Construct one new instance of each type registered.
  // Ownership is transferred to the called.
  static std::vector<std::unique_ptr<Base>> Make() {
    auto *r = Get();

    std::vector<std::unique_ptr<Base>> ret;
    ret.reserve(r->factories_.size());
    for (const auto& f : r->factories_) ret.emplace_back(f());
    return ret;
  }

  // Return a default set of instances.
  // These are on on-demand constructed once
  // per process and ownership is retained.
  static const std::vector<Base>& GetDefault() {
    // grab this only the first time.
    static auto *hold = new auto{Make()};
    static auto *ret = [] {
      auto *x = new std::vector<Base>;
      x->reserve(hold->size());
      for(const auto i : *hold) x.push_back(&i);
      return x;
    }();
    return *ret;
  }

 private:
  Registrar() = default;

  // On-demand construct the global static instance.
  static Registrar* Get() {
    static auto *ret = new Registrar;
    return ret;
  }

  template<class, class> friend struct Register;

  std::list<std::function<std::unique_ptr<Base>()>> factories_;
};

template <class Base, class Derived>
struct Register {
  Register() {
    static auto _ = [] {  // Do this only once per type.
      auto *r = Registrar<Base>::Get();
      r->factories_.emplace_back([] {
        return std::unique_ptr<Base>{new Derived};
      });
      return nullptr;
    }();
    (void)_;  // Ignore unused.
  }
};

}  // namespace registration

#endif  // REGISTRATION_REGISTRATION_H_
