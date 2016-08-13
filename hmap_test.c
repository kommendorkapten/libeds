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

#include "hmap.h"
#include <scut.h>
#include <stdlib.h>
#include <string.h>

static int test_hmap_create(void);
static int test_hmap_get_set(void);
static int test_hmap_clear(void);
static int test_hmap_del(void);
static int test_hmap_expand(void);
static int test_hmap_iter(void);

uint32_t const_hash(void* key)
{
        return 1;
}

int test_hmap(void)
{
        int ret;
        
        scut_create("Test Hash table");
        
        SCUT_ADD(test_hmap_create);
        SCUT_ADD(test_hmap_get_set);
        SCUT_ADD(test_hmap_clear);
        SCUT_ADD(test_hmap_del);
        SCUT_ADD(test_hmap_expand);
        SCUT_ADD(test_hmap_iter);

        ret = scut_run(0);

        scut_destroy();

        return ret;
}

static int test_hmap_create(void)
{
        struct hmap* h = hmap_create(NULL, NULL, 128, 0.7);
        struct hmap_entry* i;
        size_t count;
        
        SCUT_ASSERT_IE(hmap_size(h), 0);
        i = hmap_iter(h, &count);

        if (count)
        {
                SCUT_FAIL("Non empty list returned");
        }

        free(i);
        hmap_destroy(h);

        return 0;
}

static int test_hmap_get_set(void)
{
        struct hmap* h = hmap_create(NULL, NULL, 128, 0.7);
        
        SCUT_ASSERT_IE(hmap_size(h), 0);

        hmap_set(h, "a", (void*)1l);
        hmap_set(h, "aa", (void*)2l);
        hmap_set(h, "aaa", (void*)3l);

        SCUT_ASSERT_IE(hmap_size(h), 3);
        SCUT_ASSERT_IE(hmap_cap(h), 128);

        SCUT_ASSERT_IE(hmap_get(h, "a"), (void*)1l);
        SCUT_ASSERT_IE(hmap_get(h, "aa"), (void*)2l);
        SCUT_ASSERT_IE(hmap_get(h, "aaa"), (void*)3l);

        SCUT_ASSERT_IE(hmap_get(h, "aaaa"), NULL);

        /* Replace a value */
        hmap_set(h, "aa", (void*)123l);
        SCUT_ASSERT_IE(hmap_size(h), 3);
        SCUT_ASSERT_IE(hmap_get(h, "aa"), (void*)123l);

        hmap_destroy(h);

        return 0;
}

static int test_hmap_clear(void)
{
        struct hmap* h = hmap_create(NULL, NULL, 128, 0.7);
        
        SCUT_ASSERT_IE(hmap_size(h), 0);

        hmap_set(h, "a", (void*)1l);
        hmap_set(h, "aa", (void*)2l);
        hmap_set(h, "aaa", (void*)3l);

        SCUT_ASSERT_IE(hmap_size(h), 3);
        SCUT_ASSERT_IE(hmap_cap(h), 128);
        
        hmap_clear(h);
        SCUT_ASSERT_IE(hmap_size(h), 0);

        hmap_set(h, "a", (void*)4l);
        hmap_set(h, "aa", (void*)5l);
        hmap_set(h, "aaa", (void*)6l);

        SCUT_ASSERT_IE(hmap_get(h, "a"), (void*)4l);
        SCUT_ASSERT_IE(hmap_get(h, "aa"), (void*)5l);
        SCUT_ASSERT_IE(hmap_get(h, "aaa"), (void*)6l);

        SCUT_ASSERT_IE(hmap_size(h), 3);
        SCUT_ASSERT_IE(hmap_cap(h), 128);

        SCUT_ASSERT_IE(hmap_get(h, "aaaa"), NULL);

        hmap_destroy(h);

        return 0;
}

static int test_hmap_del(void)
{
        struct hmap* h = hmap_create(NULL, NULL, 128, 0.7);
        
        SCUT_ASSERT_IE(hmap_size(h), 0);

        hmap_set(h, "a", (void*)1l);        
        SCUT_ASSERT_IE(hmap_get(h, "a"), (void*)1l);
        SCUT_ASSERT_IE(hmap_size(h), 1);

        hmap_del(h, "a");
        SCUT_ASSERT_IE(hmap_get(h, "a"), NULL);
        SCUT_ASSERT_IE(hmap_size(h), 0);

        hmap_destroy(h);

        /* Test with deleted entries */
        h = hmap_create(&const_hash, NULL, 128, 0.7);

        hmap_set(h, "a", (void*)100L);
        hmap_set(h, "b", (void*)200L);
        hmap_set(h, "c", (void*)300L);
        hmap_set(h, "d", (void*)400L);
        SCUT_ASSERT_IE(hmap_size(h), 4);

        SCUT_ASSERT_IE(hmap_get(h, "a"), (void*)100L);
        SCUT_ASSERT_IE(hmap_get(h, "b"), (void*)200L);
        SCUT_ASSERT_IE(hmap_get(h, "c"), (void*)300L);
        SCUT_ASSERT_IE(hmap_get(h, "d"), (void*)400L);

        hmap_del(h, "a");
        SCUT_ASSERT_IE(hmap_size(h), 3);

        SCUT_ASSERT_IE(hmap_get(h, "a"), NULL);
        SCUT_ASSERT_IE(hmap_get(h, "b"), (void*)200L);
        SCUT_ASSERT_IE(hmap_get(h, "c"), (void*)300L);
        SCUT_ASSERT_IE(hmap_get(h, "d"), (void*)400L);

        hmap_del(h, "c");
        SCUT_ASSERT_IE(hmap_size(h), 2);

        SCUT_ASSERT_IE(hmap_get(h, "a"), NULL);
        SCUT_ASSERT_IE(hmap_get(h, "b"), (void*)200L);
        SCUT_ASSERT_IE(hmap_get(h, "c"), NULL);
        SCUT_ASSERT_IE(hmap_get(h, "d"), (void*)400L);

        hmap_set(h, "a", (void*)666L);
        hmap_set(h, "c", (void*)777L);

        /* Oh lord. Things are getting ugly. */
        char* elems = (char*)h;
        long val;
        /* Step up to elems ptr */
        elems = elems + 16;
        /* follow up elems ptr */
        elems = *(char**)elems;

        /* data is at offset 8 */
        elems += 8;

        /* first and sixth slot should be empty */
        /* Expected data is 0, 666, 200, 777, 400, 0 */

        /* node size is 24 bit on 64b system */
        memcpy(&val, elems, sizeof(long));
        SCUT_ASSERT_IE(val, 0L);
        memcpy(&val, elems + 24, sizeof(long));
        SCUT_ASSERT_IE(val, 666L);
        memcpy(&val, elems + 48, sizeof(long));
        SCUT_ASSERT_IE(val, 200L);
        memcpy(&val, elems + 72, sizeof(long));
        SCUT_ASSERT_IE(val, 777L);
        memcpy(&val, elems + 96, sizeof(long));
        SCUT_ASSERT_IE(val, 400L);        
        memcpy(&val, elems + 120, sizeof(long));
        SCUT_ASSERT_IE(val, 0L);
        
        hmap_destroy(h);

        return 0;
}

static int test_hmap_expand(void)
{
        struct hmap* h = hmap_create(NULL, NULL, 16, 0.7);        
        long values[14] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
        char* keys[14] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                          "10", "11", "12", "13"};
        size_t elems = sizeof(keys) / sizeof(char*);
        size_t count = 0;
        struct hmap_entry* entries;
        
        /* 11 first should be ok */

        for (size_t i = 0; i < 11; i++)
        {
                hmap_set(h, keys[i], (void*)values[i]);
        }

        SCUT_ASSERT_IE(hmap_size(h), 11);
        SCUT_ASSERT_IE(hmap_cap(h), 16);

        /* add rest, expect expansion */
        for (size_t i = 11; i < elems; i++)
        {
                hmap_set(h, keys[i], (void*)values[i]);                
        }

        SCUT_ASSERT_IE(hmap_size(h), 14);
        SCUT_ASSERT_IE(hmap_cap(h), 32);

        /* Make sure all elems are present */
        for (size_t i = 0; i < elems; i++)
        {
                void* v = hmap_get(h, keys[i]);
                SCUT_ASSERT_IE(v, values[i]);
        }

        /* Iterate over all elems as well */
        entries = hmap_iter(h, &count);
        SCUT_ASSERT_IE(count, elems);

        for (size_t i = 0; i < elems; i++)
        {
                int found = 0;

                for (size_t j = 0; j < count; j++)
                {
                        if (values[i] == (long)entries[j].data &&
                            strncmp(keys[i], entries[j].key, 2) == 0)
                        {
                                found = 1;
                                break;
                        }
                }

                if (!found)
                {
                        char s[32];
                        snprintf(s, 32, "Could not find %ld", values[i]);
                        SCUT_FAIL(s);
                }
        }
        

        hmap_destroy(h);
        free(entries);

        return 0;
}

static int test_hmap_iter(void)
{
        struct hmap* h = hmap_create(NULL, NULL, 128, 0.7);
        struct hmap_entry* data;
        size_t count;
        long exp[4] = {100L, 200L, 300L, 400L};
        char* keys[4] = {"a", "b", "c", "d" };
        size_t elems = sizeof(exp)/sizeof(long);

        hmap_set(h, "a", (void*)100L);
        hmap_set(h, "b", (void*)200L);
        hmap_set(h, "c", (void*)300L);
        hmap_set(h, "d", (void*)400L);
        SCUT_ASSERT_IE(hmap_size(h), 4);

        data = hmap_iter(h, &count);

        SCUT_ASSERT_IE(count, elems);

        for (size_t i = 0; i < elems; i++)
        {
                int found = 0;
                
                for (size_t j = 0; j < count; j++)
                {
                        if (exp[i] == (long)data[j].data &&
                            strcmp(keys[i], data[j].key) == 0)
                        {
                                found = 1;
                                break;
                        }
                }

                if (!found)
                {
                        char s[32];
                        snprintf(s, 32, "Could not find %ld", exp[i]);
                        SCUT_FAIL(s);
                }
        }

        hmap_destroy(h);
        free(data);

        return 0;
}
