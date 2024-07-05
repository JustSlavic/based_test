#include "unit_testing.hpp"
#include <platform.hpp>
#include <memory/allocator.hpp>
#include <web.hpp>


#include "test_memory_arena.hpp"
#include "test_static_array.hpp"


int main()
{
    TEST_BEGIN();

    RUN_TEST_SUITE__MEMORY_ARENA();
    RUN_TEST_SUITE__STATIC_ARRAY();

    TEST_END();
    return 0;
}


#include <memory/allocator.cpp>
#include <web.cpp>
#include <util.cpp>

#if OS_MAC || OS_LINUX
#include <os/platform_posix.cpp>
#endif
