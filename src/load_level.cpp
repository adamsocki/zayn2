

enum FileMode
{
    FileMode_Read,
    FileMode_Write,
};

struct FileHandle 
{
    FILE *file;

    FileMode mode;

    uint64 size;
    uint64 offset;

    u8 *data;
};

