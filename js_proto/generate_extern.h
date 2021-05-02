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

#ifndef JS_PROTO_GENERATE_EXTERN_H_
#define JS_PROTO_GENERATE_EXTERN_H_

#include <set>
#include <string>
#include <vector>

#include "google/protobuf/descriptor.h"

namespace js_proto {

class ProtoJsonApi {
 public:
  // Generate the type name for the given field.
  std::string TypeString(const google::protobuf::FieldDescriptor* field);

  void ProcessEnum(const std::string& js_package,
                   const google::protobuf::EnumDescriptor* enu);
  void ProcessMessage(const std::string& js_package,
                      const google::protobuf::Descriptor* msg);
  void ProcessFile(const google::protobuf::FileDescriptor* file);
  std::string ToString();

  std::set<std::string> requiered_;
  std::vector<std::string> blobs_;
};

}  // namespace js_proto

#endif  // JS_PROTO_GENERATE_EXTERN_H_