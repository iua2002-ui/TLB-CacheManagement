#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TLB_SIZE 16
#define PAGE_TABLE_SIZE 16
#define CACHE_SIZE 64
#define BYTE_SIZE 256

// Structure for TLB entry
typedef struct {
    int setIndex;
    int tag;
    int ppn;
} TLBEntry;

// Structure for Page Table entry
typedef struct {
    int vpn;
    int ppn;
} PageTableEntry;

// Structure for Cache entry
typedef struct {
    int cacheIndex;
    int tag;
    int byteOffsets[4];
} CacheEntry;

// Function to find a TLB entry
int findTLBEntry(TLBEntry tlb[], int setIndex, int tag) {
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb[i].setIndex == setIndex && tlb[i].tag == tag) {
            return tlb[i].ppn;
        }
    }
    return -1;  // Entry not found
}

// Function to find a Page Table entry
int findPageTableEntry(PageTableEntry pageTable[], int vpn) {
    for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
        if (pageTable[i].vpn == vpn) {
            return pageTable[i].ppn;
        }
    }
    return -1;  // Entry not found
}

// Function to find a Cache entry
int findCacheEntry(CacheEntry cache[], int cacheIndex, int tag) {
    for (int i = 0; i < CACHE_SIZE; i++) {
        if (cache[i].cacheIndex == cacheIndex && cache[i].tag == tag) {
            return i;  // Return the cache entry index
        }
    }
    return -1;  // Entry not found
}

// Function to read TLB entries from input file
void readTLB(FILE *file, TLBEntry tlb[]) {
    char line[100];
    char recordType[10];
    int setIndex, tag, ppn;

    printf("Reading TLB entries:\n");

    while (fgets(line, sizeof(line), file) != NULL) {
        printf("Read TLB Entry: %s", line);  // Add this line to print the line read
        if (sscanf(line, "%9[^,],%d,%x,%x", recordType, &setIndex, &tag, &ppn) == 4) {
            if (strcmp(recordType, "TLB") == 0 && setIndex >= 0 && setIndex < TLB_SIZE) {
                tlb[setIndex].setIndex = setIndex;
                tlb[setIndex].tag = tag;
                tlb[setIndex].ppn = ppn;
            } else {
                fprintf(stderr, "Invalid TLB entry: %s", line);
            }
        } else {
            fprintf(stderr, "Invalid TLB entry format: %s", line);
        }
    }
}

//readPageTable function
void readPageTable(FILE *file, PageTableEntry pageTable[]) {
    char line[100];
    int vpn, ppn;

    printf("Reading Page Table entries:\n");

    while (fgets(line, sizeof(line), file) != NULL) {
        printf("Read Page Table Entry: %s", line);  // Add this line to print the line read
        if (sscanf(line, "Page,%d,%x", &vpn, &ppn) == 2) {
            if (vpn >= 0 && vpn < PAGE_TABLE_SIZE) {
                pageTable[vpn].vpn = vpn;
                pageTable[vpn].ppn = ppn;
            } else {
                fprintf(stderr, "Invalid Page Table entry: %s", line);
            }
        } else {
            fprintf(stderr, "Invalid Page Table entry format: %s", line);
        }
    }
}

void readCache(FILE *file, CacheEntry cache[]) {
    char line[100];
    char recordType[10];
    int cacheIndex, tag, byteOffsets[4];

    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%s,%d,%x,%x,%x,%x", recordType, &cacheIndex, &tag, &byteOffsets[0], &byteOffsets[1], &byteOffsets[2]) == 6) {
            if (strcmp(recordType, "Cache") == 0 && cacheIndex >= 0 && cacheIndex < CACHE_SIZE) {
                cache[cacheIndex].cacheIndex = cacheIndex;
                cache[cacheIndex].tag = tag;
                memcpy(cache[cacheIndex].byteOffsets, byteOffsets, sizeof(byteOffsets));
            } else {
                fprintf(stderr, "Invalid Cache entry: %s", line);
            }
        } else {
            fprintf(stderr, "Invalid Cache entry format: %s", line);
        }
    }
}

int main() {
    TLBEntry tlb[TLB_SIZE] = {0};
    PageTableEntry pageTable[PAGE_TABLE_SIZE] = {0};
    CacheEntry cache[CACHE_SIZE] = {0};

    FILE *file = fopen("Project4Input.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    // Read TLB entries from file
    readTLB(file, tlb);

    // Read Page Table entries from file
    readPageTable(file, pageTable);

    // Read Cache entries from file
    readCache(file, cache);

    fclose(file);

    for (int i = 0; i < 3; i++) {
        printf("Enter Virtual Address: ");

        char virtualAddressStr[5];
        if (scanf("%s", virtualAddressStr) != 1) {
            printf("Invalid input format\n");
            return 1;
        }

        int virtualAddress;
        if (sscanf(virtualAddressStr, "%x", &virtualAddress) != 1) {
            printf("Invalid input format\n");
            return 1;
        }

        // Extract set index, tag, and offset from virtual address
        int setIndex = (virtualAddress >> 4) & 0xF;
        int tag = (virtualAddress >> 8) & 0xFF;
        int offset = virtualAddress & 0xF;

        // Find TLB entry
        int tlbResult = findTLBEntry(tlb, setIndex, tag);

        if (tlbResult != -1) {
            // TLB hit
            int ppn = tlbResult;
            // Check if the corresponding cache entry exists
            int cacheIndex = (ppn >> 2) & 0x3F;
            int cacheTag = (ppn >> 6) & 0xFFFF;
            int cacheEntryIndex = findCacheEntry(cache, cacheIndex, cacheTag);

            if (cacheEntryIndex != -1) {
                // Cache hit
                printf("Byte at Virtual Address %x: %02x\n", virtualAddress, cache[cacheEntryIndex].byteOffsets[offset]);
            } else {
                // Cache miss
                printf("Cannot be determined (Cache miss)\n");
            }
        } else {
            // TLB miss
            // Find the corresponding Page Table entry
            int vpn = (virtualAddress >> 4);
            int pageTableResult = findPageTableEntry(pageTable, vpn);

            if (pageTableResult != -1) {
                // Page Table hit
                int ppn = pageTableResult;
                // Check if the corresponding cache entry exists
                int cacheIndex = (ppn >> 2) & 0x3F;
                int cacheTag = (ppn >> 6) & 0xFFFF;
                int cacheEntryIndex = findCacheEntry(cache, cacheIndex, cacheTag);

                if (cacheEntryIndex != -1) {
                    // Cache hit
                    printf("Byte at Virtual Address %x: %02x\n", virtualAddress, cache[cacheEntryIndex].byteOffsets[offset]);
                } else {
                    // Cache miss
                    printf("Cannot be determined (Cache miss)\n");
                }
            } else {
                // Page Table miss
                printf("Cannot be determined (Page Table miss)\n");
            }
        }
    }

    return 0;
}
