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
#include <set>
#include <vector>

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

// If the derived class constructors need to take arguments, then the base class
// needs to define a nested type alias of std::tuple<...> called MakeArgs
// holding the types to forward to each derived class. When Base::MakeArgs is
// not empty, *only* Registrar::Make is usable and Registrar::GetDefault is
// disabled.

/*
struct AbstractBaseClass { using MakeArgs = std::tuple<std::string>; };

namespace {
struct ConcreteDerivedClass : public AbstractBaseClass {
 public:
  ConcreteDerivedClass(std::string);
  ConcreteDerivedClass() = delete;
};
Register<AbstractBaseClass, ConcreteDerivedClass> unique_name;
}  // namespace

auto made = Registrar<AbstractBaseClass>::Make("Hello world");
*/

// If the base class defines a type alias called RegistrationKeyType then
// MakeOnly(k, ...) may be used like Make(). Derived classes must define a
// constexpr static Base::RegistrationKeyType member named RegistrationKey and
// MakeOnly(k, ...) will only construct types where k==Derived::RegistrationKey.

template <class Base>
class Registrar {
  // For SFINAE
  template<class T> struct mix { using t = Base; };

  // A base class without a virtual destructor is a huge foot-gun.
  // Just forbid it and force people to deal with it.
  static_assert(std::has_virtual_destructor<Base>::value);

 public:
  // Construct one new instance of each type registered.
  // Ownership is transferred to the called.
  template<class... C>
  static std::vector<std::unique_ptr<Base>> Make(const C &...c) {
    auto *r = Get();

    std::vector<std::unique_ptr<Base>> ret;
    ret.reserve(r->factories_.size());
    for (const auto& f : r->factories_) ret.emplace_back(f.fn(c...));
    return ret;
  }

  template<class F, class... C>
  static std::vector<std::unique_ptr<Base>> MakeOnly(const F &k, const C &...c) {
    static_assert(sizeof(typename Base::RegistrationKeyType) >= 0);

    auto *r = Get();

    std::vector<std::unique_ptr<Base>> ret;
    ret.reserve(r->factories_.size());
    for (const auto& f : r->factories_) {
      if (k != f.key) continue;
      ret.emplace_back(f.fn(c...));
    }
    return ret;
  }

  template<class B = Base, class K = typename B::RegistrationKeyType>
  static std::set<K> GetKeys() {
    static_assert(std::is_same<B, Base>::value);
    static_assert(std::is_same<K, typename B::RegistrationKeyType>::value);

    std::set<K> ret;
    for (const auto& f : Get()->factories_) ret.emplace(f.key);
    return ret;
  }

  // Return a default set of instances.
  // These are on on-demand constructed once
  // per process and ownership is retained.
  static const std::vector<Base*>& GetDefault() {
    static_assert(std::is_same<
            typename has_make::type,
            std::function<std::unique_ptr<Base>()>>::value,
        "Registrar<B>::GetDefault() disabled where B::MakeArgs is not empty.");
    // grab this only the first time.
    static auto *hold = new auto{Make()};
    static auto *ret = [] {
      auto *x = new std::vector<Base*>;
      x->reserve(hold->size());
      for(const auto &i : *hold) x->push_back(i.get());
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

  template<class t>
  struct has_make_t {
    template<class... c>
    static std::function<std::unique_ptr<Base>(c...)> convert(std::tuple<c...>);
    using type = decltype(convert(std::declval<t>()));
  };

  // SFINAE magic to extract the arguments.
  template<class C>
  static has_make_t<typename C::MakeArgs> HasMake(typename C::MakeArgs*);
  template<class C>
  static has_make_t<std::tuple<>> HasMake(...);
  using has_make = decltype(HasMake<Base>(nullptr));

  struct Factory {
    // Compute all the key stuff
    template<class T, class V = typename mix<T>::t::RegistrationKeyType>
    static V MakeKey(T*) { return static_cast<V>(T::RegistrationKey); }
    static char MakeKey(...) { return 0; }
    using key_type = decltype(MakeKey(std::declval<Base*>()));

    typename has_make::type fn;
    key_type key;

    Factory(typename has_make::type f, key_type k)
        : fn(std::move(f)),
          key(k) {}
  };

  std::list<Factory> factories_;
};

template <class Base, class Derived>
struct Register {
  Register() {
    static auto _ = [] {  // Do this only once per type.
      auto *r = Registrar<Base>::Get();
      using BF = typename Registrar<Base>::Factory;
      r->factories_.emplace_back(BF([](const auto&... ts) {
        return std::unique_ptr<Base>{new Derived{ts...}};
      }, BF::MakeKey(static_cast<Derived*>(nullptr))));
      return nullptr;
    }();
    (void)_;  // Ignore unused.
  }
};

}  // namespace registration

#endif  // REGISTRATION_REGISTRATION_H_
