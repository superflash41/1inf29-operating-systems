#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// Saymon Estefano Nicho Manrique - 20211866

#define CLUSTER_EOF 0xFFFFFFFF
#define CLUSTER_MAX 0xFFFFFFF6
#define CLUSTER_MIN 0x00000003

typedef struct {
    char jump[3];
    char FSName[8];
    char Chunk[53];
    long offsetPart;
    long sizeVol;
    int FATOffset;
    int FATlen;
    int ClusterHeapOffset;
    int ClusterCount;
    int RootDirFirstCluster;
    int VSN;
    short FSR;
    short FlagVol;
    char BytePerSector;
    char SectorPerCluster;
    char NumberFats;
    char DriveSelect;
    char PercentUse;
    char reserved[7];
    char BootCode[390];
    short BootSignature;
} __attribute((packed)) exFatBootSector;

typedef char DirEntry[32];

int main(int argc, char *argv[]) {
    int fd, files=0, exp;
    exFatBootSector boot;
    if (argc != 2) {
        printf("Usage: %s <Image File System>\n", argv[0]);
        exit(1);
    }
    if ((fd = open(argv[1], O_RDONLY)) < 0)
        perror("Disk image could not be opened!\n");
    if (read(fd, &boot, sizeof(boot)) < 0)
        perror("Disk image could not be read!\n");

    size_t bytesPerCluster = pow(2.0, boot.BytePerSector + boot.SectorPerCluster);
    size_t rootDirOffset = boot.ClusterHeapOffset * pow(2.0, boot.BytePerSector) +
      (boot.RootDirFirstCluster - 2) * pow(2.0, boot.BytePerSector + boot.SectorPerCluster);
    if (lseek(fd, rootDirOffset, SEEK_SET) < 0) perror("Error in seek\n");
    if (lseek(fd, sizeof(DirEntry) * 3, SEEK_CUR) < 0)
        perror("Error in seek\n");

    printf("List fo files in the root directory:\n");
    printf("-----------------------------------------------\n");
    while (1) {
        DirEntry entry;
        if (read(fd, &entry, sizeof(DirEntry)) < 0)
            perror("Error reading the entry of the directory!\n");
        unsigned char entryType = entry[0];
        if (!entryType) break; // no more entries
        if (entryType != 0x85) continue; // not a file
        if (read(fd, &entry, sizeof(DirEntry)) < 0)
            perror("Error reading the entry of the directory!\n");
        unsigned char nameLength = entry[3];
        unsigned char left = nameLength;
        printf("%02d) File \"", ++files);
        unsigned int amountFileNameEntries = (nameLength + 15) / 15;
        for (size_t i = 0; i < amountFileNameEntries; i++) {
            if (read(fd, &entry, sizeof(DirEntry)) < 0)
                perror("Error reading the entry of the directory!");
            char* nameSection = &entry[2];
            unsigned int charsToPrint = 30;
            if (left*2 < charsToPrint) charsToPrint = left*2;
            for (unsigned int i=0; i<charsToPrint; i++)
                printf("%c", nameSection[i]);
            left -= charsToPrint;
        }
        printf("\"\n");
    }
    printf("-----------------------------------------------\n");
    printf("--> Total number of entries retrieved: %d\n", files);
    exit(0);
}
