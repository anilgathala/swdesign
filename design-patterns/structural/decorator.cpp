// Decorator design pattern
// create
// Decorator pattern allows a user to add new functionality to an existing object without altering its structure.
// link: https://www.tutorialspoint.com/design_pattern/decorator_pattern.htm
//
// Example:
// Take Pizza class and build a "pepporoni", "paneer" pizzas by creating wrapper classes aroud basic Pizza class
//

#include <sys/types.h>
#include <iostream>
#include <stdlib.h>

// 32 byte alignment
const size_t alignment_bytes = 128;

class MemAllocator {
public:
    void* do_alloc(size_t size) {
        void* addr = malloc(size);
        std::cout << "allocated " << size << " bytes at addr " << addr << std::endl;
        return addr;
    }

    void do_free(void* addr) {
        free (addr);
        std::cout << "free'd addr " << addr << std::endl;
    }
};


class AlignedMemAllocator {
private:
    MemAllocator* basic_allocator_;
public:
    AlignedMemAllocator(MemAllocator* basic_allocator): basic_allocator_(basic_allocator)
    {}

    void* do_alloc(size_t size) {
        // basic idea: allocate a few more bytes to include:
        // -- alignedment_bytes
        // -- size_t (for srtoring orig ptr)

        // step 1: allocate more bytes
        void* addr = basic_allocator_->do_alloc(size + alignment_bytes + sizeof(size_t));
        std::cout << "orig addr: " << addr << std::endl;
        // step 2: calculate ptr after extra bytes (p1)
        size_t p1  = ((size_t)addr + alignment_bytes + sizeof(size_t));
        std::cout << "p1 = " << (void*)p1 << " p1 % alignement = " << (p1 % alignment_bytes) << std::endl;
        // step 3: remove mod bytes to get previous aligned addr from p1 (ret_addr)
        void* ret_addr = (void*)(p1 - (p1 % alignment_bytes));
        std::cout << "ret addr: " << ret_addr << std::endl; 
        // step 4: store orig ptr one byte prior to aligned address (ret_addr)
        *((size_t*)ret_addr - 1) = (size_t) addr;
        std::cout << "orig_addr: " << addr << " returning aligned address: " << ret_addr << std::endl;
        return ret_addr;
    }

    void do_free(void* addr) {
        // step 1: step one byte back to retrieve orig ptr
        void* orig_addr = (void *) *((size_t*) addr - 1);
        basic_allocator_->do_free(orig_addr);
        std::cout << "(aligned) free'd addr " << addr << " orig_addr " << orig_addr << std::endl;
    }
};

bool is_aligned(void *addr, size_t a_alignment_bytes)
{
    // set all bits one for the alignment bit-length
    // check if AND'ing results in complete zero, if so addr is aligned
    // otherwise, return false
    if ((size_t) addr & (a_alignment_bytes - 1)) {
        return false;
    }
    return true;
}

int main()
{
    MemAllocator* basic_allocator = new MemAllocator();
    

    AlignedMemAllocator* aligned_allocator = new AlignedMemAllocator(new MemAllocator());

    for (int i = 0; i < 100; i++) {
        void* addr1 = basic_allocator->do_alloc(16);
        void* addr2 = aligned_allocator->do_alloc(16);

        std::cout << "add1 = " << addr1 << " addr2 = " << addr2 << std::endl;
        // check if addresses are aligned
        if (! is_aligned(addr1, alignment_bytes)) {
            std::cout << "***********>> basic allocator: addr not aligned" << std::endl;
        }
        if (! is_aligned(addr2, alignment_bytes)) {
            std::cout << "***********>> aligned allocator: addr not aligned" << std::endl;
        }

        basic_allocator->do_free(addr1);
        aligned_allocator->do_free(addr2);
    }
    

    return 0;
}

