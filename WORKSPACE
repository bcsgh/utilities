workspace(name = "com_github_bcsgh_utilities")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "rules_foreign_cc",
    commit = "f76d9281bd7ae1f36179740ba20db3d58cd3b7a3",  # current as of 2021/12/17
    remote = "https://github.com/bazelbuild/rules_foreign_cc.git",
    shallow_since = "1639486112 +0000",
)

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

rules_foreign_cc_dependencies([])

git_repository(
    name = "rules_proto",
    commit = "cfdc2fa31879c0aebe31ce7702b1a9c8a4be02d2",
    init_submodules = 1,
    remote = "https://github.com/bazelbuild/rules_proto.git",
    shallow_since = "1612880706 +0100",
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
    tag = "0.11.0",
)

load("@io_bazel_rules_closure//closure:repositories.bzl", "rules_closure_dependencies")

rules_closure_dependencies()  # needed for @rules_python//...

#############################################
# needed by com_github_glog_glog
git_repository(
    name = "com_github_gflags_gflags",
    commit = "827c769e5fc98e0f2a34c47cef953cc6328abced",  # current as of 2021/02/17
    remote = "https://github.com/gflags/gflags.git",
    shallow_since = "1604052972 +0000",
)

#############################################
git_repository(
    name = "com_github_glog_glog",
    commit = "9dc1107f88d3a1613d61b80040d83c1c1acbac3d",  # current as of 2021/12/17
    remote = "https://github.com/google/glog.git",
    shallow_since = "1639566274 +0100",
)

#############################################
git_repository(
    name = "com_google_googletest",
    commit = "97a467571a0f615a4d96e79e4399c43221ca1232",  # current as of 2021/12/17
    remote = "https://github.com/google/googletest.git",
    shallow_since = "1639586168 -0800",
)

#############################################
git_repository(
    name = "com_google_absl",
    commit = "52d41a9ec23e39db7e2cbce5c9449506cf2d3a5c",  # current as of 2021/12/17
    remote = "https://github.com/abseil/abseil-cpp.git",
    shallow_since = "1639580175 -0500",
)

#############################################
