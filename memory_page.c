#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <sys/mman.h>

static size_t SYSTEM_PAGE_SIZE = 0;

void mm_init(){
    SYSTEM_PAGE_SIZE = getpagesize();
}

static void* mm_get_new_vm_page_from_kernel(int quantity){
    void *vm_page = mmap(
        0,
        quantity * SYSTEM_PAGE_SIZE,
        PROT_EXEC | PROT_READ| PROT_WRITE,
        0x20 | MAP_PRIVATE,
        0,
        0
    );

    if(vm_page == MAP_FAILED){
        printf("Error: VM Page allocation has failed \n");
        return NULL;
    }

    memset(vm_page, 0, quantity*SYSTEM_PAGE_SIZE);

    return (void*) vm_page;
}

// return a page to the kernel 

static void mm_return_vm_page_to_kernel(void *vm_page, int units){
    if(munmap(vm_page, units*SYSTEM_PAGE_SIZE)){
        printf("Error: Could not return page to the kernel using munmap.");
    }
}

int main(int argc, char** argv){
    mm_init();
    printf("VM Page Size is = %lu \n", SYSTEM_PAGE_SIZE);

    void *address_1 = mm_get_new_vm_page_from_kernel(1); 
    void *address_2 = mm_get_new_vm_page_from_kernel(1);

    printf("page 1 =  %p, page 2 = %p \n", address_1, address_2);
    return 0;
}
