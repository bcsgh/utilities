load("@google_protobuf//:protobuf.bzl", "cc_proto_library")
load("@io_bazel_rules_closure//closure:defs.bzl", "closure_js_library", "closure_js_test")

cc_library(
    name = "generate_extern",
    srcs = ["generate_extern.cc"],
    hdrs = ["generate_extern.h"],
    deps = [
        "@com_github_glog_glog//:glog",
        "@com_google_absl//absl/strings",
        "@google_protobuf//:protobuf",
    ],
)

cc_library(
    name = "generate",
    srcs = ["generate.cc"],
    deps = [
        ":generate_extern",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@google_protobuf//:protobuf",
    ],
)

########################
cc_proto_library(
    name = "example2_proto",
    srcs = ["example2.proto"],
    protoc = "@google_protobuf//:protoc",
    default_runtime = "@google_protobuf//:protobuf",
    testonly = True,
)

cc_proto_library(
    name = "example_proto",
    srcs = ["example.proto"],
    protoc = "@google_protobuf//:protoc",
    default_runtime = "@google_protobuf//:protobuf",
    deps = [":example2_proto"],
    testonly = True,
    alwayslink = True,
)

########################
cc_binary(
    name = "generate_example_bin",
    deps = [
        ":generate",
        ":example_proto",
        ":example2_proto",
    ],
    testonly = True,
)

genrule(
    name = "generate_example",
    testonly = True,
    tools = [":generate_example_bin"],
    outs = ["example.pb.js"],
    cmd = "$(location :generate_example_bin) --proto_file_name=js_proto/example.proto > $@",
)

genrule(
    name = "generate_example2",
    testonly = True,
    tools = [":generate_example_bin"],
    outs = ["example2.pb.js"],
    cmd = "$(location :generate_example_bin) --proto_file_name=js_proto/example2.proto > $@",
)

closure_js_library(
    name = "generate_example_js",
    srcs = ["example.gold.js"],
    deps = [":generate_example2_js"],
    testonly = True,
)

closure_js_library(
    name = "generate_example2_js",
    srcs = ["example2.gold.js"],
    testonly = True,
)

########################
cc_library(
    name = "test_main",
    srcs = ["test_main.cc"],
    deps = [
        "@com_github_glog_glog//:glog",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/time",
        "@com_google_googletest//:gtest",
    ],
)

cc_test(
    name = "generate_extern_test",
    srcs = ["generate_extern_test.cc"],
    deps = [
        ":example_proto",
        ":generate_extern",
        ":test_main",
        "@com_google_googletest//:gtest",
    ],
)

closure_js_test(
    name = "generate_example_test",
    srcs = ["generate_example_test.js"],
    compilation_level = "ADVANCED",
    entry_points = ["js_proto/generate_example_test.js"],
    suppress = ["moduleLoad"],
    deps = [
        ":generate_example_js",
        ":generate_example2_js",
        "@io_bazel_rules_closure//closure/library/testing:asserts",
        "@io_bazel_rules_closure//closure/library/testing:jsunit",
        "@io_bazel_rules_closure//closure/library/testing:testsuite",
    ],
)