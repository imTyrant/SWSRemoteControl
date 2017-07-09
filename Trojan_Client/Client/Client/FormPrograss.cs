using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net.Sockets;
using System.Net;
using System.Threading;
using System.IO;

namespace Client
{
    public partial class FormPrograss : Form
    {
        private Socket testSocket = null;
        public int mID;
        /// <summary>
        /// 主窗口
        /// </summary>
        /// <param name="socket"></param>
        /// <param name="mID3"></param>
        public FormPrograss(Socket socket, int mID3)
        {
            InitializeComponent();
            mID = mID3;
            this.testSocket = socket;
            Thread aa = new Thread(ServerMsg);
            aa.Start();
        }

        public event SendMsg3 sendMsg3;

        /// <summary>
        /// 接受消息
        /// </summary>
        private void ServerMsg()
        {
            Socket socketServer = testSocket;
            byte[] buffer = new byte[1024];
            int rect = socketServer.Receive(buffer, 0, 1024, 0);
            int size = BitConverter.ToInt32(buffer, 0);
            int dataleft = size;
            buffer = new byte[size];
            int total = 0;
            while (total < size)   //当接收长度小于总长度时继续执行  
            {
                rect = socketServer.Receive(buffer, total, dataleft, 0);    //接收字节流，receive方法返回int获取已接收字节个数，第一个参数是需要写入的字节组，第二个参数是起始位置，第三个参数是接收字节的长度  
                total += rect;            //已接收个数-下一次从当前个数开始接收  
                dataleft -= rect;  //剩下的字节长度  
            }
            string strStrcMsg = Encoding.Unicode.GetString(buffer, 0, size);
            textBox1.AppendText(strStrcMsg + "\r\n");
        }



        private void button1_Click(object sender, EventArgs e)
        {
            int pid = Convert.ToInt32(txtpid.Text.Trim());
            byte[] buffer = new byte[4];
            buffer[0] = (byte)(pid);
            buffer[1] = (byte)(pid >> 8);
            buffer[2] = (byte)(pid >> 16);
            buffer[3] = (byte)(pid >> 24);
            sendMsg3("C", mID, -1, buffer);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            int index = 0;
            index = textBox1.Text.IndexOf(textBox2.Text, index);
            if (index < 0)
            {
                index = 0;
                textBox1.SelectionStart = 0;
                textBox1.SelectionLength = 0;
                MessageBox.Show("已到结尾");
                return;
            }
            textBox1.SelectionStart = index;
            textBox1.SelectionLength = textBox2.Text.Length;
            index = index + textBox2.Text.Length;
            textBox1.Focus();
        }
    }
}
