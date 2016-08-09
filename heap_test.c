#include <scut.h>
#include "heap.h"

int test_heap_create(void);
int test_heap_insert(void);
int test_heap_clear(void);
int test_heap_min(void);
int test_heap_expand(void);

int test_cmp(const void* a, const void* b)
{
        long t1 = (long)a;
        long t2 = (long)b;
        
        if (t1 < t2)
        {
                return 1;
        }
        else if (t1 > t2)
        {
                return -1;
        }

        return 0;
}

int main(void)
{
        int ret;

        scut_create("Test heap");
        
        SCUT_ADD(test_heap_create);
        SCUT_ADD(test_heap_insert);
        SCUT_ADD(test_heap_clear);
        SCUT_ADD(test_heap_min);
        SCUT_ADD(test_heap_expand);

        ret = scut_run(0);

        return ret;
}

int test_heap_create(void)
{
        struct heap* h = heap_create(&test_cmp);
        
        if (h == NULL)
        {
                SCUT_FAIL("Can not create heap");
        }

        SCUT_ASSERT_IE(heap_size(h), 0);
        SCUT_ASSERT_IE(heap_min(h), NULL);

        heap_destroy(h);

        return 0;
}

int test_heap_insert(void)
{
        struct heap* h = heap_create(&test_cmp);
        
        heap_insert(h, (void*) 1L);
        heap_insert(h, (void*) 2L);
        heap_insert(h, (void*) 3L);
        heap_insert(h, (void*) 4L);

        SCUT_ASSERT_IE(heap_size(h), 4);

        heap_destroy(h);

        return 0;        
}

int test_heap_clear(void)
{
        struct heap* h = heap_create(&test_cmp);
        
        heap_insert(h, (void*) 1L);
        heap_insert(h, (void*) 2L);
        heap_insert(h, (void*) 3L);
        heap_insert(h, (void*) 4L);

        SCUT_ASSERT_IE(heap_size(h), 4);

        heap_clear(h);
        SCUT_ASSERT_IE(heap_size(h), 0);
        SCUT_ASSERT_IE(heap_min(h), NULL);

        heap_insert(h, (void*) 1L);
        heap_insert(h, (void*) 2L);
        heap_insert(h, (void*) 3L);
        heap_insert(h, (void*) 4L);

        SCUT_ASSERT_IE(heap_size(h), 4);

        heap_destroy(h);

        return 0;        
}

int test_heap_min(void)
{
        struct heap* h = heap_create(&test_cmp);        
        void* r;

        /* Insert in order */
        heap_insert(h, (void*) 1L);
        heap_insert(h, (void*) 2L);
        heap_insert(h, (void*) 3L);
        heap_insert(h, (void*) 4L);

        r = heap_min(h);
        SCUT_ASSERT_IE(r, 1L);
        SCUT_ASSERT_IE(heap_size(h), 3);

        r = heap_min(h);
        SCUT_ASSERT_IE(r, 2L);
        SCUT_ASSERT_IE(heap_size(h), 2);

        r = heap_min(h);
        SCUT_ASSERT_IE(r, 3L);
        SCUT_ASSERT_IE(heap_size(h), 1);

        r = heap_min(h);
        SCUT_ASSERT_IE(r, 4L);
        SCUT_ASSERT_IE(heap_size(h), 0);

        r = heap_min(h);
        SCUT_ASSERT_IE(r, NULL);

        /* Insert out of order */
        heap_insert(h, (void*) 4L);
        heap_insert(h, (void*) 3L);
        heap_insert(h, (void*) 2L);
        heap_insert(h, (void*) 1L);

        r = heap_min(h);
        SCUT_ASSERT_IE(r, 1L);
        SCUT_ASSERT_IE(heap_size(h), 3);

        r = heap_min(h);
        SCUT_ASSERT_IE(r, 2L);
        SCUT_ASSERT_IE(heap_size(h), 2);

        r = heap_min(h);
        SCUT_ASSERT_IE(r, 3L);
        SCUT_ASSERT_IE(heap_size(h), 1);

        r = heap_min(h);
        SCUT_ASSERT_IE(r, 4L);
        SCUT_ASSERT_IE(heap_size(h), 0);

        r = heap_min(h);
        SCUT_ASSERT_IE(r, NULL);

        heap_destroy(h);

        return 0;
}

int test_heap_expand(void)
{
        struct heap* h = heap_create(&test_cmp);

        /* default size is 128 entries */

        for (long i = 256; i > 0; i--)
        {
                heap_insert(h, (void*)i);
        }
        SCUT_ASSERT_IE(heap_size(h), 256);
        
        for (long i = 1; i <= 256; i++)
        {
                void* r = heap_min(h);
                
                SCUT_ASSERT_IE(r, i);
        }

        SCUT_ASSERT_IE(heap_size(h), 0);
        SCUT_ASSERT_IE(heap_min(h), NULL);

        heap_destroy(h);
        return 0;
}
