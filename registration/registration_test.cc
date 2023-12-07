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

#include "registration/registration.h"

#include <set>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "registration/registration_use.h"

namespace registration {
namespace {
using testing::SizeIs;
using testing::UnorderedElementsAre;

struct D9 : public B {
  int F() override { return 9; }
};

Register<B, D9> b_d9_test;
Register<B, D9> b_d9_dup;

TEST(RegistrationTest, Basic) {
  auto made = Registrar<B>::Make();
  EXPECT_THAT(made, SizeIs(2));

  std::set<int> v;
  for (const auto& i : made) v.insert(i->F());
  EXPECT_THAT(v, SizeIs(2));
  EXPECT_THAT(v, UnorderedElementsAre(6, 9));
}

struct BX { virtual void F() = 0; };
struct DX : public BX { void F() override {} };
Register<BX, DX> bx_dx_test;

TEST(RegistrationTest, Other) {
  auto made = Registrar<BX>::GetDefault();
  EXPECT_THAT(made, SizeIs(1));
}

struct BArg {
  using MakeArgs = std::tuple<int>;
  virtual int F() = 0;
};

struct DArg : public BArg {
  DArg(int i) : i_(i) {}
  int F() override { return i_; }
  int i_;
};
Register<BArg, DArg> barg_darg_test;

TEST(RegistrationTest, Arg) {
  auto made = Registrar<BArg>::Make(1);
  EXPECT_THAT(made, SizeIs(1));
}

struct BY {
  virtual int F() = 0;
  typedef int RegistrationKeyType;
};

template<int i, int y = 0>
struct DY : public BY {
  static constexpr int RegistrationKey = i;
  int F() override { return i + y; }
};
Register<BY, DY<1> > by_dy1_test;
Register<BY, DY<2> > by_dy2_test;
Register<BY, DY<2, 2> > by_dy22_test;

TEST(RegistrationTest, Filter) {
  EXPECT_THAT(Registrar<BY>::Make(), SizeIs(3));

  auto v0 = Registrar<BY>::MakeOnly(0);
  auto v1 = Registrar<BY>::MakeOnly(1);
  auto v2 = Registrar<BY>::MakeOnly(2);
  EXPECT_THAT(v0, SizeIs(0));
  EXPECT_THAT(v1, SizeIs(1));
  EXPECT_THAT(v2, SizeIs(2));
}
}  // namespace
}  // namespace registration
