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

#include "http/content_type.h"

#include <map>

#include "absl/strings/string_view.h"

namespace http_utils {

absl::string_view GetContentType(absl::string_view path) {
  // from:
  // https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Complete_list_of_MIME_types
  static auto const *const lookup =
      new std::map<absl::string_view, absl::string_view>{
          {"bmp", content_type::kImageBmp},
          {"css", content_type::kTextCss},
          {"csv", content_type::kTextCsv},
          {"gif", content_type::kImageGif},
          {"htm", content_type::kTextHtml},
          {"html", content_type::kTextHtml},
          {"ico", content_type::kImageIcon},
          {"jpg", content_type::kImageJpeg},
          {"jpeg", content_type::kImageJpeg},
          {"js", content_type::kTextJavaScript},
          {"json", content_type::kApplicationJson},
          {"mjs", content_type::kTextJavaScript},
          {"pdf", content_type::kApplicationPdf},
          {"png", content_type::kImagePng},
          {"svg", content_type::kImageSvg},
          {"txt", content_type::kTextPlain},
          {"xml", content_type::kApplicationXml},
      };

  auto a = path.rfind('.');
  if (a == absl::string_view::npos) return content_type::kTextPlain;

  auto ext = path.substr(a + 1);
  auto type = lookup->find(ext);
  if (type != lookup->end()) return type->second;

  return content_type::kTextPlain;
}

}  // namespace http_utils
