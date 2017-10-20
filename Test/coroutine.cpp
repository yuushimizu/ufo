#include <gtest/gtest.h>
#include "ufo/coroutine.hpp"

using namespace ufo;

namespace {
    TEST(CoroutineTest, Flat) {
        auto coro = coroutine<int()> {
            []() {
                return 42;
            }, []() {
                return 84;
            }
        };
        ASSERT_FALSE(coro.is_finished());
        ASSERT_EQ(42, coro());
        ASSERT_FALSE(coro.is_finished());
        ASSERT_EQ(84, coro());
        ASSERT_TRUE(coro.is_finished());
    }
    
    TEST(CoroutineTest, Nested) {
        auto coro = coroutine<int()> {
            []() {
                return coroutine<int()> {
                    []() {
                        return 10;
                    }, []() {
                        return 20;
                    }
                };
            }
        };
        ASSERT_FALSE(coro.is_finished());
        ASSERT_EQ(10, coro());
        ASSERT_FALSE(coro.is_finished());
        ASSERT_EQ(20, coro());
        ASSERT_TRUE(coro.is_finished());
    }
    
    TEST(CoroutineTest, Mixed) {
        auto coro = coroutine<std::string()> {
            []() {
                return "foo";
            }, [] () {
                return coroutine<std::string()> {
                    []() {
                        return "foo/hoge";
                    }, []() {
                        return "foo/piyo";
                    }
                };
            }, []() {
                return "bar";
            }
        };
        ASSERT_FALSE(coro.is_finished());
        ASSERT_EQ("foo", coro());
        ASSERT_FALSE(coro.is_finished());
        ASSERT_EQ("foo/hoge", coro());
        ASSERT_FALSE(coro.is_finished());
        ASSERT_EQ("foo/piyo", coro());
        ASSERT_FALSE(coro.is_finished());
        ASSERT_EQ("bar", coro());
        ASSERT_TRUE(coro.is_finished());
    }
    
    TEST(CoroutineTest, Parameters) {
        auto coro = coroutine<int(int, int)> {
            [](int n, int m) {
                return n + m;
            }, [](int n, int m) {
                return n * m;
            }
        };
        ASSERT_FALSE(coro.is_finished());
        ASSERT_EQ(7, coro(5, 2));
        ASSERT_FALSE(coro.is_finished());
        ASSERT_EQ(18, coro(3, 6));
        ASSERT_TRUE(coro.is_finished());
    }
    
    TEST(CoroutineTest, ParametersNested) {
        auto coro = coroutine<int(int)> {
            [](int n) {
                return coroutine<int(int)> {
                    [n](int m) {
                        return n * m;
                    }, [n](int m) {
                        return n + m;
                    }
                };
            }
        };
        ASSERT_FALSE(coro.is_finished());
        ASSERT_EQ(64, coro(8));
        ASSERT_FALSE(coro.is_finished());
        ASSERT_EQ(18, coro(10));
        ASSERT_TRUE(coro.is_finished());
    }
    
    TEST(CoroutineTest, ParametersReference) {
        auto coro = coroutine<void(int &)> {
            [](int &acc) {
                acc += 1;
            }, [](int &acc) {
                acc += 10;
            }
        };
        int x = 5;
        ASSERT_FALSE(coro.is_finished());
        coro(x);
        ASSERT_EQ(6, x);
        ASSERT_FALSE(coro.is_finished());
        coro(x);
        ASSERT_EQ(16, x);
        ASSERT_TRUE(coro.is_finished());
    }
    
    coroutine<int()> make_coro(int n) {
        if (n > 1) {
            return coroutine<int()> {
                [n]() {
                    return n;
                }, [n]() {
                    return make_coro(n - 1);
                }
            };
        } else {
            return coroutine<int()> {
                []() {
                    return 1;
                }
            };
        }
    }
    
    TEST(CoroutineTest, Recursive) {
        auto coro = make_coro(5);
        ASSERT_FALSE(coro.is_finished());
        ASSERT_EQ(5, coro());
        ASSERT_FALSE(coro.is_finished());
        ASSERT_EQ(4, coro());
        ASSERT_FALSE(coro.is_finished());
        ASSERT_EQ(3, coro());
        ASSERT_FALSE(coro.is_finished());
        ASSERT_EQ(2, coro());
        ASSERT_FALSE(coro.is_finished());
        ASSERT_EQ(1, coro());
        ASSERT_TRUE(coro.is_finished());
    }
    
    TEST(CoroutineTest, Concat) {
        auto coro = coroutine<int()> {
            coroutine<int()> {
                []() {
                    return 1;
                }, []() {
                    return 2;
                }
            },
            coroutine<int()> {
                []() {
                    return 3;
                }
            }
        };
        ASSERT_FALSE(coro.is_finished());
        ASSERT_EQ(1, coro());
        ASSERT_FALSE(coro.is_finished());
        ASSERT_EQ(2, coro());
        ASSERT_FALSE(coro.is_finished());
        ASSERT_EQ(3, coro());
        ASSERT_TRUE(coro.is_finished());
    }
    
    coroutine<int()> make_inf_coro(int n) {
        return coroutine<int()> {
            [n]() {
                return n;
            },
            [n]() {
                return make_inf_coro(n + 1);
            }
        };
    }
    
    TEST(CoroutineTest, InfiniteRecursion) {
        auto coro = make_inf_coro(0);
        for (int i = 0 ; i < 2048; ++i) coro();
        ASSERT_FALSE(coro.is_finished());
        ASSERT_EQ(2048, coro());
    }
    
    TEST(CoroutineTest, Empty) {
        auto coro = coroutine<int()> {};
        ASSERT_TRUE(coro.is_finished());
    }
}
