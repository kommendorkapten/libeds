/*
* Copyright (C) 2016 Fredrik Skogman, skogman - at - gmail.com.
* This file is part of libeds.
*
* The contents of this file are subject to the terms of the Common
* Development and Distribution License (the "License"). You may not use this
* file except in compliance with the License. You can obtain a copy of the
* License at http://opensource.org/licenses/CDDL-1.0. See the License for the
* specific language governing permissions and limitations under the License. 
* When distributing the software, include this License Header Notice in each
* file and include the License file at http://opensource.org/licenses/CDDL-1.0.
*/

#include <scut.h>
#include "heap.h"

static int test_heap_create(void);
static int test_heap_insert(void);
static int test_heap_clear(void);
static int test_heap_min(void);
static int test_heap_expand(void);

static int test_cmp(const void* a, const void* b)
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

int test_heap(void)
{
        int ret;

        scut_create("Test heap");
        
        SCUT_ADD(test_heap_create);
        SCUT_ADD(test_heap_insert);
        SCUT_ADD(test_heap_clear);
        SCUT_ADD(test_heap_min);
        SCUT_ADD(test_heap_expand);

        ret = scut_run(0);

        scut_destroy();
        
        return ret;
}

static int test_heap_create(void)
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

static int test_heap_insert(void)
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

static int test_heap_clear(void)
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

static int test_heap_min(void)
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

static int test_heap_expand(void)
{
        struct heap* h = heap_create(&test_cmp);

        /* default size is 128 entries */
        long size = 100000;


        for (long i = size; i > 0; i--)
        {
                heap_insert(h, (void*)i);
        }
        SCUT_ASSERT_IE(heap_size(h), size);
        
        for (long i = 1; i <= size; i++)
        {
                void* r = heap_min(h);
                
                SCUT_ASSERT_IE(r, i);
        }

        SCUT_ASSERT_IE(heap_size(h), 0);
        SCUT_ASSERT_IE(heap_min(h), NULL);

        heap_destroy(h);
        return 0;
}
