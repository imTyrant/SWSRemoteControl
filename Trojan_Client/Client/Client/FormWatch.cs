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

namespace Client
{
    public delegate void SendMsg1(string  type, int mID, int res, string sendMsg);
    public delegate void SendMsg3(string type, int mID, int res, byte[] sendMsg);

    public partial class FormWatch : Form
    {
        private Socket testSocket = null;
        public int mID;
        public FormWatch(Socket socket,int mID1)
        {
            InitializeComponent();
            mID = mID1;
            this.testSocket = socket;
            Thread aa = new Thread(ServerMsg);
            aa.Start();
        }

        public event SendMsg1 sendMsg1;

        /// <summary>
        /// 获取时间
        /// </summary>
        /// <returns></returns>
        private DateTime GetCurrentTime()
        {
            DateTime currentTime = new DateTime();
            currentTime = DateTime.Now;
            return currentTime;
        }

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
            string strStrcMsg = Encoding.ASCII.GetString(buffer, 0, size);
            txtMsg.AppendText(strStrcMsg);
        }

        /// <summary>
        /// 发送指令
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnSend_Click(object sender, EventArgs e)
        {
            Thread aa = new Thread(ServerMsg);
            aa.Start();
            sendMsg1("A",mID,-1,txtDirect.Text.Trim());
            txtMsg.AppendText("返回信息" + GetCurrentTime() + "\r\n" + txtDirect.Text.Trim() + "\r\n");
        }

        /// <summary>
        /// 发送指令
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void txtDirect_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                Thread aa = new Thread(ServerMsg);
                aa.Start();
                sendMsg1("A", mID, -1, txtDirect.Text.Trim());
                txtMsg.AppendText("返回信息" + GetCurrentTime() + "\r\n" + txtDirect.Text.Trim() + "\r\n");
            }
        }

        /// <summary>
        /// 获取随机数
        /// </summary>
        /// <returns></returns>
        public static int GetRandomSequence0()
        {
            int[] index = new int[100];
            for (int i = 0; i < 100; i++)index[i] = i+6;
            Random r = new Random();
            int result;
            int site = 100;
            int id;
            id = r.Next(0, site - 1);
            result = index[id];
            return result;
            index[id] = index[site - 1];
            site--;
        }

        public event changecolor coloring;
        /// <summary>
        /// 关闭窗口
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void FormWatch_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (testSocket.Connected)
                sendMsg1("0", mID, -1, "");
            else
            {
                coloring(true);
            }
        }

    }
}
