// Copyright (c) 2021, Benjamin Shropshire,
// All rights reserved.

#include <cstdlib>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/time/clock.h"
#include "absl/time/time.h"
#include "gflags/gflags.h"  // TODO dump
#include "glog/logging.h"
#include "gtest/gtest.h"

ABSL_FLAG(int32_t, test_srand_seed, 0, "The seed used for random");

// TODO: Dump this once absl gets logging.

// Note: copied (with edits) from absl/flags/flag.h
#define ABSL_FLAG_X(T, n, d, h) ABSL_FLAG_IMPL_X(T, n, d, h)
#define ABSL_FLAG_IMPL_X(Type, name, default_value, help)                     \
  ABSL_FLAG_IMPL_DECLARE_DEF_VAL_WRAPPER(name, Type, default_value)           \
  ABSL_FLAG_IMPL_DECLARE_HELP_WRAPPER(name, help)                             \
  ABSL_CONST_INIT absl::Flag<Type> FLAGSx_##name{                             \
      ABSL_FLAG_IMPL_FLAGNAME(#name), ABSL_FLAG_IMPL_FILENAME(),              \
      ABSL_FLAG_IMPL_HELP_ARG(name), ABSL_FLAG_IMPL_DEFAULT_ARG(Type, name)}; \
  extern absl::flags_internal::FlagRegistrarEmpty FLAGSx_no##name;            \
  absl::flags_internal::FlagRegistrarEmpty FLAGSx_no##name =                  \
      ABSL_FLAG_IMPL_REGISTRAR(Type, FLAGSx_##name)

ABSL_FLAG_X(bool, alsologtostderr, false,
            "log messages go to stderr in addition to logfiles");
ABSL_FLAG_X(bool, logtostderr, false,
            "log messages go to stderr instead of logfiles");
ABSL_FLAG_X(int32_t, v, 0, "Show all VLOG(m) messages for m <= this.");

DECLARE_bool(alsologtostderr);
DECLARE_bool(logtostderr);
DECLARE_int32(v);

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  auto args = absl::ParseCommandLine(argc, argv);
  // Forward flags to glog (it doesn't use absl::Flags).
  FLAGS_alsologtostderr = absl::GetFlag(FLAGSx_alsologtostderr);
  FLAGS_logtostderr = absl::GetFlag(FLAGSx_logtostderr);
  FLAGS_v = absl::GetFlag(FLAGSx_v);
  google::InitGoogleLogging(args[0]);
  google::InstallFailureSignalHandler();

  int32_t test_srand_seed = absl::GetFlag(FLAGS_test_srand_seed);
  if (test_srand_seed == 0) {
    test_srand_seed = absl::ToUnixMicros(absl::Now());
  }
  LOG(WARNING) << "--test_srand_seed=" << test_srand_seed;
  std::srand(test_srand_seed);

  return RUN_ALL_TESTS();
}