cc_library(
  name = "interpreter",
  srcs = ["interpreter.cc"],
  hdrs = ["interpreter.h"],
  deps = ["//tokens:tokens", "//parser:parser", "//lexer:lexer"],
  visibility = ["//:__subpackages__"],
)

cc_test(
    name = "interpreter_test",
    srcs = ["interpreter_test.cc"],
    deps = [
        ":interpreter",
        "//lexer:lexer",
        "//tokens:tokens",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
    data = ["//examples:files"],
)