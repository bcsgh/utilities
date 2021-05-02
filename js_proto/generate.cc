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

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "glog/logging.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/descriptor.pb.h"
#include "google/protobuf/descriptor_database.h"
#include "js_proto/generate_extern.h"

ABSL_FLAG(std::string, proto_file_name, "",
          "The name of the file to generated for. "
          "The file must be linked into the binary.");
ABSL_FLAG(std::string, descriptor_file_name, "",
          "The name of the file contaning a binary FileDescriptor proto.");
ABSL_FLAG(std::string, output, "",  //
          "The name of the file to output to.");

using google::protobuf::DescriptorPool;
using google::protobuf::FileDescriptorSet;
using google::protobuf::SimpleDescriptorDatabase;

int main(int argc, char** argv) {
  auto args = absl::ParseCommandLine(argc, argv);
  google::InitGoogleLogging(args[0]);

  const auto* pool = google::protobuf::DescriptorPool::generated_pool();
  std::string file_name = absl::GetFlag(FLAGS_proto_file_name);

  // Allow these to hang around
  std::unique_ptr<SimpleDescriptorDatabase> load_db;
  std::unique_ptr<DescriptorPool> load_pool;

  // If we are given descriptor_file_name, use that.
  if (!absl::GetFlag(FLAGS_descriptor_file_name).empty()) {
    std::ifstream istr;
    istr.open(absl::GetFlag(FLAGS_descriptor_file_name));
    if (!istr.is_open()) {
      std::cerr << "Error opening '" << absl::GetFlag(FLAGS_descriptor_file_name) << "': " << strerror(errno) << "\n";
      return 4;
    }

    FileDescriptorSet file_desc;
    if (!file_desc.ParseFromIstream(&istr)) {
      std::cerr << "Failed to parse proto from '" << absl::GetFlag(FLAGS_descriptor_file_name) << "'\n";
      return 5;
    }
    if (file_desc.file_size() != 1) {
      std::cerr << "Exactly one file must be provided, found " << file_desc.file_size() << "\n";
      for (const auto& f : file_desc.file()) std::cerr << f.name() << "\n";
      return 6;
    }

    file_name = file_desc.file(0).name();

    load_db = std::make_unique<SimpleDescriptorDatabase>();
    load_db->Add(file_desc.file(0));

    load_pool = std::make_unique<DescriptorPool>(load_db.get());
    pool = load_pool.get();
  }

  if (!pool || file_name.empty()) {
    std::cerr << "Internal error\n";
    return 1;
  }
  auto file = pool->FindFileByName(file_name);

  if (!file) {
    std::cerr << "Failed to find file: '"  //
              << absl::GetFlag(FLAGS_proto_file_name) << "'\n";

    /*  A hack to see what files exist
    std::vector<std::string> all;
    google::protobuf::DescriptorPool::internal_generated_database()->FindAllFileNames(&all);
    for (const auto& f : all) std::cerr << f << "\n";
    // */

    return 2;
  }

  js_proto::ProtoJsonApi process;
  process.ProcessFile(file);

  if (absl::GetFlag(FLAGS_output).empty()) {
    std::cout << process.ToString() << std::flush;
  } else {
    std::ofstream ostr;
    ostr.open(absl::GetFlag(FLAGS_output));
    if (!ostr.is_open()) {
      std::cerr  //
          << "Failed to open file: '" << absl::GetFlag(FLAGS_output) << "'";
      return 3;
    }

    ostr << process.ToString() << std::flush;
    ostr.close();
  }

  return 0;
}