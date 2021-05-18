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

def _extract_proto_descriptor(ctx):
    ret = ctx.actions.declare_file(ctx.attr.out)

    ctx.actions.run(
        inputs = depset(ctx.attr.cc_proto_library.proto.srcs + ctx.attr.cc_proto_library.proto.deps.to_list()),
        tools = [ctx.executable.protoc],
        outputs = [ret],
        arguments = [
            "--include_source_info",
            "--descriptor_set_out=" + ret.path,
            ctx.attr.cc_proto_library.proto.srcs[0].path,
        ],
        executable = ctx.executable.protoc,
        mnemonic = "ProtoCompile",
    )

    return [
        DefaultInfo(files = depset([ret])),
    ]

extract_proto_descriptor = rule(
    implementation = _extract_proto_descriptor,
    attrs = {
        "out": attr.string(mandatory = True),
        "cc_proto_library": attr.label(mandatory = True),
        "protoc": attr.label(
            cfg = "host",
            executable = True,
            allow_single_file = True,
            default = "@com_google_protobuf//:protoc",
        ),
    },
)

def _js_proto_generate_extern(ctx):
    ret = ctx.actions.declare_file(ctx.attr.out)
    ctx.actions.run(
        inputs = [ctx.file.descriptor_pb],
        outputs = [ret],
        executable = ctx.executable.tool,
        arguments = [
            "--descriptor_file_name=%s" % ctx.file.descriptor_pb.path,
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
        "descriptor_pb": attr.label(
            mandatory = True,
            allow_single_file = True,
        ),
    },
)

def js_proto_extern(name = None, cc_proto_library = None, testonly = False):
    """Generate a closure externs file from a cc_proto_library rule.

    Args:
      name: The target name.
      cc_proto_library: a cc_proto_library lable to build from.
      testonly: testonly
    """
    ext = name + "_generator_ext"
    desc = name + ".pb.desc"
    gen = name + "_generate_js"

    extract_proto_descriptor(
        name = ext,
        testonly = testonly,
        out = desc,
        # See https://github.com/protocolbuffers/protobuf/blob/master/protobuf.bzl#L299
        cc_proto_library = cc_proto_library + "_genproto",
    )

    js_proto_generate_extern(
        name = gen,
        testonly = testonly,
        out = name + ".pb.js",
        tool = "@com_github_bcsgh_utilities//js_proto:generate",
        descriptor_pb = ":" + ext,
    )

    closure_js_library(
        name = name,
        srcs = [":%s" % gen],
        testonly = testonly,
    )
