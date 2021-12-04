cc_library(
    name = "bingo_subsystem",
    srcs = [
        "bingo_subsystem.cpp",
        "bingo_board.cpp",
    ],
    hdrs = [
        "bingo_subsystem.h",
        "bingo_board.h",
    ],
)

cc_binary(
    name = "play_bingo",
    srcs = ["play_bingo.cpp"],
    deps = [":bingo_subsystem"],
)
