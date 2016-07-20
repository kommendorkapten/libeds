#include <scut.h>
#include "btree.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern void btree_print(struct btree*);
int test_bt_insert(void);
int test_bt_find(void);
int test_bt_remove(void);
int test_bt_bf(void);
int test_bt_df(void);
int test_bt_balance(void);
int cmp_fun(const void*, const void*);

int main(void)
{
        int ret;

        scut_create("Test Binary tree");

        SCUT_ADD(test_bt_insert);
        SCUT_ADD(test_bt_find);
        SCUT_ADD(test_bt_remove);
        SCUT_ADD(test_bt_bf);
        SCUT_ADD(test_bt_df);
        SCUT_ADD(test_bt_balance);
        ret = scut_run(0);

        return ret;
}

int cmp_fun(const void* a, const void* b)
{
        char* s1 = (char*)a;
        char* s2 = (char*)b;

        return strcmp(s1, s2);
}

int cmp_lng(const void* a, const void* b)
{
        long s1 = (long)a;
        long s2 = (long)b;
        int ret = 0;

        if (s1 > s2) 
        {
                ret = 1;
        }
        if (s1 < s2)
        {
                ret = -1;
        }

        return ret;
}

int test_bt_insert(void)
{
        struct btree* bt = btree_create(&cmp_fun);
        
        SCUT_ASSERT_TRUE(bt);
        SCUT_ASSERT_IE(btree_height(bt), 0);

        // Insert in worst case order, shall only be allowed to insert two
        SCUT_ASSERT_IE(btree_insert(bt, "a"), 0);
        SCUT_ASSERT_IE(btree_insert(bt, "aa"), 0);
        SCUT_ASSERT_IE(btree_insert(bt, "aaa"), 0);
        SCUT_ASSERT_IE(btree_height(bt), 3);
        SCUT_ASSERT_IE(btree_size(bt), 3);

        btree_clear(bt);
        SCUT_ASSERT_IE(btree_height(bt), 0);
        SCUT_ASSERT_IE(btree_size(bt), 0);
       
        // Insert in optimal order, shall store four items
        SCUT_ASSERT_IE(btree_insert(bt, "aaa"), 0);
        SCUT_ASSERT_IE(btree_insert(bt, "aa"), 0);
        SCUT_ASSERT_IE(btree_insert(bt, "aa"), 0);
        SCUT_ASSERT_IE(btree_insert(bt, "aaaa"), 0);
        SCUT_ASSERT_IE(btree_insert(bt, "a"), 0);
        SCUT_ASSERT_IE(btree_height(bt), 3);
        SCUT_ASSERT_IE(btree_size(bt), 4);
        
        btree_destroy(bt);

        return 0;
}

int test_bt_find(void)
{
        struct btree* bt;
        void* ret;

        bt = btree_create(&cmp_fun);
        SCUT_ASSERT_IE(btree_insert(bt, "aaaaa"), 0);
        SCUT_ASSERT_IE(btree_insert(bt, "aaaa"), 0);
        SCUT_ASSERT_IE(btree_insert(bt, "aaa"), 0);
        SCUT_ASSERT_IE(btree_insert(bt, "aa"), 0);
        SCUT_ASSERT_IE(btree_insert(bt, "aab"), 0);
        SCUT_ASSERT_IE(btree_insert(bt, "a"), 0);
        SCUT_ASSERT_IE(btree_insert(bt, "bbb"), 0);
        SCUT_ASSERT_IE(btree_insert(bt, "bb"), 0);
        SCUT_ASSERT_IE(btree_insert(bt, "b"), 0);
        SCUT_ASSERT_IE(btree_height(bt), 5);
        SCUT_ASSERT_IE(btree_size(bt), 9);

        ret = btree_find(bt, "a");
        SCUT_ASSERT_TRUE(ret);
        ret = btree_find(bt, "aaaaa");
        SCUT_ASSERT_TRUE(ret);
        ret = btree_find(bt, "b");
        SCUT_ASSERT_TRUE(ret);
        ret = btree_find(bt, "bbb");
        SCUT_ASSERT_TRUE(ret);
        ret = btree_find(bt, "c");
        SCUT_ASSERT_FALSE(ret);

        btree_destroy(bt);

        return 0;
}

int test_bt_remove(void)
{
        struct btree* bt = btree_create(&cmp_lng);
        void* e;

        // No children
        SCUT_ASSERT_IE(btree_insert(bt, (void*)10l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)5l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)14l), 0);
        SCUT_ASSERT_IE(btree_height(bt), 2);
        SCUT_ASSERT_IE(btree_size(bt), 3);
        e = btree_remove(bt, (void*)14l);
        SCUT_ASSERT_IE(btree_size(bt), 2);
        SCUT_ASSERT_TRUE(e == (void*)14l);
        e = btree_find(bt, (void*)14l);
        SCUT_ASSERT_TRUE(e == NULL);

        btree_destroy(bt);

        // one left child
        bt = btree_create(&cmp_lng);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)50l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)25l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)60l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)17l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)10l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)20l), 0);
        SCUT_ASSERT_IE(btree_height(bt), 4);
        e = btree_remove(bt, (void*)25l);
        SCUT_ASSERT_TRUE(e == (void*)25l);
        SCUT_ASSERT_TRUE(btree_find(bt, (void*)17l));
        SCUT_ASSERT_TRUE(btree_find(bt, (void*)60l));
        SCUT_ASSERT_TRUE(btree_find(bt, (void*)20l));
        SCUT_ASSERT_TRUE(btree_find(bt, (void*)10l));
        SCUT_ASSERT_FALSE(btree_find(bt, (void*)25l));
        btree_destroy(bt);

        // one right child
        bt = btree_create(&cmp_lng);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)10l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)5l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)15l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)30l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)25l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)35l), 0);
        SCUT_ASSERT_IE(btree_size(bt), 6);

        e = btree_remove(bt, (void*)15l);
        SCUT_ASSERT_TRUE(e == (void*)15l);
        SCUT_ASSERT_IE(btree_size(bt), 5);
        SCUT_ASSERT_IE(e, 15);
        e = btree_find(bt, (void*)15l);
        SCUT_ASSERT_TRUE(e == NULL);
        SCUT_ASSERT_TRUE(btree_find(bt, (void*)30l));
        SCUT_ASSERT_TRUE(btree_find(bt, (void*)25l));
        SCUT_ASSERT_TRUE(btree_find(bt, (void*)35l));

        btree_destroy(bt);

        // two children
        bt = btree_create(&cmp_lng);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)50l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)25l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)60l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)17l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)55l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)70l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)10l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)20l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)54l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)58l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)75l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)72l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)80l), 0);        
        SCUT_ASSERT_IE(btree_size(bt), 13);
        SCUT_ASSERT_IE(btree_height(bt), 5);
        e = btree_remove(bt, (void*)60l);
        SCUT_ASSERT_TRUE(e == (void*)60l);
        SCUT_ASSERT_IE(btree_height(bt), 4);
        SCUT_ASSERT_FALSE(btree_find(bt, (void*)60l));
        SCUT_ASSERT_TRUE(btree_find(bt, (void*)70l));
        SCUT_ASSERT_TRUE(btree_find(bt, (void*)10l));
        SCUT_ASSERT_TRUE(btree_find(bt, (void*)54l));
        SCUT_ASSERT_TRUE(btree_find(bt, (void*)80l));

        btree_destroy(bt);

        return 0;
}

int test_bt_bf(void) 
{
        struct btree* bt = btree_create(&cmp_lng);
        void** traversal;
        void* res[] = {
                (void*)50l,
                (void*)25l,
                (void*)60l,
                (void*)17l,
                (void*)55l,
                (void*)70l,
                (void*)10l,
                (void*)20l,
                (void*)54l,
                (void*)58l,
                (void*)75l,
                (void*)72l,
                (void*)80l
        };

        SCUT_ASSERT_IE(btree_insert(bt, (void*)50l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)25l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)60l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)17l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)55l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)70l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)10l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)20l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)54l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)58l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)75l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)72l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)80l), 0);

        traversal = btree_bf(bt);
        SCUT_ASSERT_TRUE(traversal);
        
        for (unsigned int i = 0; i < btree_size(bt); i++)
        {
                SCUT_ASSERT_IE(res[i], traversal[i]);
        }

        free(traversal);
        btree_destroy(bt);

        return 0;
}

int test_bt_df(void) 
{
        struct btree* bt = btree_create(&cmp_lng);
        void** traversal;
        void* res[] = {
                (void*)50l,
                (void*)25l,
                (void*)17l,
                (void*)10l,
                (void*)20l,
                (void*)60l,
                (void*)55l,
                (void*)54l,
                (void*)58l,
                (void*)70l,
                (void*)75l,
                (void*)72l,
                (void*)80l
        };

        SCUT_ASSERT_IE(btree_insert(bt, (void*)50l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)25l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)60l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)17l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)55l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)70l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)10l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)20l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)54l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)58l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)75l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)72l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)80l), 0);

        traversal = btree_df(bt);
        SCUT_ASSERT_TRUE(traversal);
        
        for (unsigned int i = 0; i < btree_size(bt); i++)
        {
                SCUT_ASSERT_IE(res[i], traversal[i]);
        }

        free(traversal);
        btree_destroy(bt);

        return 0;
}

int test_bt_balance(void)
{
        struct btree* bt = btree_create(&cmp_lng);

        SCUT_ASSERT_IE(btree_insert(bt, (void*)50l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)25l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)60l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)17l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)55l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)70l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)10l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)20l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)54l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)58l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)75l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)72l), 0);
        SCUT_ASSERT_IE(btree_insert(bt, (void*)80l), 0);

        SCUT_ASSERT_IE(btree_height(bt), 5);

        btree_balance(bt);

        SCUT_ASSERT_IE(btree_size(bt), 13);
        SCUT_ASSERT_IE(btree_height(bt), 4);

        btree_destroy(bt);

        return 0;
}
