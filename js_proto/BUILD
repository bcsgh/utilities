load("@google_protobuf//:protobuf.bzl", "cc_proto_library")
load("@io_bazel_rules_closure//closure:defs.bzl", "closure_js_library", "closure_js_test")
load(":js_proto.bzl", "js_proto_extern")

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

cc_binary(
    name = "generate",
    visibility = ["//visibility:public"],
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
)

########################
js_proto_extern(
    name = "generate_example_js",
    cc_proto_library = ":example_proto",
    testonly = True,
)

js_proto_extern(
    name = "generate_example2_js",
    cc_proto_library = ":example2_proto",
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