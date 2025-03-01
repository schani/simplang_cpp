cc_library(
  name = "lexer",
  srcs = ["lexer.cc"],
  hdrs = ["lexer.h"],
  deps = ["//tokens:tokens",
          "@glog//:glog"],
  visibility = ["//:__subpackages__"],
)

cc_binary(
    name = "lexer_main",
    srcs = ["lexer_main.cc"],
    deps = [":lexer",
            "//tokens:tokens",
            "@glog//:glog"],
    visibility = ["//:__subpackages__"],
)

cc_test(
    name = "lexer_test",
    srcs = ["lexer_test.cc"],
    deps = [
        ":lexer",
        "//tokens:tokens",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
    data = ["//examples:files"],
)