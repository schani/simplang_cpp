cc_library(
  name = "parser",
  srcs = ["parser.cc"],
  hdrs = ["parser.h"],
  deps = [
    "//ast:ast",
     "//lexer:lexer",
    "//tokens:tokens",
     ],
     copts = ["-std=c++20"],
  visibility = ["//:__subpackages__"],
)

cc_test(
    name = "parser_test",
    srcs = ["parser_test.cc"],
    deps = [
        ":parser",
        "//lexer:lexer",
        "//tokens:tokens",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
    data = ["//examples:files"],
)