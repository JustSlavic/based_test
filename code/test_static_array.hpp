#pragma once

#define RUN_TEST_SUITE__STATIC_ARRAY() \
    printf("        StaticArray:\n"); \
    TEST_RUN(StaticArrayInit0); \
    TEST_RUN(StaticArrayInit1); \
    TEST_RUN(StaticArrayInit10); \
    TEST_RUN(StaticArrayBeginAndEnd); \
    TEST_RUN(StaticArrayResize); \
    TEST_RUN(StaticArrayPushBack); \
    (void)0

TEST(StaticArrayInit0)
{
    static_array<int, 5> a = make_static_array<5, int>(); // Could not deduce T, have to specialize

    int *data = a.data();
    TEST_ASSERT_EQ(data[0], 0);
    TEST_ASSERT_EQ(data[1], 0);
    TEST_ASSERT_EQ(data[2], 0);
    TEST_ASSERT_EQ(data[3], 0);
    TEST_ASSERT_EQ(data[4], 0);
    TEST_ASSERT_EQ(a.size(), 0);
    TEST_ASSERT_EQ(a.capacity(), 5);
}

TEST(StaticArrayInit1)
{
    static_array<int, 5> a = make_static_array<5>(42);

    int *data = a.data();
    TEST_ASSERT_EQ(data[0], 42);
    TEST_ASSERT_EQ(data[1], 0);
    TEST_ASSERT_EQ(data[2], 0);
    TEST_ASSERT_EQ(data[3], 0);
    TEST_ASSERT_EQ(data[4], 0);
    TEST_ASSERT_EQ(a.size(), 1);
    TEST_ASSERT_EQ(a.capacity(), 5);
}

TEST(StaticArrayInit10)
{
    static_array<int, 5> a = make_static_array<5>(5, 4, 3, 2, 1);

    int *data = a.data();
    TEST_ASSERT_EQ(data[0], 5);
    TEST_ASSERT_EQ(data[1], 4);
    TEST_ASSERT_EQ(data[2], 3);
    TEST_ASSERT_EQ(data[3], 2);
    TEST_ASSERT_EQ(data[4], 1);
    TEST_ASSERT_EQ(a.size(), 5);
    TEST_ASSERT_EQ(a.capacity(), 5);
}

TEST(StaticArrayBeginAndEnd)
{
    static_array<int, 5> a = make_static_array<5>(5, 4, 3, 2, 1);

    auto it = a.begin();
    TEST_ASSERT_EQ(*it, 5);
    TEST_ASSERT_EQ(*(it + 1), 4);
    TEST_ASSERT_EQ(*(it + 2), 3);
    TEST_ASSERT_EQ(*(it + 3), 2);
    TEST_ASSERT_EQ(*(it + 4), 1);
    TEST_ASSERT_EQ(it + 5, a.end());
}

TEST(StaticArrayResize)
{
    static_array<int, 5> a = make_static_array<5>(5, 4, 3);

    TEST_ASSERT_EQ(a.size(), 3);
    a.resize(1);
    TEST_ASSERT_EQ(a.size(), 1);
    a.resize(5);
    TEST_ASSERT_EQ(a.size(), 5);
}

TEST(StaticArrayPushBack)
{
    static_array<int, 5> a = make_static_array<5, int>();

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
}
