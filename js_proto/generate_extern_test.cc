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

#include "js_proto/generate_extern.h"

#include "absl/log/log.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_split.h"
#include "gmock/gmock.h"
#include "google/protobuf/descriptor.h"
#include "gtest/gtest.h"

namespace js_proto {
namespace {
using testing::HasSubstr;

TEST(JsProto, Basic) {
  const auto* pool = google::protobuf::DescriptorPool::generated_pool();

  auto file_name = "js_proto/example.proto";
  auto file = pool->FindFileByName(file_name);
  ASSERT_THAT(file, testing::NotNull());

  ProtoJsonApi process;
  process.ProcessFile(file);

  auto js_file = process.ToString();

  std::vector<std::string> parts = {
      R"JS(
 * @externs
)JS", R"JS(
 * @public
)JS", R"JS(
var js_proto_test = {};
)JS", R"JS(
js_proto_test.A = class {
  constructor() {
    /** @type {!js_proto_test.A.A_Inner} */
    this.inner_type;  // message
    /** @type {?js_proto_test.A.A_Inner.E_Inner} */
    this.e;  // enum
  }
};
)JS", R"JS(
js_proto_test.A.A_Inner = class {
  constructor() {
    /** @type {?number} */
    this.a;  // int32
    /** @type {?number} */
    this.b;  // int64
    /** @type {?number} */
    this.c;  // uint32
    /** @type {!number} */
    this.d;  // uint64
    /** @type {?number} */
    this.e;  // sint32
    /** @type {?number} */
    this.f;  // sint64
    /** @type {?number} */
    this.k;  // fixed32
    /** @type {?number} */
    this.l;  // fixed64
    /** @type {?number} */
    this.m;  // sfixed32
    /** @type {?number} */
    this.n;  // sfixed64
    /** @type {?Array<!string>} */
    this.g;  // string
    /** @type {?Array<!string>} */
    this.h;  // bytes
    /** @type {?Array<!number>} */
    this.i;  // float
    /** @type {?Array<!number>} */
    this.j;  // double
    /** @type {?Array<!boolean>} */
    this.o;  // bool
  }
};
)JS", R"JS(
/** @enum {number} */
js_proto_test.A.A_Inner.E_Inner = {
  I: 1,
  J: 2,
  K: 3,
};
)JS", R"JS(
js_proto_test.B = class {
  constructor() {
    /** @type {?js_proto_test.A.A_Inner} */
    this.ai;  // message
    /** @type {?js_proto_test.A} */
    this.a;  // message
    /** @type {?Array<!js_proto_test2.Imp>} */
    this.imp;  // message
  }
};
)JS", R"JS(
/** @enum {number} */
js_proto_test.E = {
  Z: 1,
  Y: 2,
  X: 3,
};
)JS", R"JS(
// done
)JS",
  };

  for (const auto& p : parts) EXPECT_THAT(js_file, HasSubstr(p));
}

}  // namespace
}  // namespace js_proto