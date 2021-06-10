workspace(name = "com_github_bcsgh_utilities")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "rules_foreign_cc",
    commit = "2407938f22aaf09e5705e726037cf61e2003b291",  # current as of 2021/06/10
    remote = "git://github.com/bazelbuild/rules_foreign_cc.git",
    shallow_since = "1623082150 -0700",
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
    commit = "c1499f64a8fb2eae2de79ed7195cf248e706efc9",  # current as of 2021/06/10
    remote = "git://github.com/google/glog.git",
    shallow_since = "1623050920 +0200",
)

#############################################
git_repository(
    name = "com_google_googletest",
    commit = "aa533abfd4232b01f9e57041d70114d5a77e6de0",  # current as of 2021/06/10
    remote = "git://github.com/google/googletest.git",
    shallow_since = "1623242719 -0400",
)

#############################################
git_repository(
    name = "com_google_absl",
    commit = "8f92175783c9685045c50f227e7c10f1cddb4d58",  # current as of 2021/06/10
    remote = "git://github.com/abseil/abseil-cpp.git",
    shallow_since = "1623281200 -0400",
)

#############################################
