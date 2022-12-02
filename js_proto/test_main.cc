// Copyright (c) 2021, Benjamin Shropshire,
// All rights reserved.

#include <cstdlib>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/log/initialize.h"
#include "absl/log/log.h"
#include "absl/time/clock.h"
#include "absl/time/time.h"
#include "gtest/gtest.h"

ABSL_FLAG(int32_t, test_srand_seed, 0, "The seed used for random");

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  auto args = absl::ParseCommandLine(argc, argv);
  absl::InitializeLog();

  int32_t test_srand_seed = absl::GetFlag(FLAGS_test_srand_seed);
  if (test_srand_seed == 0) {
    test_srand_seed = absl::ToUnixMicros(absl::Now());
  }
  LOG(WARNING) << "--test_srand_seed=" << test_srand_seed;
  std::srand(test_srand_seed);

  return RUN_ALL_TESTS();
}