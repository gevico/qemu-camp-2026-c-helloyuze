#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 16 LRU 缓存淘汰算法（哈希表 + 双向链表）
 *  - put(k,v)、get(k) 均为 O(1)
 *  - 容量满时淘汰最久未使用（LRU）的元素
 */

typedef struct LRUNode {
    int key;
    int value;
    struct LRUNode* prev;
    struct LRUNode* next;
} LRUNode;

typedef struct HashEntry {
    int key;
    LRUNode* node;
    struct HashEntry* next;
} HashEntry;

typedef struct {
    int capacity;
    int size;
    LRUNode* head; /* 最近使用（MRU） */
    LRUNode* tail; /* 最久未使用（LRU） */
    /* 简单链式哈希表 */
    size_t bucket_count;
    HashEntry** buckets;
} LRUCache;

static unsigned hash_int(int key) {
    unsigned x = (unsigned)key;
    x ^= x >> 16;
    x *= 0x7feb352dU;
    x ^= x >> 15;
    x *= 0x846ca68bU;
    x ^= x >> 16;
    return x;
}

static HashEntry* hash_find(LRUCache* c, int key, HashEntry*** pprev_next) {
    if (c == NULL || c->bucket_count == 0) {
        return NULL;
    }

    size_t idx = hash_int(key) % c->bucket_count;
    HashEntry** cur = &c->buckets[idx];
    while (*cur != NULL) {
        if ((*cur)->key == key) {
            if (pprev_next != NULL) {
                *pprev_next = cur;
            }
            return *cur;
        }
        cur = &(*cur)->next;
    }
    if (pprev_next != NULL) {
        *pprev_next = cur;
    }
    return NULL;
}

static void list_add_to_head(LRUCache* c, LRUNode* node) {
    node->prev = NULL;
    node->next = c->head;
    if (c->head != NULL) {
        c->head->prev = node;
    }
    c->head = node;
    if (c->tail == NULL) {
        c->tail = node;
    }
}

static void list_remove(LRUCache* c, LRUNode* node) {
    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        c->head = node->next;
    }

    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        c->tail = node->prev;
    }
    node->prev = NULL;
    node->next = NULL;
}

static void list_move_to_head(LRUCache* c, LRUNode* node) {
    if (c->head == node) {
        return;
    }
    list_remove(c, node);
    list_add_to_head(c, node);
}

static LRUNode* list_pop_tail(LRUCache* c) {
    LRUNode* node = c->tail;
    if (node != NULL) {
        list_remove(c, node);
    }
    return node;
}

/* LRU 接口实现 */
static LRUCache* lru_create(int capacity) {
    if (capacity <= 0) {
        return NULL;
    }

    LRUCache* c = calloc(1, sizeof(*c));
    if (c == NULL) {
        return NULL;
    }
    c->capacity = capacity;
    c->bucket_count = 64;
    c->buckets = calloc(c->bucket_count, sizeof(*c->buckets));
    if (c->buckets == NULL) {
        free(c);
        return NULL;
    }
    return c;
}

static void lru_free(LRUCache* c) {
    if (c == NULL) {
        return;
    }

    LRUNode* node = c->head;
    while (node != NULL) {
        LRUNode* next = node->next;
        free(node);
        node = next;
    }

    for (size_t i = 0; i < c->bucket_count; ++i) {
        HashEntry* entry = c->buckets[i];
        while (entry != NULL) {
            HashEntry* next = entry->next;
            free(entry);
            entry = next;
        }
    }
    free(c->buckets);
    free(c);
}

static int lru_get(LRUCache* c, int key, int* out_value) {
    HashEntry* entry = hash_find(c, key, NULL);
    if (entry == NULL) {
        return 0;
    }
    if (out_value != NULL) {
        *out_value = entry->node->value;
    }
    list_move_to_head(c, entry->node);
    return 1;
}

static void lru_put(LRUCache* c, int key, int value) {
    if (c == NULL) {
        return;
    }

    HashEntry** prev_next = NULL;
    HashEntry* entry = hash_find(c, key, &prev_next);
    if (entry != NULL) {
        entry->node->value = value;
        list_move_to_head(c, entry->node);
        return;
    }

    LRUNode* node = malloc(sizeof(*node));
    if (node == NULL) {
        return;
    }
    node->key = key;
    node->value = value;
    node->prev = NULL;
    node->next = NULL;

    entry = malloc(sizeof(*entry));
    if (entry == NULL) {
        free(node);
        return;
    }
    entry->key = key;
    entry->node = node;

    size_t idx = hash_int(key) % c->bucket_count;
    entry->next = c->buckets[idx];
    c->buckets[idx] = entry;
    list_add_to_head(c, node);
    c->size++;

    if (c->size > c->capacity) {
        LRUNode* old = list_pop_tail(c);
        if (old != NULL) {
            HashEntry** old_prev_next = NULL;
            HashEntry* old_entry = hash_find(c, old->key, &old_prev_next);
            if (old_entry != NULL && old_prev_next != NULL) {
                *old_prev_next = old_entry->next;
                free(old_entry);
            }
            free(old);
            c->size--;
        }
    }
}

/* 打印当前缓存内容（从头到尾） */
static void lru_print(LRUCache* c) {
    LRUNode* p = c->head;
    int first = 1;
    while (p) {
        if (!first) printf(", ");
        first = 0;
        printf("%d:%d", p->key, p->value);
        p = p->next;
    }
    printf("\n");
}

int main(void) {
    /* 容量 3：put(1,1), put(2,2), put(3,3), put(4,4), get(2), put(5,5) */
    LRUCache* c = lru_create(3);
    if (!c) {
        fprintf(stderr, "创建 LRU 失败\n");
        return 1;
    }

    lru_put(c, 1, 1); /* 缓存：1 */
    lru_put(c, 2, 2); /* 缓存：2,1 */
    lru_put(c, 3, 3); /* 缓存：3,2,1 (满) */
    lru_put(c, 4, 4); /* 淘汰 LRU(1)，缓存：4,3,2 */

    int val;
    if (lru_get(c, 2, &val)) {
        /* 访问 2：缓存：2,4,3 */
        (void)val; /* 演示无需使用 */
    }

    lru_put(c, 5, 5); /* 淘汰 LRU(3)，缓存：5,2,4 */

    /* 期望最终键集合：{2,4,5}，顺序无关。此处按最近->最久打印：5:5, 2:2, 4:4 */
    lru_print(c);

    lru_free(c);
    return 0;
}
