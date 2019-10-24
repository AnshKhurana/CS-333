#include "simplefs-ops.h"

int main()
{
    simplefs_formatDisk();
    simplefs_create("file.txt");
    int fd = simplefs_open("file.txt");
    simplefs_close(fd);
    simplefs_dump();
    simplefs_delete("file.txt");
    simplefs_dump();
}