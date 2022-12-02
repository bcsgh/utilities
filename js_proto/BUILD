load("@rules_proto//proto:defs.bzl", "proto_library")
load("@io_bazel_rules_closure//closure:defs.bzl", "closure_js_test")
load(":js_proto.bzl", "js_proto_extern")

cc_proto_library(
    name = "descriptor_proto_cc",
    deps = ["@com_google_protobuf//:descriptor_proto"],
)

cc_library(
    name = "generate_extern",
    srcs = ["generate_extern.cc"],
    hdrs = ["generate_extern.h"],
    deps = [
        ":descriptor_proto_cc",
        "@com_google_absl//absl/log:log",
        "@com_google_absl//absl/strings",
    ],
)

cc_binary(
    name = "generate",
    srcs = ["generate.cc"],
    visibility = ["//visibility:public"],
    deps = [
        ":descriptor_proto_cc",
        ":generate_extern",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/log:initialize",
        "@com_google_absl//absl/log:log",
        "@com_google_absl//absl/memory",
    ],
)

########################
proto_library(
    name = "example2_proto",
    testonly = True,
    srcs = ["example2.proto"],
)

proto_library(
    name = "example_proto",
    testonly = True,
    srcs = ["example.proto"],
    deps = [":example2_proto"],
)

########################
js_proto_extern(
    name = "generate_example_js",
    testonly = True,
    proto_library = ":example_proto",
)

js_proto_extern(
    name = "generate_example2_js",
    testonly = True,
    proto_library = ":example2_proto",
)

########################
cc_library(
    name = "test_main",
    srcs = ["test_main.cc"],
    deps = [
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/log:initialize",
        "@com_google_absl//absl/log:log",
        "@com_google_absl//absl/time",
        "@com_google_googletest//:gtest",
    ],
)

cc_proto_library(
    name = "example_proto_cc",
    testonly = True,
    deps = [":example_proto"],
)

cc_test(
    name = "generate_extern_test",
    srcs = ["generate_extern_test.cc"],
    deps = [
        ":example_proto_cc",
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
        ":generate_example2_js",
        ":generate_example_js",
        "@io_bazel_rules_closure//closure/library/testing:asserts",
        "@io_bazel_rules_closure//closure/library/testing:jsunit",
        "@io_bazel_rules_closure//closure/library/testing:testsuite",
    ],
)
