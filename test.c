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

