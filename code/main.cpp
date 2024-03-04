#include "unit_testing.hpp"


#include <memory_allocator.hpp>


TEST(MemoryArena)
{
    memory_allocator arena = mallocator()->allocate_arena(MEGABYTES(1));

    int32 *px = arena.allocate<int32>();
    *px = -14;

    uint32 *py = arena.allocate<uint32>();
    *py = 98;

    auto buffer = arena.allocate_buffer(98);
    UNUSED(buffer);

    TEST_ASSERT_EQ(*px, -14);
    TEST_ASSERT_EQ(*py, 98);
}


int main()
{
    TEST_BEGIN();

    TEST_RUN(MemoryArena);

    TEST_END();
    return 0;
}


#include <memory_allocator.cpp>
