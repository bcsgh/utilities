workspace(name = "com_github_bcsgh_utilities")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

git_repository(
    name = "rules_foreign_cc",
    commit = "c923238c6dc5a35c233a4acca28d90a0b1816836",  # current as of 2022/10/27
    remote = "https://github.com/bazelbuild/rules_foreign_cc.git",
    shallow_since = "1666910343 +0100",
)

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

rules_foreign_cc_dependencies([])

#############################################
# Bazel Skylib.
http_archive(
    name = "bazel_skylib",
    urls = ["https://github.com/bazelbuild/bazel-skylib/releases/download/1.2.1/bazel-skylib-1.2.1.tar.gz"],
    sha256 = "f7be3474d42aae265405a592bb7da8e171919d74c16f082a5457840f06054728",
)

git_repository(
    name = "rules_proto",
    commit = "f371ed34ed7f1a8b83cc34ae96db277e0ba4fcb0",  # tag=5.3.0-21.5 + f371ed3
    remote = "https://github.com/bazelbuild/rules_proto.git",
    shallow_since = "1664409030 -0700",
)

load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies")

rules_proto_dependencies()
#NOTE: might also need rules_proto_toolchains()?

# buildifier: disable=no-effect
"""
NOTE: might also need:
load("@rules_cc//cc:repositories.bzl", "rules_cc_dependencies")
rules_cc_dependencies()
"""

# https://github.com/bazelbuild/rules_closure
git_repository(
    name = "io_bazel_rules_closure",
    remote = "https://github.com/bazelbuild/rules_closure.git",
    commit = "f19dc96c1dad6990b67dc3a35818cde5b7042bbb",  # https://github.com/bazelbuild/rules_closure/pull/597
    shallow_since = "1697673471 -0700",
)

load("@io_bazel_rules_closure//closure:repositories.bzl", "rules_closure_dependencies", "rules_closure_toolchains")

rules_closure_dependencies()  # needed for @rules_python//...

rules_closure_toolchains()

#############################################
git_repository(
    name = "com_google_absl",
    commit = "483a2d59e649179ea9d9bc4d808f6c9d16646f9d",  # current as of 2023/11/12
    remote = "https://github.com/abseil/abseil-cpp.git",
    shallow_since = "1699496241 -0800",
)

#############################################
git_repository(
    name = "com_google_googletest",
    commit = "b10fad38c4026a29ea6561ab15fc4818170d1c10",  # current as of 2023/11/12
    remote = "https://github.com/google/googletest.git",
    shallow_since = "1698701593 -0700",
)

#############################################
