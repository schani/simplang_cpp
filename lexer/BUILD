cc_library(
  name = "lexer",
  srcs = ["lexer.cc"],
  hdrs = ["lexer.h"],
  deps = ["//:tokens"],
  visibility = ["//:__subpackages__"],
)

cc_test(
    name = "lexer_test",
    srcs = ["lexer_test.cc"],
    deps = [
        ":lexer",
        "//:tokens",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
    data = ["//examples:files"],
)