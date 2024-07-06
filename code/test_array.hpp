#pragma once

#include "string.hpp"
#include <array.hpp>
#include <memory/allocator.hpp>

#define RUN_TEST_SUITE__ARRAY() \
    printf("        Array:\n"); \
    TEST_RUN(ArrayInit0); \
    TEST_RUN(ArrayInit1); \
    TEST_RUN(ArrayInit5); \
    TEST_RUN(ArrayBeginAndEnd); \
    TEST_RUN(ArrayResize); \
    TEST_RUN(ArrayPushBack); \
    TEST_RUN(ArrayInsert); \
    TEST_RUN(ArrayClear); \
    (void)0

TEST(ArrayInit0)
{
    auto buffer = mallocator()->allocate_buffer(5 * sizeof(int));
    array<int> a = make_array<int>(buffer); // Could not deduce T, have to specialize

    int *data = a.data();
    TEST_ASSERT_EQ(data[0], 0);
    TEST_ASSERT_EQ(data[1], 0);
    TEST_ASSERT_EQ(data[2], 0);
    TEST_ASSERT_EQ(data[3], 0);
    TEST_ASSERT_EQ(data[4], 0);
    TEST_ASSERT_EQ(a.size(), 0);
    TEST_ASSERT_EQ(a.capacity(), 5);

    mallocator()->deallocate(buffer);
}

TEST(ArrayInit1)
{
    auto buffer = mallocator()->allocate_buffer(5 * sizeof(int));
    array<int> a = make_array<int>(buffer, 42);

    int *data = a.data();
    TEST_ASSERT_EQ(data[0], 42);
    TEST_ASSERT_EQ(data[1], 0);
    TEST_ASSERT_EQ(data[2], 0);
    TEST_ASSERT_EQ(data[3], 0);
    TEST_ASSERT_EQ(data[4], 0);
    TEST_ASSERT_EQ(a.size(), 1);
    TEST_ASSERT_EQ(a.capacity(), 5);

    mallocator()->deallocate(buffer);
}

TEST(ArrayInit5)
{
    auto buffer = mallocator()->allocate_buffer(5 * sizeof(int));
    array<int> a = make_array<int>(buffer, 5, 4, 3, 2, 1);

    int *data = a.data();
    TEST_ASSERT_EQ(data[0], 5);
    TEST_ASSERT_EQ(data[1], 4);
    TEST_ASSERT_EQ(data[2], 3);
    TEST_ASSERT_EQ(data[3], 2);
    TEST_ASSERT_EQ(data[4], 1);
    TEST_ASSERT_EQ(a.size(), 5);
    TEST_ASSERT_EQ(a.capacity(), 5);

    mallocator()->deallocate(buffer);
}

TEST(ArrayBeginAndEnd)
{
    auto buffer = mallocator()->allocate_buffer(5 * sizeof(int));
    array<int> a = make_array<int>(buffer, 5, 4, 3, 2, 1);

    auto it = a.begin();
    TEST_ASSERT_EQ(*it, 5);
    TEST_ASSERT_EQ(*(it + 1), 4);
    TEST_ASSERT_EQ(*(it + 2), 3);
    TEST_ASSERT_EQ(*(it + 3), 2);
    TEST_ASSERT_EQ(*(it + 4), 1);
    TEST_ASSERT_EQ(it + 5, a.end());

    mallocator()->deallocate(buffer);
}

TEST(ArrayResize)
{
    auto buffer1 = mallocator()->allocate_buffer(5 * sizeof(int));
    array<int> a = make_array<int>(buffer1, 5, 4, 3);

    TEST_ASSERT_EQ(a.size(), 3);
    a.resize(1);
    TEST_ASSERT_EQ(a.size(), 1);
    a.resize(5);
    TEST_ASSERT_EQ(a.size(), 5);

    mallocator()->deallocate(buffer1);

    auto buffer2 = mallocator()->allocate_buffer(5 * sizeof(int));
    array<int> b = make_array<int>(buffer2, 5, 4, 3);
    b.resize(5, 12);

    TEST_ASSERT_EQ(b[0], 5);
    TEST_ASSERT_EQ(b[1], 4);
    TEST_ASSERT_EQ(b[2], 3);
    TEST_ASSERT_EQ(b[3], 12);
    TEST_ASSERT_EQ(b[4], 12);
    TEST_ASSERT_EQ(b.size(), 5);

    mallocator()->deallocate(buffer2);
}

TEST(ArrayPushBack)
{
    auto buffer = mallocator()->allocate_buffer(5 * sizeof(int));
    array<int> a = make_array<int>(buffer);

    // reference push_back() noexcept;
    int& a1 = a.push_back();
    a1 = 37;
    TEST_ASSERT_EQ(a[0], 37);
    a1 = 59;
    TEST_ASSERT_EQ(a[0], 59);

    // reference push_back_uninitialized() noexcept;
    int& a2 = a.push_back();
    a2 = 101;
    a.resize(1);
    int& a2_uninitialized = a.push_back_uninitialized();
    TEST_ASSERT_EQ(a2_uninitialized, 101);

    // void push_back(const_reference value) noexcept;
    int x = 445;
    a.push_back(x);
    TEST_ASSERT_EQ(a[2], 445);

    // void push_back(value_type&& value) noexcept;
    a.push_back(345);
    TEST_ASSERT_EQ(a[3], 345);

    // void pop_back() noexcept;
    a.pop_back();
    TEST_ASSERT_EQ(a.size(), 3);
    a.pop_back();
    TEST_ASSERT_EQ(a.size(), 2);
    a.pop_back();
    TEST_ASSERT_EQ(a.size(), 1);
    a.pop_back();
    TEST_ASSERT_EQ(a.size(), 0);

    mallocator()->deallocate(buffer);
}

TEST(ArrayInsert)
{
    auto buffer = mallocator()->allocate_buffer(10 * sizeof(int));
    array<int> a = make_array<int>(buffer, 1);

    // [1] => [2, 1]
    a.insert(a.begin(), 2);
    // [2, 1] => [3, 2, 1]
    a.insert(a.begin(), 3);

    TEST_ASSERT_EQ(a[0], 3);
    TEST_ASSERT_EQ(a[1], 2);
    TEST_ASSERT_EQ(a[2], 1);
    TEST_ASSERT_EQ(a.size(), 3);

    // [3, 2, 1] => [4, 4, 3, 2, 1]
    a.insert(a.begin(), 2, 4);
    TEST_ASSERT_EQ(a[0], 4);
    TEST_ASSERT_EQ(a[1], 4);
    TEST_ASSERT_EQ(a[2], 3);
    TEST_ASSERT_EQ(a[3], 2);
    TEST_ASSERT_EQ(a[4], 1);
    TEST_ASSERT_EQ(a.size(), 5);

    // [4, 6, 6, 4, 3, 2, 1] => [4, 4, 3, 2, 1]
    a.insert(a.begin() + 1, 2, 6);
    TEST_ASSERT_EQ(a[0], 4);
    TEST_ASSERT_EQ(a[1], 6);
    TEST_ASSERT_EQ(a[2], 6);
    TEST_ASSERT_EQ(a[3], 4);
    TEST_ASSERT_EQ(a[4], 3);
    TEST_ASSERT_EQ(a[5], 2);
    TEST_ASSERT_EQ(a[6], 1);
    TEST_ASSERT_EQ(a.size(), 7);

    mallocator()->deallocate(buffer);
}

TEST(ArrayClear)
{
    auto buffer = mallocator()->allocate_buffer(5 * sizeof(int));
    array<int> a = make_array<int>(buffer, 5, 4, 3, 2, 1);

    a.clear();
    TEST_ASSERT_EQ(a.size(), 0);
    TEST_ASSERT_EQ(a.capacity(), 5);

    mallocator()->deallocate(buffer);
}
