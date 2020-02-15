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

#include <string>

#include "absl/strings/str_cat.h"
#include "absl/strings/string_view.h"
#include "gtest/gtest.h"

namespace http_utils {
namespace {

struct Case {
  std::string path;
  std::string type;

  friend std::ostream& operator<<(std::ostream& os, const Case& c) {
    return os << "{path='" << c.path << "' type='" << c.type << "'}";
  }
};

class ContentTypeTest : public ::testing::TestWithParam<Case> {};

INSTANTIATE_TEST_SUITE_P(
    Cases, ContentTypeTest,
    ::testing::Values(Case{"a.bmp", content_type::kImageBmp},
                      Case{"b0.css", content_type::kTextCss},
                      Case{"c.csv", content_type::kTextCsv},
                      Case{"d1.gif", content_type::kImageGif},
                      Case{"e.htm", content_type::kTextHtml},
                      Case{"f2.html", content_type::kTextHtml},
                      Case{"g.ico", content_type::kImageIcon},
                      Case{"h3.jpg", content_type::kImageJpeg},
                      Case{"i.jpeg", content_type::kImageJpeg},
                      Case{"j4.js", content_type::kTextJavaScript},
                      Case{"k.json", content_type::kApplicationJson},
                      Case{"l5.mjs", content_type::kTextJavaScript},
                      Case{"m.pdf", content_type::kApplicationPdf},
                      Case{"n6.png", content_type::kImagePng},
                      Case{"o.svg", content_type::kImageSvg},
                      Case{"p7.txt", content_type::kTextPlain},
                      Case{"file.unknown", content_type::kTextPlain},
                      Case{"hello_world", content_type::kTextPlain}),
    [](const testing::TestParamInfo<Case>& c) {
      std::string ret;
      for (const auto c : c.param.path) {
        if (('0' <= c && c <= '9') ||  //
            ('A' <= c && c <= 'Z') ||  //
            ('a' <= c && c <= 'z')) {
          ret.push_back(c);
        }
      }
      return ret;
    });

TEST_P(ContentTypeTest, Basic) {
  const auto param = GetParam();
  EXPECT_EQ(std::string{GetContentType(param.path)}, param.type);
}

TEST_P(ContentTypeTest, Rooted) {
  const auto param = GetParam();
  auto path = absl::StrCat("/", param.path);
  EXPECT_EQ(std::string{GetContentType(path)}, param.type) << path;
}

TEST_P(ContentTypeTest, SubDir) {
  const auto param = GetParam();
  auto path = absl::StrCat("foo/", param.path);
  EXPECT_EQ(std::string{GetContentType(path)}, param.type) << path;
}

TEST_P(ContentTypeTest, RootedSubDir) {
  const auto param = GetParam();
  auto path = absl::StrCat("/bar/", param.path);
  EXPECT_EQ(std::string{GetContentType(path)}, param.type) << path;
}

}  // namespace
}  // namespace http_utils
