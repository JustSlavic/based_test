#include "unit_testing.hpp"
#include <platform.hpp>
#include <memory_allocator.hpp>



TEST(MemoryArena)
{
    auto memory = platform::allocate_pages((void *) TERABYTES(2), MEGABYTES(1));
    auto arena = memory_allocator::make_arena(memory);

    int32 *px = arena.allocate<int32>();
    uint32 *py = arena.allocate<uint32>();
    TEST_ASSERT_EQ((uint64)px, TERABYTES(2));
    TEST_ASSERT_EQ((uint64)py, TERABYTES(2) + sizeof(int32));

    memory_allocator subarena = arena.allocate_arena(KILOBYTES(12));
    auto *pz = subarena.allocate<uint64>();

    TEST_ASSERT_EQ((uint64)pz, TERABYTES(2) + sizeof(int32) + sizeof(uint32));

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


int main()
{
    TEST_BEGIN();

    TEST_RUN(MemoryArena);
    TEST_RUN(MemoryPool);

    TEST_END();
    return 0;
}


#include <memory_allocator.cpp>

#if OS_MAC
#include <platform_posix.cpp>
#endif
