#include <gtest/gtest.h>


extern "C"
{
#include <vmm.h>

}


TEST(Vmm, Basic) 
{
    int r;
    r = vmm_init();
    EXPECT_EQ(0,r);
}
