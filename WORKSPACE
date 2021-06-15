workspace(name = "com_github_bcsgh_utilities")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "rules_foreign_cc",
    commit = "e1f6efeda355208947efb117219af613f7eef712",  # current as of 2021/06/15
    remote = "git://github.com/bazelbuild/rules_foreign_cc.git",
    shallow_since = "1623777319 -0700",
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

"""
NOTE: might also need:
load("@rules_cc//cc:repositories.bzl", "rules_cc_dependencies")
rules_cc_dependencies()
"""

# https://github.com/bazelbuild/rules_closure
git_repository(
    name = "io_bazel_rules_closure",
    remote = "git://github.com/bazelbuild/rules_closure.git",
    tag = "0.11.0",
)

load("@io_bazel_rules_closure//closure:repositories.bzl", "rules_closure_dependencies")

rules_closure_dependencies()  # needed for @rules_python//...

#############################################
# needed by com_github_glog_glog
git_repository(
    name = "com_github_gflags_gflags",
    commit = "827c769e5fc98e0f2a34c47cef953cc6328abced",  # current as of 2021/02/17
    remote = "git://github.com/gflags/gflags.git",
    shallow_since = "1604052972 +0000",
)

#############################################
git_repository(
    name = "com_github_glog_glog",
    commit = "764162569a26da4401a8b59c96ca3525d7618a28",  # current as of 2021/06/15
    remote = "git://github.com/google/glog.git",
    shallow_since = "1623780547 +0200",
)

#############################################
git_repository(
    name = "com_google_googletest",
    commit = "e2239ee6043f73722e7aa812a459f54a28552929",  # current as of 2021/06/15
    remote = "git://github.com/google/googletest.git",
    shallow_since = "1623433346 -0700",
)

#############################################
git_repository(
    name = "com_google_absl",
    commit = "311bbd2e50ea35e921a08186840d3b6ca279e880",  # current as of 2021/06/15
    remote = "git://github.com/abseil/abseil-cpp.git",
    shallow_since = "1623359638 -0400",
)

#############################################
