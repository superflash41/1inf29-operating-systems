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

int getBit(char var, int n) { return (var & (1 << n)) > 0; }

int main(int argc, char *argv[]) {
    int fd, exp;
    exFatBootSector boot;
    if (argc != 3) {
        printf("Usage: %s <Image File System> <Cluster number>\n", argv[0]);
        exit(1);
    }

    if ((fd = open(argv[1], O_RDONLY)) < 0)
        perror("Disk image could not be opened!\n");
    if (read(fd, &boot, sizeof(boot)) < 0)
        perror("Disk image could not be read!\n");
    unsigned int FAT[boot.ClusterCount];
    size_t FAToffset = boot.FATOffset * (int)pow(2.0, boot.BytePerSector);
    if (lseek(fd, FAToffset, SEEK_SET) < 0) perror("Error in seek\n");
    unsigned int mediaType;
    if (read(fd, &mediaType, sizeof(mediaType)) < 0) perror("Error reading mediatype!\n");
    unsigned int FATconstant;
    if (read(fd, &FATconstant, sizeof(FATconstant)) < 0)
        perror("Error reading FATconstant!\n");
    if (read(fd, &FAT, sizeof(FAT)) < 0) perror("FAT could not be read\n");

    int selectedCluster = atoi(argv[2]);
    size_t clusterSizeInBytes = pow(2.0, boot.SectorPerCluster + boot.BytePerSector);
    size_t allocationBitMapOffset = boot.ClusterHeapOffset * pow(2.0, boot.BytePerSector);

    unsigned char allocationBitMap[boot.ClusterCount];
    if (lseek(fd, allocationBitMapOffset, SEEK_SET) < 0) perror("Error in seek\n");
    if (read(fd, allocationBitMap, sizeof(allocationBitMap)) < 0)
        perror("Error reading the Allocation Bitmap\n");
    int bitOffset = (selectedCluster - 2) % 8;
    int byteOffset = (selectedCluster - 2) / 8;
    int isAllocated = getBit(allocationBitMap[byteOffset], bitOffset);
    if (getBit(allocationBitMap[byteOffset], bitOffset))
        printf("Cluster %d is occuppied.\n", selectedCluster);
    else printf("Cluster %d is free\n", selectedCluster);
    exit(0);
}
