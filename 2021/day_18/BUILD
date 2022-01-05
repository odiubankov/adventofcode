cc_library(
    name = "snailfish",
    srcs = ["snailfish.cpp"],
    hdrs = ["snailfish.h"],
)

cc_test(
    name = "test",
    srcs = ["test.cpp"],
    deps = [
        ":snailfish",
        "@com_google_googletest//:gtest_main"
    ],
)

cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    deps = [":snailfish"],
)
