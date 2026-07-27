#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "myalloc.h"

void print_free_list(free_list_t *free_list) {
    printf("--- FREE LIST STATE ---\n");
    if (!free_list->head) {
        printf("  [ Free List Empty ]\n");
        return;
    }

    chunky *curr = (chunky *)free_list->head;
    int index = 0;
    while (curr) {
        printf("  Node %d: Addr=%p | Size=%zu | FreeBit=%d\n",
               index++, (void *)curr, get_chunk_size(curr), chunk_is_free(curr));
        curr = (chunky *)curr->next;
    }
    printf("-----------------------\n\n");
}

int main(void) {
    printf("=========================================\n");
    printf("     RUNNING CUSTOM ALLOCATOR TESTS\n");
    printf("=========================================\n\n");

    bumpy *bumpman = new_bumpy(4096);
    assert(bumpman != NULL && "Bump allocator initialization failed");

    free_list_t free_list;
    init_free_list(&free_list);

    printf("[TEST 1] Initial Bump Allocations\n");
    char *ptr1 = (char *)myalloc(64, bumpman, &free_list);
    char *ptr2 = (char *)myalloc(128, bumpman, &free_list);
    char *ptr3 = (char *)myalloc(64, bumpman, &free_list);

    assert(ptr1 && ptr2 && ptr3);
    printf("  Allocated ptr1 (64 bytes): %p\n", (void *)ptr1);
    printf("  Allocated ptr2 (128 bytes): %p\n", (void *)ptr2);
    printf("  Allocated ptr3 (64 bytes): %p\n\n", (void *)ptr3);

    strcpy(ptr1, "Hello Allocator!");
    strcpy(ptr2, "Testing Memory Integrity...");
    printf("  Payload Check: ptr1=\"%s\", ptr2=\"%s\"\n\n", ptr1, ptr2);

    printf("[TEST 2] Freeing Memory Blocks\n");
    myfree(ptr1, &free_list);
    myfree(ptr2, &free_list);
    print_free_list(&free_list);

    printf("[TEST 3] Double Free Safeguard Test\n");
    printf("  Attempting double free on ptr1...\n  ");
    myfree(ptr1, &free_list); 
    printf("\n");

    printf("[TEST 4] Re-allocating Memory (First-Fit + Splitting)\n");
    char *ptr4 = (char *)myalloc(32, bumpman, &free_list);
    printf("  Allocated ptr4 (32 bytes): %p\n", (void *)ptr4);
    print_free_list(&free_list);

    printf("[TEST 5] Freeing All Chunks & Running Coalesce\n");
    myfree(ptr3, &free_list);
    myfree(ptr4, &free_list);
    
    printf("Before Coalescing:\n");
    print_free_list(&free_list);

    coalesce_free_chunks(&free_list);

    printf("After Coalescing:\n");
    print_free_list(&free_list);

    /* Assert that coalescing merged everything into one large free block */
    chunky *merged = (chunky *)free_list.head;
    assert(merged != NULL);
    assert(merged->next == NULL);          /* single node */
    assert(chunk_is_free(merged));          /* still marked free */
    assert(get_chunk_size(merged) >= 352u); /* merged size covers all freed data */

    printf("=========================================\n");
    printf("     ALL TESTS PASSED SUCCESSFULLY!\n");
    printf("=========================================\n");

    return 0;
}
