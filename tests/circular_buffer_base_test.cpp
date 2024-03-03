#include <xitren/circular_buffer.hpp>

#include <gtest/gtest.h>

#include <cstring>

TEST(circullar_test, basic_push_pull)
{
    using namespace xitren;
    circular_buffer<uint8_t, 8> buffer;
    EXPECT_EQ(buffer.capacity(), 8);

    std::size_t sz;
    buffer << std::array<uint8_t, 5>{'1', '2', '3', '4', '5'};
    sz = buffer.size();
    EXPECT_EQ(sz, 5);
    char ch = '1';
    for (std::size_t j = 0; j < sz; j++) {
        EXPECT_EQ(buffer.front(), ch++);
        buffer.pop();
    }
    EXPECT_EQ(buffer.size(), 0);
}

TEST(circullar_test, basic_push_pull_mass)
{
    using namespace xitren;
    circular_buffer<uint8_t, 8> buffer;
    EXPECT_EQ(buffer.capacity(), 8);

    std::size_t sz;
    buffer << std::array<uint8_t, 5>{'1', '2', '3', '4', '5'};
    sz = buffer.end() - buffer.begin();
    EXPECT_EQ(sz, 5);
    char ch = '1';
    for (auto& item : buffer) {
        EXPECT_EQ(item, ch++);
    }
    buffer >> sz;
    EXPECT_EQ(buffer.size(), 0);
}

TEST(circullar_test, basic_push_pull_overburn)
{
    using namespace xitren;
    circular_buffer<uint8_t, 10> buffer;
    EXPECT_EQ(buffer.capacity(), 10);

    std::size_t sz;
    buffer << std::array<uint8_t, 8>{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    buffer << std::array<uint8_t, 8>{'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'};
    sz = buffer.size();
    EXPECT_EQ(sz, 10);
    char ch = 'g';
    for (std::size_t j = 0; j < sz; j++) {
        EXPECT_EQ(buffer.begin().operator*(), ch++);
        buffer.pop();
    }
    EXPECT_EQ(buffer.size(), 0);
}

TEST(circullar_test, basic_push_pull_array)
{
    using namespace xitren;
    circular_buffer<uint8_t, 10> buffer;
    EXPECT_EQ(buffer.capacity(), 10);

    std::size_t sz;
    buffer << std::array<uint8_t, 8>{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    buffer << std::array<uint8_t, 8>{'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'};
    sz = buffer.end() - buffer.begin();
    EXPECT_EQ(sz, 10);
    char                   ch = 'g';
    std::array<uint8_t, 4> ret1;
    buffer >> ret1;
    for (auto& item : ret1) {
        EXPECT_EQ(item, ch++);
    }
    sz = buffer.end() - buffer.begin();
    EXPECT_EQ(sz, 6);
    ch = 'k';
    std::array<uint8_t, 6> ret2;
    buffer >> ret2;
    for (auto& item : ret2) {
        EXPECT_EQ(item, ch++);
    }
    EXPECT_EQ(buffer.size(), 0);
}

TEST(circullar_test, basic_push_pull_mass_overburn)
{
    using namespace xitren;
    circular_buffer<uint8_t, 10> buffer;
    EXPECT_EQ(buffer.capacity(), 10);

    std::size_t sz;
    buffer << std::array<uint8_t, 8>{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    buffer << std::array<uint8_t, 8>{'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'};
    sz = buffer.end() - buffer.begin();
    EXPECT_EQ(sz, 10);
    char ch = 'g';
    for (auto& item : buffer) {
        EXPECT_EQ(item, ch++);
    }
    buffer >> sz;
    EXPECT_EQ(buffer.size(), 0);
}

TEST(circullar_test, basic_update_head)
{
    using namespace xitren;
    circular_buffer<uint8_t, 10> buffer;
    EXPECT_EQ(buffer.capacity(), 10);
    memcpy(buffer.storage(), reinterpret_cast<void const*>("abcdefghij"), 10);

    std::size_t sz;
    buffer.update_head(4);
    sz = buffer.end() - buffer.begin();
    EXPECT_EQ(sz, 4);
    char ch = 'a';
    for (std::size_t j = 0; j < sz; j++) {
        EXPECT_EQ(buffer.begin().operator*(), ch++);
        buffer.pop();
    }
    EXPECT_EQ(buffer.size(), 0);
    buffer.update_head(8);
    sz = buffer.end() - buffer.begin();
    EXPECT_EQ(sz, 4);
    ch = 'e';
    for (std::size_t j = 0; j < sz; j++) {
        EXPECT_EQ(buffer.begin().operator*(), ch++);
        buffer.pop();
    }
    EXPECT_EQ(buffer.size(), 0);
}

TEST(circullar_test, basic_update_head_overburn)
{
    using namespace xitren;
    circular_buffer<uint8_t, 10> buffer;
    EXPECT_EQ(buffer.capacity(), 10);
    memcpy(buffer.storage(), reinterpret_cast<uint8_t const*>("abcdefghij"), 10);

    std::size_t sz;
    buffer.update_head(8);
    sz = buffer.end() - buffer.begin();
    EXPECT_EQ(sz, 8);
    char ch = 'a';
    for (auto& item : buffer) {
        EXPECT_EQ(item, ch++);
    }
    buffer >> sz;
    EXPECT_EQ(buffer.size(), 0);
}

TEST(circullar_test, basic_time_measure_push)
{
    using namespace xitren;
    circular_buffer<uint8_t, 1024> buffer;
    EXPECT_EQ(buffer.capacity(), 1024);

    for (std::size_t i = 0; i < 10000000; i++) {
        buffer << std::array<uint8_t, 8>{'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'};
    }
}

TEST(circullar_test, basic_time_measure_push_pop)
{
    using namespace xitren;
    circular_buffer<uint8_t, 1024> buffer;
    EXPECT_EQ(buffer.capacity(), 1024);

    for (std::size_t i = 0; i < 10000000; i++) {
        buffer << std::array<uint8_t, 8>{'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'};
        buffer >> 3;
    }
}
TEST(circullar_test, basic_boundary_test)
{
    using namespace xitren;
    circular_buffer<uint8_t, 16> buffer;

    for (std::size_t i = 0; i < buffer.capacity(); i++) {
        buffer.push(i);
        EXPECT_EQ(buffer.size(), i + 1);
    }
    EXPECT_EQ(buffer.size(), buffer.capacity());
    buffer.push(10);
    EXPECT_EQ(buffer.size(), buffer.capacity());
    for (std::size_t i = buffer.capacity(); i > 0; i--) {
        buffer.pop();
        EXPECT_EQ(buffer.size(), i - 1);
    }
    buffer.pop();
    EXPECT_EQ(buffer.size(), 0);
}
