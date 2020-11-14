workspace(name = "com_github_bcsgh_utilities")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

#############################################
git_repository(
    name = "rules_foreign_cc",
    commit = "bafdc9d274c7ad4aee4b60b3c640764c506ba3bb",  # current as of 2020/02/12
    remote = "git://github.com/bazelbuild/rules_foreign_cc.git",
)

load("@rules_foreign_cc//:workspace_definitions.bzl", "rules_foreign_cc_dependencies")

rules_foreign_cc_dependencies([])

#############################################
# needed by com_github_glog_glog
git_repository(
    name = "com_github_gflags_gflags",
    remote = "git://github.com/gflags/gflags.git",
    tag = "v2.2.2",  # current as of 2020/1/25
)

#############################################
git_repository(
    name = "com_github_glog_glog",
    remote = "git://github.com/google/glog.git",
    tag = "v0.4.0",
)

#############################################
git_repository(
    name = "com_google_googletest",
    remote = "git://github.com/google/googletest.git",
    tag = "release-1.10.0",  # current as of 2020/1/25
)

#############################################
git_repository(
    name = "com_google_absl",
    commit = "44427702614d7b86b064ba06a390f5eb2f85dbf6",  # current as of 2020/1/25
    remote = "git://github.com/abseil/abseil-cpp.git",
)

#############################################
git_repository(
    name = "bazel_rules",
    commit = "c7e80108353400eed375f3843ef053f103c51be6",  # current as of 2020/02/15
    remote = "git://github.com/bcsgh/bazel_rules.git",
)

#############################################
new_git_repository(
    name = "com_github_etr_libhttpserver",
    build_file = "@bazel_rules//http:BUILD.libhttpserver",
    commit = "e0fd7a27568caf82fad9cbe2e4dd3ce7a7532fc0",  # current as of 2020/02/03
    remote = "git://github.com/etr/libhttpserver.git",
    #tag = "v0.11.1",  # old
)

#############################################
DOMAINS = [
    # GNU mirrors
    "ftp.wayne.edu",
    "mirrors.tripadvisor.com",
    "mirrors.kernel.org",
    "mirror.clarkson.edu",
    "mirrors.syringanetworks.net",
    "mirror.us-midwest-1.nexcess.net",
    "mirrors.ocf.berkeley.edu",
    # primary
    "ftp.gnu.org",
]

http_archive(
    name = "org_gnu_microhttpd",
    build_file = "@bazel_rules//http:BUILD.microhttpd",
    sha256 = "90d0a3d396f96f9bc41eb0f7e8187796049285fabef82604acd4879590977307",
    strip_prefix = "libmicrohttpd-0.9.70",
    urls = ["https://%s/gnu/libmicrohttpd/libmicrohttpd-0.9.70.tar.gz" % domain for domain in DOMAINS],
)
