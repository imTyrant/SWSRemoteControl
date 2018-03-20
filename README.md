# **Sofeware Security Assignment** <br> 
## Remote Control & Trojan
```本项目实现了《软件安全》课程的最终作业创建。```<br>
## 文件目录
- SWSRemoteControl
    - Trojan_Server -- 木马的被控制端 C++
        - control_message.h -- 远程交互控制信息头文件
        - gloabe_data.h -- 全局变量都文件
        - server_pic_shoot.h
        - server_pic_shoot.cpp  -- 截图系统
        - server_process_find.h
        - server_process_find.cpp -- 进程控制系统
        - server_process_cmd.h
        - server_process_cmd.h -- 远程终端系统
        - MainEntry.cpp -- 主程序入口
    - Trojan_Client -- 木马的控制端 C#
        - This author of this part is too lazy, he didn't leave any message to both you and us :)
        - 这部分的作者太懒了，什么也没留下来...
    - Trojan_Detect -- 木马检测 C++
        - ProcessFind.h 
        - ProcessFind.cpp -- 木马检出程序
        - Trojan_Detect.cpp -- 主程序
***

## 介绍
1. 系统软件模型<br>

本系统的通信模型，为C/S结构，下图（图1）展示的是本系统所使用的网络通信模型。
<center>

![](https://github.com/imTyrant/SWSRemoteControl/raw/master/pics/pic1.png)<br>
（图1）
</center >
服务端，在被害者电脑处，完成三个主要功能：<br>

>为控制端开启一个 CMD（控制台），同时实时的与控制端进行数据交互;<br>
>监控被控制端的进程，如有需要，控制端可以选择关闭一个特定的进程;<br>
>如有需要，被控制端会监控受害主机的电脑屏幕，并回传到控制端，并实时显示在客户端。<br>

下图是本系统的功能模块图（图2）。<br>
<center>

![](https://github.com/imTyrant/SWSRemoteControl/raw/master/pics/pic2.png)<br>
（图2）
</center >

2. 系统设计<br>

木马设计思路：<br>
<center>

![](https://github.com/imTyrant/SWSRemoteControl/raw/master/pics/pic3.png)<br>
（图3）
</center >
图3中是木马程序在运行中控制图。木马首先启动时，运行主线程作为管理线程（Monitor）管理整个系统的数据流向。<br>
系统刚刚开启时，Monitor先初始化网络连接，尝试连接控制端，如果未成功连接客户端，则休眠线程5秒之后重新尝试连接。如果成功与客户端建立连接，则开启两个线程分别作为接受以及发送线程。每当收到客户端发送的指令时，Monitor开启一个线程，并作为一个中间人，在Socket与新开启的线程之间转发数据，从而维护数据的完整性。主要过程在下图中（图4）展示。
<center>

![](https://github.com/imTyrant/SWSRemoteControl/raw/master/pics/pic4.png)<br>
（图4）<br>
</center>
为了保证数据转发的完整性，Monitor在启动时，维护了一个全局变量，来做为一个线程池（如图5）。
<center>

![](https://github.com/imTyrant/SWSRemoteControl/raw/master/pics/pic5.png)<br>
（图5）<br>
</center>

## 碎碎念
TKS. TO CHEN BAO LU FAN<br>
![](https://github.com/imTyrant/SWSRemoteControl/raw/master/pics/result.png)

