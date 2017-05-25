# **Sofeware Security Assignment** <br> Remote Control


## `如果没有什么意见的话主要功能部分就使用C/C++`
**功能的话用dll来拓展？然后如果需要图形界面的编辑就用C#或者Java来实现**<br>
(如果进度顺利而且XDDT的话)<br><br><br>
# 正文
> **参考资料**
>* [Telnet资料1，附基于C#的实现](http://blog.csdn.net/whatday/article/details/50541582)<br>
>* [Telnet资料2](http://www.cnblogs.com/liang-ling/p/5833489.html)<br>
>* [木马详细教程](https://lellansin.wordpress.com/tutorials/hello_trojan/)<br>
>* [进程通信：管道1](http://blog.csdn.net/morewindows/article/details/7390350)<br>
>* [进程通信：管道2](http://blog.csdn.net/houkai6/article/details/8613082)<br>
>* [句柄](http://blog.csdn.net/wenzhou1219/article/details/17659485)<br>
***

## **思路（更新）：**
1. 怎样和让服务端（安装木马端）和客户端（控制木马端）进行沟通..考虑以下两种方式：
    * 直接使用管道的远程的远程调用的方式来实现
    * 或者使用Socket的方式来实现两个主机之间的通信<br>



## **现在有的问题：**
1. 主要还是怎样去让发过去的数据报实施到用户的主机上？
```C++
#include<Windows.h>

system("COMANDS");
```
或者是通过创建一个隐藏的CMD的子进程然后利用管道来输入命令？
```C++
#include<Windows.h>
CreateProcess();
``` 
2. 用户权限的问题，如果只是一个win32程序的话只能是Ring3，权限很低，怎么获得高权限？
    * 漏洞？
    * 其他的方式？ 