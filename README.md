# 串口助手

**本串口助手制作初衷是用于学习Qt, 以及为实验室学弟做一款PID实时调参软件**

## 1. 串口助手

**本串口助手仅完成最基本的发送接收功能, 还有以下功能未实现或有bug**

* **16进制发送**
* **发送新行**
* **当接收数据过多就会导致卡顿**

## 2. 波形助手

**该模块的波形显示模块引用了开源的[qcustomplot](https://www.qcustomplot.com/ )**

**波形数据采用以下协议**

|   字节    |            意义             |
| :-------: | :-------------------------: |
| head/帧头 |         1byte[0xA5]         |
|   16*N    | 16 * sizeof(N) 16个数据通道 |
| tail/帧尾 |         1byte[0x5A]         |

***N 为可选的数据类型, 支持int8/int16/float32/float64***

**波形助手也存在一些问题如下:**

* **接收帧数太快会导致卡顿**
* **CPU占用过高**
* **无法保存波形数据/从文件加载波形**
* **查看X轴历史波形**

## 3. PID调参



