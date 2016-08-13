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
#include "llist.h"

static int test_ll_pushf(void);
static int test_ll_pushb(void);
static int test_ll_iter(void);
static int test_ll_clear(void);

int test_llist(void)
{
        int ret;

        scut_create("Test linked list");

        SCUT_ADD(test_ll_pushf);
        SCUT_ADD(test_ll_pushb);
        SCUT_ADD(test_ll_iter);
        SCUT_ADD(test_ll_clear);

        ret = scut_run(0);

        scut_destroy();

        return ret;
}

static int test_ll_pushf(void)
{
        struct llist* l = llist_create();
        void* ret;

        SCUT_ASSERT_TRUE(l);

        SCUT_ASSERT_FALSE(llist_pushf(l, (void*)0x123l));
        SCUT_ASSERT_FALSE(llist_pushf(l, (void*)0x234l));
        SCUT_ASSERT_FALSE(llist_pushf(l, (void*)0x345l));

        SCUT_ASSERT_IE(llist_size(l), 3l);

        ret = llist_pop(l);
        SCUT_ASSERT_IE(ret, 0x345l);
        ret = llist_pop(l);
        SCUT_ASSERT_IE(ret, 0x234l);
        ret = llist_pop(l);
        SCUT_ASSERT_IE(ret, 0x123l);
        ret = llist_pop(l);
        SCUT_ASSERT_IE(ret, NULL);

        llist_destroy(l);

        return 0;
}

static int test_ll_pushb(void)
{
        struct llist* l = llist_create();
        void* ret;

        SCUT_ASSERT_TRUE(l);

        SCUT_ASSERT_FALSE(llist_pushb(l, (void*)0x123l));
        SCUT_ASSERT_FALSE(llist_pushb(l, (void*)0x234l));
        SCUT_ASSERT_FALSE(llist_pushb(l, (void*)0x345l));

        SCUT_ASSERT_IE(llist_size(l), 3l);

        ret = llist_pop(l);
        SCUT_ASSERT_IE(ret, 0x123l);
        ret = llist_pop(l);
        SCUT_ASSERT_IE(ret, 0x234l);
        ret = llist_pop(l);
        SCUT_ASSERT_IE(ret, 0x345l);
        ret = llist_pop(l);
        SCUT_ASSERT_IE(ret, NULL);
        ret = llist_pop(l);
        SCUT_ASSERT_IE(ret, NULL);

        llist_destroy(l);

        return 0;
}

static int test_ll_iter(void)
{
        struct llist* l = llist_create();
        struct lnode* n;

        SCUT_ASSERT_TRUE(l);

        SCUT_ASSERT_FALSE(llist_pushb(l, (void*)0x123l));
        SCUT_ASSERT_FALSE(llist_pushb(l, (void*)0x234l));
        SCUT_ASSERT_FALSE(llist_pushb(l, (void*)0x345l));

        SCUT_ASSERT_IE(llist_size(l), 3l);

        n = llist_head(l);
        SCUT_ASSERT_TRUE(n);
        SCUT_ASSERT_IE(n->data, (void*)0x123l);

        n = n->next;
        SCUT_ASSERT_TRUE(n);
        SCUT_ASSERT_IE(n->data, (void*)0x234l);

        n = n->next;
        SCUT_ASSERT_TRUE(n);
        SCUT_ASSERT_IE(n->data, (void*)0x345l);

        SCUT_ASSERT_FALSE(n->next);

        llist_destroy(l);

        return 0;
}

static int test_ll_clear(void)
{
        struct llist* l = llist_create();

        SCUT_ASSERT_TRUE(l);

        SCUT_ASSERT_FALSE(llist_pushb(l, (void*)0x123l));
        SCUT_ASSERT_FALSE(llist_pushb(l, (void*)0x234l));
        SCUT_ASSERT_FALSE(llist_pushb(l, (void*)0x345l));

        SCUT_ASSERT_IE(llist_size(l), 3l);

        llist_clear(l);

        SCUT_ASSERT_IE(llist_size(l), 0l);

        SCUT_ASSERT_FALSE(llist_pushb(l, (void*)0x123l));
        SCUT_ASSERT_FALSE(llist_pushb(l, (void*)0x234l));
        SCUT_ASSERT_FALSE(llist_pushb(l, (void*)0x345l));

        SCUT_ASSERT_IE(llist_size(l), 3l);

        return 0;
}
