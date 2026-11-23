#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 8192

/* 1. PageHeaderData (24 bytes total) */
typedef struct PageHeaderData {
    uint64_t pd_lsn;              /* 8 bytes: Next byte after last WAL record */
    uint16_t pd_checksum;         /* 2 bytes: Page checksum */
    uint16_t pd_flags;            /* 2 bytes: Flag bits */
    uint16_t pd_lower;            /* 2 bytes: Offset to start of free space */
    uint16_t pd_upper;            /* 2 bytes: Offset to end of free space */
    uint16_t pd_special;          /* 2 bytes: Offset to start of special space */
    uint16_t pd_pagesize_version; /* 2 bytes: Page size and layout version */
    uint32_t pd_prune_xid;        /* 4 bytes: Oldest unpruned XMAX on page */
} PageHeaderData;

typedef PageHeaderData* PageHeader;

/* 2. ItemIdData (4 bytes total) */
typedef struct ItemIdData {
    unsigned int lp_off : 15;     /* 15 bits: Byte-offset to tuple (up to 32767) */
    unsigned int lp_flags : 2;    /* 2 bits: Status flags (0=unused, 1=used) */
    unsigned int lp_len : 15;     /* 15 bits: Byte length of tuple */
} ItemIdData;

typedef ItemIdData* ItemId;

/* 3. HeapTupleHeaderData (23 bytes fixed header) */
typedef struct __attribute__((packed)) HeapTupleHeaderData {
    uint32_t t_xmin;              /* 4 bytes: Insert XID */
    uint32_t t_xmax;              /* 4 bytes: Delete XID */
    uint32_t t_cid;               /* 4 bytes: Command ID */
    uint8_t  t_ctid[6];           /* 6 bytes: Current TID (block + item index) */
    uint16_t t_infomask2;         /* 2 bytes: Attribute count & flags */
    uint16_t t_infomask;          /* 2 bytes: Tuple flags */
    uint8_t  t_hoff;              /* 1 byte: Offset to user data */
} HeapTupleHeaderData;

/* Initialize a raw 8KB buffer as an empty page */
char* page_init(void) {
    char *raw_page = (char *)malloc(PAGE_SIZE);
    if (!raw_page) {
        return NULL;
    }

    memset(raw_page, 0, PAGE_SIZE);

    PageHeader header = (PageHeader)raw_page;
    header->pd_lower = sizeof(PageHeaderData); /* Starts at byte 24 */
    header->pd_upper = PAGE_SIZE;              /* Ends at byte 8192 (no special space) */
    header->pd_special = PAGE_SIZE;            /* No special space reserved */
    header->pd_pagesize_version = (uint16_t)(PAGE_SIZE | 4);

    return raw_page;
}

/* Helper to build a sample tuple: { int32_t id, char name[] } */
char* build_sample_tuple(uint32_t xmin, int32_t id, const char *name, uint16_t *out_len) {
    uint16_t name_len = (uint16_t)(strlen(name) + 1); /* Include '\0' */
    uint8_t hoff = 24;                                /* 23-byte header + 1 byte padding */
    uint16_t total_len = hoff + sizeof(int32_t) + name_len;

    char *buf = (char *)malloc(total_len);
    if (!buf) {
        return NULL;
    }
    memset(buf, 0, total_len);

    HeapTupleHeaderData *thdr = (HeapTupleHeaderData *)buf;
    thdr->t_xmin = xmin;
    thdr->t_xmax = 0;
    thdr->t_cid = 0;
    thdr->t_infomask2 = 2; /* 2 attributes: id and name */
    thdr->t_infomask = 0;
    thdr->t_hoff = hoff;

    char *payload = buf + thdr->t_hoff;
    memcpy(payload, &id, sizeof(int32_t));
    memcpy(payload + sizeof(int32_t), name, name_len);

    *out_len = total_len;
    return buf;
}

/* Insert a tuple into the 8KB page buffer; returns 1 on success, 0 on failure */
int insert_tuple(char *raw_page, const char *tuple_data, uint16_t tuple_len) {
    if (!raw_page || !tuple_data || tuple_len == 0) {
        return 0;
    }

    PageHeader header = (PageHeader)raw_page;
    uint16_t space_needed = (uint16_t)(tuple_len + sizeof(ItemIdData));

    if (header->pd_upper < header->pd_lower ||
        (header->pd_upper - header->pd_lower) < space_needed) {
        return 0; /* Not enough free space */
    }

    /* 1. Move pd_upper backward and copy tuple bytes */
    header->pd_upper -= tuple_len;
    memcpy(raw_page + header->pd_upper, tuple_data, tuple_len);

    /* 2. Write ItemId at current pd_lower */
    ItemId item = (ItemId)(raw_page + header->pd_lower);
    item->lp_off = header->pd_upper;
    item->lp_len = tuple_len;
    item->lp_flags = 1; /* Mark slot as used */

    /* 3. Advance pd_lower past the new 4-byte ItemId */
    header->pd_lower += sizeof(ItemIdData);

    return 1;
}

/* Scan and print all tuples on the page */
void print_page_tuples(const char *raw_page) {
    const PageHeader header = (const PageHeader)raw_page;
    uint16_t item_count = (header->pd_lower - sizeof(PageHeaderData)) / sizeof(ItemIdData);

    printf("Page state -> pd_lower: %u | pd_upper: %u | items: %u\n",
           header->pd_lower, header->pd_upper, item_count);

    for (uint16_t i = 0; i < item_count; i++) {
        const ItemId item = (const ItemId)(raw_page + sizeof(PageHeaderData) + (i * sizeof(ItemIdData)));
        if (item->lp_flags == 0 || item->lp_len == 0) {
            continue;
        }

        const char *tuple_ptr = raw_page + item->lp_off;
        const HeapTupleHeaderData *thdr = (const HeapTupleHeaderData *)tuple_ptr;
        const char *payload = tuple_ptr + thdr->t_hoff;

        int32_t id = 0;
        memcpy(&id, payload, sizeof(int32_t));
        const char *name = payload + sizeof(int32_t);

        printf("  [Item %u] off=%u len=%u xmin=%u -> id=%d, name=\"%s\"\n",
               i, item->lp_off, item->lp_len, thdr->t_xmin, id, name);
    }
}

int main(void) {
    char *page = page_init();
    if (!page) {
        perror("Failed to allocate page");
        return 1;
    }

    uint16_t len1 = 0, len2 = 0;
    char *t1 = build_sample_tuple(100, 1, "Alice", &len1);
    char *t2 = build_sample_tuple(101, 2, "Bob", &len2);

    insert_tuple(page, t1, len1);
    insert_tuple(page, t2, len2);

    print_page_tuples(page);

    free(t1);
    free(t2);
    free(page);
    return 0;
}