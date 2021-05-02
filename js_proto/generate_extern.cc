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

#include <iostream>

#include "absl/strings/str_cat.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_replace.h"
#include "absl/strings/str_split.h"
#include "glog/logging.h"
#include "google/protobuf/descriptor.h"

namespace js_proto {
using google::protobuf::Descriptor;
using google::protobuf::EnumDescriptor;
using google::protobuf::FieldDescriptor;
using google::protobuf::FileDescriptor;

//////////////////////////////////////////////////////////////////////////////
void ProtoJsonApi::ProcessEnum(const std::string& js_package,
                               const EnumDescriptor* enu) {
  LOG(INFO) << "ENUM " << enu->full_name();
  const int val_c = enu->value_count();
  for (int val_i = 0; val_i < val_c; val_i++) {
    auto val = enu->value(val_i);
    LOG(INFO) << "VALUE " << val->name() << " = " << val->number();
  }
}

template <class T>
static std::string JsTypeName(const T* des) {
  auto package = des->file()->package();
  auto js_package = absl::StrReplaceAll(package, {{".", "_"}});
  return absl::StrReplaceAll(des->full_name(), {{package, js_package}});
}

// Generate the type name for the given field.
std::string ProtoJsonApi::TypeString(const FieldDescriptor* field) {
  std::string type;
  switch (field->type()) {
    default:
      LOG(FATAL)  //
          << "Unknown field type: " << field->type_name() << " "
          << field->containing_type()->full_name() << "." << field->name();

    case FieldDescriptor::TYPE_INT32:
    case FieldDescriptor::TYPE_INT64:
    case FieldDescriptor::TYPE_UINT32:
    case FieldDescriptor::TYPE_UINT64:
    case FieldDescriptor::TYPE_SINT32:
    case FieldDescriptor::TYPE_SINT64:
    case FieldDescriptor::TYPE_FIXED32:
    case FieldDescriptor::TYPE_FIXED64:
    case FieldDescriptor::TYPE_SFIXED32:
    case FieldDescriptor::TYPE_SFIXED64:
    case FieldDescriptor::TYPE_DOUBLE:
    case FieldDescriptor::TYPE_FLOAT: {
      type = "number";
      break;
    }

    case FieldDescriptor::TYPE_BOOL: {
      type = "boolean";
      break;
    }

    case FieldDescriptor::TYPE_STRING:
    case FieldDescriptor::TYPE_BYTES: {
      type = "string";
      break;
    }

    case FieldDescriptor::TYPE_MESSAGE: {
      if (field->file()->name() != field->message_type()->file()->name()) {
        LOG(INFO) << "External: " << field->message_type()->full_name() << " @"
                  << field->message_type()->file()->name();
        requiered_.insert(field->message_type()->full_name());
      }
      type = JsTypeName(field->message_type());
      break;
    }

    case FieldDescriptor::TYPE_ENUM: {
      type = field->type_name();
      break;
    }
  }

  switch (field->label()) {
    case FieldDescriptor::LABEL_REQUIRED: {
      type = absl::StrCat("!", type);
      break;
    }

    case FieldDescriptor::LABEL_REPEATED: {
      type = absl::StrCat("?Array<!", type, ">");
      break;
    }

    case FieldDescriptor::LABEL_OPTIONAL: {
      type = absl::StrCat("?", type);
      break;
    }
  }

  return type;
}

//////////////////////////////////////////////////////////////////////////////
void ProtoJsonApi::ProcessMessage(const std::string& js_package,
                                  const Descriptor* msg) {
  LOG(INFO) << "MSG " << msg->full_name();

  blobs_.emplace_back(
      absl::StrCat(JsTypeName(msg), " = class {\n  constructor() {"));

  const int field_c = msg->field_count();
  for (int field_i = 0; field_i < field_c; field_i++) {
    auto* field = msg->field(field_i);

    blobs_.emplace_back(
        absl::StrCat("    /** @type {", TypeString(field), "} */"));
    blobs_.emplace_back(
        absl::StrCat("    this.", field->name(), ";  // ", field->type_name()));
  }

  blobs_.emplace_back("  }\n};\n");

  ////
  const int msg_c = msg->nested_type_count();
  for (int msg_i = 0; msg_i < msg_c; msg_i++) {
    ProcessMessage(js_package, msg->nested_type(msg_i));
  }

  const int enu_c = msg->enum_type_count();
  for (int enu_i = 0; enu_i < enu_c; enu_i++) {
    ProcessEnum(js_package, msg->enum_type(enu_i));
  }
}

//////////////////////////////////////////////////////////////////////////////
void ProtoJsonApi::ProcessFile(const FileDescriptor* file) {
  LOG(INFO) << "FILE " << file->name() << " " << file->package();
  blobs_.emplace_back(absl::StrCat("// Generated from ", file->name(), "\n"));

  // Declare namespace
  auto js_package = absl::StrReplaceAll(file->package(), {{".", "_"}});
  blobs_.emplace_back(absl::StrCat("const ", js_package, " = {};"));

  blobs_.emplace_back("");

  ////
  const int msg_c = file->message_type_count();
  for (int msg_i = 0; msg_i < msg_c; msg_i++) {
    ProcessMessage(js_package, file->message_type(msg_i));
  }

  const int enu_c = file->enum_type_count();
  for (int enu_i = 0; enu_i < enu_c; enu_i++) {
    ProcessEnum(js_package, file->enum_type(enu_i));
  }
}

//////////////////////////////////////////////////////////////////////////////
std::string ProtoJsonApi::ToString() {
  constexpr auto intro = R"JS(/**
 * @fileoverview Externs for JSON
 * @externs
 * @public
 */
/* eslint-disable */

// GENERATED CODE -- DO NOT EDIT!
)JS";
  constexpr auto outtro = R"JS(
// done
)JS";

  return absl::StrCat(intro, absl::StrJoin(blobs_, "\n"), outtro);
}

}  // namespace js_proto