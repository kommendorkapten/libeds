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

#include "btree.h"
#include "heap.h"
#include "hmap.h"
#include "llist.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <assert.h>

int* dur_btree;
int* dur_heap;
int* dur_hmap; 
int* dur_llist; 
int* dur_stack;
long* data;

unsigned long current_time_us(void);
void perf_insert(int, int);
void perf_rebalance(int, int);
void perf_find(int, int);
void gauss_dist(int*, int, double*, double*);

/* util  methods */
int bt_cmp(const void* a, const void* b);
uint32_t hmap_hash_fn(void*);
int hmap_eq_fn(void*, void*);

/* Data structure references */
struct btree* bt;
struct llist* ll;
struct hmap* hmap;
struct heap* heap;

/* Dummy variable to prohibit compiler from optimizing out code */
long dummy;

int main(void)
{
        int outer = 1000;
        int inner = 100;

        dur_btree = malloc(sizeof(int) * outer * inner);
        dur_llist = malloc(sizeof(int) * outer * inner);
        dur_hmap  = malloc(sizeof(int) * outer * inner);
        dur_heap  = malloc(sizeof(int) * outer * inner);
        dur_stack = malloc(sizeof(int) * outer * inner);
        data      = malloc(sizeof(long) * outer * inner);

        /* Init test data */
        for (int i = 0; i < outer * inner; i++)
        {
                data[i] = i + 1;
        }
        /* Shuffle data, insert into binary tree will take forever
           if inserted in sorted order */
        for (int i = 0; i < outer * inner; i++)
        {
                long tmp = data[i];
                int new = rand() % (outer * inner);

                data[i] = data[new];
                data[new] = tmp;
        }

        bt   = btree_create(&bt_cmp);
        ll   = llist_create();
        hmap = hmap_create(&hmap_hash_fn, &hmap_eq_fn, 4096, 0.7f);
        heap = heap_create(&bt_cmp);

        printf("*** Insert ***\n");
        perf_insert(outer, inner);
        printf("*** Find ***\n");
        perf_find(outer, inner);
        printf("*** Rebalance ***\n");
        perf_rebalance(outer, inner);

        btree_destroy(bt);
        llist_destroy(ll);
        hmap_destroy(hmap);
        heap_destroy(heap);

        if (dummy == -1)
        {
                printf("Should not happen\n");
        }

        return 0;
}

void perf_insert(int outer, int inner)
{
        double mean;
        double sigma;

        for (int i = 0; i < outer; i++)
        {
                /* binary tree */
                for (int j = 0; j < inner; j++)
                {
                        unsigned long begin, dur;

                        begin = current_time_us();
                        btree_insert(bt, (void*)data[i * inner + j]);
                        dur = current_time_us() - begin;
                        dur_btree[i * inner + j] = (int)dur;
                }

                /* list */
                for (int j = 0; j < inner; j++)
                {
                        unsigned long begin, dur;

                        begin = current_time_us();
                        llist_pushb(ll, (void*)data[i * inner + j]);
                        dur = current_time_us() - begin;
                        dur_llist[i * inner + j] = (int)dur;
                }
                
                /* hash table */
                for (int j = 0; j < inner; j++)
                {
                        unsigned long begin, dur;

                        begin = current_time_us();
                        hmap_set(hmap,
                                 (void*)data[i * inner + j],
                                 (void*)data[i * inner + j]);
                        dur = current_time_us() - begin;
                        dur_hmap[i * inner + j] = (int)dur;
                }
                /* heap */
                for (int j = 0; j < inner; j++)
                {
                        unsigned long begin, dur;

                        begin = current_time_us();
                        heap_insert(heap, (void*)data[i * inner + j]);
                        dur = current_time_us() - begin;
                        dur_heap[i * inner + j] = (int)dur;
                }
        }

        printf("Size of tree: %lu\n", btree_size(bt));
        printf("Size of list: %lu\n", llist_size(ll));
        printf("Size of hmap: %lu\n", hmap_size(hmap));
        printf("Size of heap: %lu\n", heap_size(heap));

        gauss_dist(dur_btree, outer * inner, &mean, &sigma);
        printf("Binary tree (%u) insert: m: %.3fus s: %.3fus\n",
               btree_height(bt), mean, sigma);
        gauss_dist(dur_llist, outer * inner, &mean, &sigma);
        printf("Linked list insert:      m: %.3fus s: %.3fus\n",
               mean, sigma);
        gauss_dist(dur_hmap, outer * inner, &mean, &sigma);
        printf("Hash table insert:       m: %.3fus s: %.3fus\n",
               mean, sigma);
        gauss_dist(dur_heap, outer * inner, &mean, &sigma);
        printf("Heap insert:             m: %.3fus s: %.3fus\n", 
               mean, sigma);
}

void perf_find(int outer, int inner)
{
        double mean;
        double sigma;
        void* e;

        for (int i = 0; i < outer; i++)
        {
                /* binary tree */
                for (int j = 0; j < inner; j++)
                {
                        unsigned long begin, dur;

                        begin = current_time_us();
                        e = btree_find(bt, (void*)data[i * inner + j]);
                        dur = current_time_us() - begin;
                        dur_btree[i * inner + j] = (int)dur;
                        assert(e != NULL);
                }

                /* list */
                for (int j = 0; j < inner; j++)
                {
                        unsigned long begin, dur;
                        e = NULL;
                        begin = current_time_us();
                 
                        for (struct lnode* n = llist_head(ll); 
                             n != NULL; 
                             n = n->next)
                        {
                                if ((long)n->data == data[i * inner + j])
                                {
                                        dummy = (long)n->data;
                                        e = n->data;
                                        break;
                                }
                        }
                        dur = current_time_us() - begin;
                        dur_llist[i * inner + j] = (int)dur;
                        assert(e != NULL);
                }
                /* hash table */
                for (int j = 0; j < inner; j++)
                {
                        unsigned long begin, dur;

                        begin = current_time_us();
                        e = hmap_get(hmap, (void*)data[i * inner + j]);
                        dur = current_time_us() - begin;
                        dur_hmap[i * inner + j] = (int)dur;
                        assert(e != NULL);
                }
                /* heap */
                for (int j = 0; j < inner; j++)
                {
                        long f = i * inner + j + 1;
                        unsigned long begin, dur;
                        
                        begin = current_time_us();
                        e = heap_min(heap);
                        dur = current_time_us() - begin;
                        dur_heap[i * inner + j] = (int)dur;
                        assert(e == (void*)f);
                }
        }

        gauss_dist(dur_btree, outer * inner, &mean, &sigma);
        printf("Binary tree (%u) find:   m: %.3fus s: %.3fus\n", 
               btree_height(bt), mean, sigma);
        gauss_dist(dur_llist, outer * inner, &mean, &sigma);
        printf("Linked list find:        m: %.3fus s: %.3fus\n", 
               mean, sigma);
        gauss_dist(dur_hmap, outer * inner, &mean, &sigma);
        printf("Hash table find:         m: %.3fus s: %.3fus\n", 
               mean, sigma);
        gauss_dist(dur_heap, outer * inner, &mean, &sigma);
        printf("Heap min:                m: %.3fus s: %.3fus\n", 
               mean, sigma);
}

void perf_rebalance(int outer, int inner)
{
        double mean;
        double sigma;
        unsigned long begin, dur;
        unsigned int h = btree_height(bt);

        begin = current_time_us();
        btree_balance(bt);
        dur = current_time_us() - begin;
        printf("Balanced binary (%u->%u) tree in %ldus\n", 
               h, btree_height(bt), dur);

        for (int i = 0; i < outer; i++)
        {
                /* binary tree */
                for (int j = 0; j < inner; j++)
                {
                        void* e;

                        begin = current_time_us();
                        e = btree_find(bt, (void*)data[i * inner + j]);
                        dur = current_time_us() - begin;
                        dur_btree[i * inner + j] = (int)dur;
                        assert(e != NULL);
                }
        }

        gauss_dist(dur_btree, outer * inner, &mean, &sigma);
        printf("Binary tree (%u) find:   m: %.3fus s: %.3fus\n", 
               btree_height(bt), mean, sigma);
}

unsigned long current_time_us(void)
{
#ifdef NDEBUG
        struct timeval tv;
#else
        struct timeval tv = {
                0, 0
        };
#endif

        gettimeofday(&tv, NULL);
        
        return tv.tv_sec * 1000000 + tv.tv_usec;
}

int bt_cmp(const void* a, const void* b)
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

uint32_t hmap_hash_fn(void* v)
{
        return (uint32_t)(long)v;
}

int hmap_eq_fn(void* a, void* b)
{
        if (a == b)
        {
                return 0;
        }

        return 1;
}

void gauss_dist(int* s, int size, double* mean, double* sigma)
{
        double mu;

        *mean = 0;
        *sigma = 0;
        
        for (int i = 0; i < size; i++)
        {
                *mean += (double)s[i];
        }
        *mean /= (double)size;

        mu = (double)*mean;
        for (int i = 0; i < size; i++)
        {
                double diff = mu - (double)s[i];
                *sigma += diff * diff;
        }

        *sigma /= (double)size;        
        *sigma = sqrt(*sigma);
}
