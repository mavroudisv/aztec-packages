#pragma once

#include <stdexcept>

#define EXPECT_THROW_WITH_MESSAGE(code, expectedMessage)                                                               \
    EXPECT_DEATH(                                                                                                      \
        try { code; } catch (const std::exception& e) {                                                                \
            std::cerr << e.what();                                                                                     \
            std::abort();                                                                                              \
        },                                                                                                             \
        expectedMessage);
