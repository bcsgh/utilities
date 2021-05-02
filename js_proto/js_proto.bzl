# Copyright (c) 2021, Benjamin Shropshire,
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
# 3. Neither the name of the copyright holder nor the names of its contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

"""
Generate a closure externs file from a cc_proto_library rule.
"""

load("@io_bazel_rules_closure//closure:defs.bzl", "closure_js_library")

def _js_proto_generate_extern(ctx):
  if len(ctx.attr.cc_proto_library.proto.srcs) != 1:
    fail("js_proto_extern requiers a cc_proto_library with exactly one source")

  ret = ctx.actions.declare_file(ctx.attr.out)
  ctx.actions.run(
    outputs = [ret],
    executable = ctx.executable.tool,
    arguments = [
      "--proto_file_name=%s" % ctx.attr.cc_proto_library.proto.srcs[0].path,
      "--output=%s" % ret.path,
    ],
    mnemonic = "PbJsGen",
  )
  return [
    DefaultInfo(files = depset([ret])),
  ]

js_proto_generate_extern = rule(
  implementation = _js_proto_generate_extern,
  attrs = {
    "tool": attr.label(
        mandatory = True,
        cfg = "host",
        executable = True,
    ),
    "out": attr.string(mandatory = True),
    "cc_proto_library": attr.label(mandatory = True),
  }
)

def js_proto_extern(name = None, cc_proto_library = None, testonly = False):
    """Generate a closure externs file from a cc_proto_library rule.

    Args:
      name: The target name.
      cc_proto_library: a cc_proto_library lable to build from.
      testonly: testonly
    """
    bin = name + "_generator_bin"
    gen = name + "_generate_js"

    native.cc_binary(
        name = bin,
        deps = [
            ":generate",
            cc_proto_library,
        ],
        testonly = testonly,
    )

    js_proto_generate_extern(
        name = gen,
        testonly = testonly,
        out = name + ".pb.js",
        tool = ":%s" % bin,
        # See https://github.com/protocolbuffers/protobuf/blob/master/protobuf.bzl#L299
        cc_proto_library = cc_proto_library + "_genproto",
    )

    closure_js_library(
        name = name,
        srcs = [":%s" % gen],
        testonly = testonly,
    )

