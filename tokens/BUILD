cc_library(
  name = "tokens",
  hdrs = ["tokens.h"],
  deps = ["@glog//:glog"],
  copts = ["-std=c++20"],
  visibility = ["//:__subpackages__"],
)

cc_test(
  name = "tokens_test",
  srcs = ["tokens_test.cc"],
  copts = ["-std=c++20"],
  deps = [":tokens",
          "@googletest//:gtest",
        "@googletest//:gtest_main"
        ],
   data = ["//examples:files"]
)