// Copyright (c) 2020, Benjamin Shropshire,
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

#include "http/file_resource.h"

#include <set>

#include "absl/memory/memory.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_join.h"
#include "absl/strings/string_view.h"
#include "glog/logging.h"
#include "http/content_type.h"
#include "httpserver.hpp"

namespace bcs_http_util {
namespace internal {

void MapHelper::Add(absl::string_view uri, absl::string_view path) {
  auto i = files_map.find(path);
  if (i == files_map.end()) {
    missing.insert(path);
  } else {
    ret.emplace_back(
        internal::Add(ws, uri, i->second, http_utils::GetContentType(path)));
  }
}

MapHelper::~MapHelper() {
  if (missing.empty()) return;

  std::set<absl::string_view> have;
  for (const auto &f : files_map) have.insert(f.first);
  LOG(WARNING) << "Unknown targets '" << absl::StrJoin(missing, "', '")
               << "', Known targets '" << absl::StrJoin(have, "', '") << "'";
}

namespace {

class StringResource : public httpserver::http_resource {
 public:
  StringResource(absl::string_view data, absl::string_view type)
      : httpserver::http_resource(), data_(data), type_(type) {}

 private:
  const std::shared_ptr<httpserver::http_response> render(
      const httpserver::http_request &req) override {
    auto ret =
        absl::make_unique<httpserver::string_response>(std::string{data_});
    ret->with_header("Content-Type", std::string{type_});
    return ret;
  }

  absl::string_view data_;
  absl::string_view type_;
};

}  // namespace

std::unique_ptr<httpserver::http_resource> Add(  //
    httpserver::webserver *ws, absl::string_view path, absl::string_view data) {
  return Add(ws, path, data, http_utils::GetContentType(path));
}

std::unique_ptr<httpserver::http_resource> Add(  //
    httpserver::webserver *ws, absl::string_view path, absl::string_view data,
    absl::string_view type) {
  LOG(INFO) << path << ", " << type << ", " << data.size() << "B";
  auto ret = absl::make_unique<StringResource>(data, type);
  ws->register_resource(std::string{path}, ret.get());
  return ret;
}

}  // namespace internal
}  // namespace bcs_http_util
