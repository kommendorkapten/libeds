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

#include "stack.h"
#include <scut.h>

static int test_stack_push(void);
static int test_stack_clear(void);
static int test_stack_no_expand(void);
static int test_stack_expand(void);

int test_stack(void)
{
        int ret;

        scut_create("Test stack");

        SCUT_ADD(test_stack_push);
        SCUT_ADD(test_stack_clear);
        SCUT_ADD(test_stack_no_expand);
        SCUT_ADD(test_stack_expand);

        ret = scut_run(0);

        scut_destroy();

        return ret;
}

static int test_stack_push(void)
{
        struct stack* s = stack_create(12, 0);

        SCUT_ASSERT_TRUE(s);

        for (long i = 0; i < 12; i++)
        {
                int ret = stack_push(s, (void*)i);
                SCUT_ASSERT_FALSE(ret);
        }

        SCUT_ASSERT_IE(stack_size(s), 12);

        for (long i = 11; i >= 0; i--)
        {
                void* ret = stack_pop(s);
                SCUT_ASSERT_IE(ret, (void*)i);
        }

        SCUT_ASSERT_IE(stack_size(s), 0);
        SCUT_ASSERT_IE(stack_pop(s), NULL);
        stack_destroy(s);

        return 0;
}

static int test_stack_clear(void)
{
        struct stack* s = stack_create(12, 0);
        int ret;

        SCUT_ASSERT_TRUE(s);

        for (long i = 0; i < 12; i++)
        {
                ret = stack_push(s, (void*)i);
                SCUT_ASSERT_FALSE(ret);
        }

        SCUT_ASSERT_IE(stack_size(s), 12);
        stack_clear(s);
        SCUT_ASSERT_IE(stack_size(s), 0);        
        ret = stack_push(s, NULL);
        
        SCUT_ASSERT_FALSE(ret);
        SCUT_ASSERT_IE(stack_size(s), 1);

        stack_destroy(s);

        return 0;
}

static int test_stack_no_expand(void)
{
        struct stack* s = stack_create(12, 0);
        int ret;

        SCUT_ASSERT_TRUE(s);

        for (long i = 0; i < 12; i++)
        {
                ret = stack_push(s, (void*)i);
                SCUT_ASSERT_FALSE(ret);
        }

        ret = stack_push(s, NULL);
        SCUT_ASSERT_TRUE(ret);
        SCUT_ASSERT_IE(stack_size(s), 12);
        
        stack_destroy(s);

        return 0;
}

static int test_stack_expand(void)
{
        struct stack* s = stack_create(12, STACK_AUTO_EXPAND);
        int ret;

        SCUT_ASSERT_TRUE(s);

        for (long i = 0; i < 24; i++)
        {
                ret = stack_push(s, (void*)i);
                SCUT_ASSERT_FALSE(ret);
        }

        SCUT_ASSERT_IE(stack_size(s), 24);

        for (long i = 23; i >= 0; i--)
        {
                void* d = stack_pop(s);
                SCUT_ASSERT_IE(d, i);
        }
        
        SCUT_ASSERT_IE(stack_size(s), 0);
        stack_destroy(s);
        
        return 0;
}
