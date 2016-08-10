extern int test_btree(void);
extern int test_hmap(void);
extern int test_heap(void);
extern int test_llist(void);
extern int test_stack(void);

int main(void)
{
        int ret = 0;

        if (test_btree())
        {
                ret = 1;
        }
        if (test_hmap())
        {
                ret = 1;
        }
        if (test_heap())
        {
                ret = 1;
        }
        if (test_llist())
        {
                ret = 1;
        }
        if (test_stack())
        {
                ret = 1;
        }

        return ret;
}

