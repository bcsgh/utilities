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

#ifndef HTTP_CONTENT_TYPE_H_
#define HTTP_CONTENT_TYPE_H_

#include "absl/strings/string_view.h"

namespace http_utils {

// Lookup a sutable Content-Type for a file given a path.
// This is based only on the name of the file.
//
// The returned string_view has unrestricted lifetime
// (it referse to static data).
absl::string_view GetContentType(absl::string_view path);

namespace content_type {

constexpr char kApplicationJson[] = "application/json";
constexpr char kApplicationPdf[] = "application/pdf";
constexpr char kApplicationXml[] = "application/xml";
constexpr char kImageBmp[] = "image/bmp";
constexpr char kImageGif[] = "image/gif";
constexpr char kImageIcon[] = "image/vnd.microsoft.icon";
constexpr char kImageJpeg[] = "image/jpeg";
constexpr char kImagePng[] = "image/png";
constexpr char kImageSvg[] = "image/svg+xml";
constexpr char kTextCss[] = "text/css";
constexpr char kTextCsv[] = "text/csv";
constexpr char kTextHtml[] = "text/html";
constexpr char kTextJavaScript[] = "text/javascript";
constexpr char kTextPlain[] = "text/plain";

}  // namespace content_type
}  // namespace http_utils

#endif  // HTTP_CONTENT_TYPE_H_
