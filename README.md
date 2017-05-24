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
## **思路（更新）**

**keywords:** &nbsp; `重定向` &nbsp;&nbsp; `管道 ` &nbsp;&nbsp; `Telent`<br><br>
应该是通过开启一个CMD之后再利用管道来将操作转到CMD上吧主要的思路还是来自与[木马教程](https://lellansin.wordpress.com/tutorials/hello_trojan/)的方式吧。