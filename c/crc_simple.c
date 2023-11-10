#include <stdio.h>

const unsigned char CRC7_POLY = 0x91;
 
unsigned char getCRC(unsigned char message[], unsigned char length)
{
  unsigned char i, j, crc = 0;
 
  for (i = 0; i < length; i++)
  {
    crc ^= message[i];
    for (j = 0; j < 8; j++)
    {
      if (crc & 1)
        crc ^= CRC7_POLY;
      crc >>= 1;
    }
  }
  return crc;
}

// gcc -o Wpadded main example.c;./main
int main(){
    // create a message array that has one extra byte to hold the CRC:
    unsigned char message[3] = {0x83, 0x01, 0x00};
    message[2] = getCRC(message, 2);
    // send this message to the Simple Motor Controller
    printf("message2:%d\n", message[2]);
    return 0;
}

/*
以下示例程序显示了如何用 C 语言计算 CRC 字节。 外循环处理每个字节，内循环处理这些字节的每一位。 在示例 main() 例程中，这用于生成消息 0x83、0x01 中的 CRC 字节，这在第 6.5 节中使用。 getCRC() 函数无需修改即可在 Arduino 和 Orangutan 程序中使用。
*/