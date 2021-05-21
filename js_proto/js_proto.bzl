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
Generate a JS closure externs file from a proto_library rule.
"""

load("@io_bazel_rules_closure//closure:defs.bzl", "closure_js_library")

def _extract_proto_descriptor(ctx):
    ret = ctx.actions.declare_file(ctx.attr.out)

    pi = ctx.attr.proto_library[ProtoInfo]
    ctx.actions.run(
        inputs = depset(pi.direct_sources + pi.transitive_imports.to_list()),
        tools = [ctx.executable.protoc],
        outputs = [ret],
        arguments = [
            "-I%s" % i
            for i in pi.transitive_proto_path.to_list()
        ] + [
            "--include_source_info",
            "--descriptor_set_out=" + ret.path,
            pi.direct_sources[0].path,
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
        "proto_library": attr.label(mandatory = True),
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

def js_proto_extern(name = None, proto_library = None, testonly = False):
    """Generate a closure externs file from a proto_library rule.

    Args:
      name: The target name.
      proto_library: a proto_library lable to build from.
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
        proto_library = proto_library,
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
