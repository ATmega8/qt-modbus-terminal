#基于Qt的Modbus主机

##用法：
1. 使用最右边的按钮配置串口
2. 使用最左边的按钮打开串口
3. 使用左起第三个按钮发送MODBUS请求，目前只支持Read Holding Register (0x03) 与 Write Single Register (0x06) 指令
3.1 Read Holding Register (0x03) 指令分别要填入从机地址，寄存器地址与读取长度，以上数据均为16进制
3.2 Write Single Register (0x06) 指令分别要填入从机地址，寄存器地址与写入内容，以上数据均为16进制
4. 使用左起第二个按钮可以关闭串口，当程序结束时也会自动关闭串口
