#include "unit_testing.hpp"
#include <platform.hpp>
#include <memory_allocator.hpp>
#include <web.hpp>


TEST(MemoryArena)
{
    auto memory = platform::allocate_pages((void *) TERABYTES(2), MEGABYTES(1));
    auto arena = memory_allocator::make_arena(memory);

    int32 *px = arena.allocate<int32>();
    uint32 *py = arena.allocate<uint32>();

    memory_allocator subarena = arena.allocate_arena(KILOBYTES(12));
    auto *pz = subarena.allocate<uint64>();
    auto *pw = arena.allocate<int32>();
/*
    +--+--+------------+--+
    |  |  | +--+-----+ |  |
    |px|py| |pz| ... | |pw|
    |  |  | +--+-----+ |  |
    +--+--+------------+--+
*/
    TEST_ASSERT_EQ((uint64)px, TERABYTES(2));
    TEST_ASSERT_EQ((uint64)py, TERABYTES(2) + sizeof(int32));
    TEST_ASSERT_EQ((uint64)pz, TERABYTES(2) + sizeof(int32) + sizeof(uint32));
    TEST_ASSERT_EQ((uint64)pw, TERABYTES(2) + sizeof(int32) + sizeof(int32) + KILOBYTES(12));

    platform::deallocate_pages(memory);
}

TEST(MemoryPool)
{
    auto memory = platform::allocate_pages((void *) TERABYTES(2), MEGABYTES(1));
    auto pool = memory_allocator::make_pool(memory, KILOBYTES(1));

    auto *px = pool.allocate<int32>();
    auto *py = pool.allocate<int32>();

    TEST_ASSERT_EQ((uint64) px, TERABYTES(2));
    TEST_ASSERT_EQ((uint64) py, TERABYTES(2) + KILOBYTES(1));

    pool.deallocate(px);

    auto *pz = pool.allocate<int32>();
    TEST_ASSERT_EQ((uint64) pz, TERABYTES(2));

    platform::deallocate_pages(memory);
}

TEST(UrlEncode)
{
    auto memory = platform::allocate_pages((void *) TERABYTES(2), KILOBYTES(1));

    char decoded_data[] = "Привет, Мир!";
    char encoded_data[] = "%D0%9F%D1%80%D0%B8%D0%B2%D0%B5%D1%82%2C+%D0%9C%D0%B8%D1%80%21";

    auto decoded = memory_buffer::from(decoded_data, sizeof(decoded_data) - 1);
    auto encoded = memory_buffer::from(encoded_data, sizeof(encoded_data) - 1);

    int result_size = web::url_encode(decoded, memory);

    TEST_ASSERT_EQ(encoded.size, result_size);
    if (encoded.size == result_size)
        for (int i = 0; i < result_size; i++)
            TEST_ASSERT_EQ(encoded.data[i], memory.data[i]);

    platform::deallocate_pages(memory);
}

TEST(UrlDecode)
{
    auto memory = platform::allocate_pages((void *) TERABYTES(2), KILOBYTES(1));

    char decoded_data[] = "Привет, Мир!";
    char encoded_data[] = "%D0%9F%D1%80%D0%B8%D0%B2%D0%B5%D1%82%2C+%D0%9C%D0%B8%D1%80%21";

    auto decoded = memory_buffer::from(decoded_data, sizeof(decoded_data) - 1);
    auto encoded = memory_buffer::from(encoded_data, sizeof(encoded_data) - 1);

    int result_size = web::url_decode(encoded, memory);

    TEST_ASSERT_EQ(decoded.size, result_size);
    if (decoded.size == result_size)
        for (int i = 0; i < result_size; i++)
            TEST_ASSERT_EQ(decoded.data[i], memory.data[i]);

    platform::deallocate_pages(memory);
}



int main()
{
    TEST_BEGIN();

    TEST_RUN(MemoryArena);
    TEST_RUN(MemoryPool);
    TEST_RUN(UrlEncode);
    TEST_RUN(UrlDecode);

    TEST_END();
    return 0;
}


#include <memory_allocator.cpp>
#include <web.cpp>
#include <util.cpp>

#if OS_MAC
#include <os/platform_posix.cpp>
#endif
