char _char = 0x89;
unsigned char _uchar = 0b10100011;
bit _bit = false;
int _int = -1;
unsigned int uint = 6;
sfr16 _sfr16 = 0xff00; 
sfr _sfr = 0x80;
sbit _sbit = _sfr^1;
idata int _idata;
xdata char _xdata;

int* _ptr = {0, 1, 2, 3};
char _arr[] = {'1', '2', '3', '4'};
int _arr2[4] = {1, 2, 3, 4};

// 默认为uint8_t
enum Enum {
    ONE=1,
    TOW
};
enum Enum _enum = ONE;

struct Struct {
    int a;
    char b;
};
struct Struct _struct = {1, 'a'};

union TimerControl {
    unsigned char byte;  // 整个控制寄存器作为一个字节
    struct bits{
        unsigned char mode : 2;  // 模式位，占用2位
        unsigned char clock_source : 1;  // 时钟源位，占用1位
        unsigned char overflow_interrupt_enable : 1;  // 溢出中断使能位，占用1位
        unsigned char start_stop : 1;  // 启动/停止位，占用1位
        unsigned char reserved : 3;  // 保留位，不使用
    } bits;
};
union TimerControl _timer_control;