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

#ifndef HTTP_FILE_RESOURCE_H_
#define HTTP_FILE_RESOURCE_H_

#include <map>
#include <memory>
#include <set>

#include "absl/base/attributes.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/string_view.h"
#include "httpserver.hpp"

namespace bcs_http_util {
namespace internal {

ABSL_MUST_USE_RESULT
std::unique_ptr<httpserver::http_resource> Add(httpserver::webserver* ws,
                                               absl::string_view path,
                                               absl::string_view data);
ABSL_MUST_USE_RESULT
std::unique_ptr<httpserver::http_resource> Add(httpserver::webserver* ws,
                                               absl::string_view path,
                                               absl::string_view data,
                                               absl::string_view type);

struct MapHelper {
  template <class Files>
  MapHelper(httpserver::webserver* ws_, const Files& files)
      : ws(ws_), files_map(files.begin(), files.end()) {}

  void Add(absl::string_view uri, absl::string_view path);

  ~MapHelper();

  httpserver::webserver* ws;
  std::map<absl::string_view, absl::string_view> files_map;
  std::set<absl::string_view> missing;
  std::vector<std::unique_ptr<httpserver::http_resource>> ret;
};

}  // namespace internal

template <class Files>
std::vector<std::unique_ptr<httpserver::http_resource>> ExportFiles(
    httpserver::webserver* ws, absl::string_view prefix, const Files& files) {
  std::vector<std::unique_ptr<httpserver::http_resource>> ret;
  for (const auto& file : files) {
    ret.emplace_back(
        internal::Add(ws, absl::StrCat(prefix, file.first), file.second));
  }
  return ret;
}

template <class Mapping, class Files>
std::vector<std::unique_ptr<httpserver::http_resource>> ExportFiles(
    httpserver::webserver* ws, absl::string_view prefix, const Mapping& mapping,
    const Files& files) {
  internal::MapHelper helper(ws, files);
  for (const auto& m : mapping) {
    helper.Add(absl::StrCat(prefix, m.first), m.second);
  }
  return std::move(helper.ret);
}

}  // namespace bcs_http_util

#endif  // HTTP_FILE_RESOURCE_H_
