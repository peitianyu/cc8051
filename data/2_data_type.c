char _char = 0x89;
unsigned char _uchar = 0b10100011;
signed char _schar = 0xff;
bit _bit = false;
int _int = -1;
unsigned int _uint = 1;
sfr16 _sfr16 = 0xff00; 
sfr _sfr = 0x80;
sbit _sbit = 0x80^1;
idata int _idata;
xdata char _xdata;

int* _ptr = NULL;
char _arr[] = {1, 2, 3, 4};
int _arr2[4] = {1, 2, 3, 4};

// 默认为uint8_t
enum Enum { ONE=1, TOW};
enum Enum _enum = ONE;

struct Struct { int _a; char _b; };
struct Struct _struct = {1, 'a'};

union Union {
    unsigned char byte;
    struct bits{
        unsigned char mode : 4;
        unsigned char ctrl : 4;
    } _bits;
};
union Union _union = {0xff};