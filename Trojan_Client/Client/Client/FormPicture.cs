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
using System.Drawing.Imaging;
using System.Runtime.InteropServices;

namespace Client
{
    
    
    public partial class FormPicture : Form
    {
        private Socket testSocket = null;
        public int mID;
        public FormPicture(Socket socket,int mID2)
        {
            InitializeComponent();
            mID = mID2;
            this.testSocket = socket;
            Thread aa = new Thread(ServerMsg);
            aa.Start();

        }

        private void ServerMsg()
        /*{
            byte[] b = new byte[4096];
            MemoryStream fs = new MemoryStream();
            NetworkStream ns = new NetworkStream(testSocket);
            int length = 0;
            try
            {
                Thread.Sleep(30000);
                if (ns.CanRead)
                {
                    while (ns.DataAvailable)
                    {
                        length = ns.Read(b, 0, b.Length);
                        fs.Write(b, 0, length);
                    }
                MessageBox.Show("1");
                }
            
                Bitmap Img = new Bitmap(fs);
                Img.Save(@"01.bmp", ImageFormat.Bmp);
                fs.Flush();
                fs.Close();
                }
            catch (Exception e)
            {
                //Bitmap Img = new Bitmap(fs);
                //Img.Save(@"01.bmp", ImageFormat.Bmp);
                //fs.Flush();
                //fs.Close();
                MessageBox.Show(e.Message);
                label1.BackColor = Color.Red;
            }
        }*/
        {
            while (true)
            {
                //FileStream wrtr = new FileStream(@"F:\GIT\SWSRemoteControl\Client\Client\bin\Debug" + "//test.bmp", FileMode.Create);
                MemoryStream fs = new MemoryStream();
                Socket socketServer = testSocket;
                NetworkStream ns = new NetworkStream(socketServer);
                byte[] buffer = new byte[1024];
                int rect = socketServer.Receive(buffer, 0, 1024, 0);
                int size = BitConverter.ToInt32(buffer, 0);
                int dataleft = size;
                buffer = new byte[size];
                int total = 0;
                while (total < size)   //当接收长度小于总长度时继续执行  
                {
                    rect = socketServer.Receive(buffer, total, dataleft, 0);    //接收字节流，receive方法返回int获取已接收字节个数，第一个参数是需要写入的字节组，第二个参数是起始位置，第三个参数是接收字节的长度  
                    textBox1.AppendText(rect.ToString() + "\n");
                    total += rect;            //已接收个数-下一次从当前个数开始接收  
                    dataleft -= rect;  //剩下的字节长度  
                }
                fs.Write(buffer, 0, buffer.Length); //输出文件  
                Bitmap Img = new Bitmap(fs);
                fs.Flush();  //强制输出  
                fs.Close();  //关闭文件流对象
                pictureBox1.Image = Img;
                Thread.Sleep(100);
                break;
            }
        }
        //        while (true)
        //        {
        //            if (close)
        //            {
        //                goto _QUIT;
        //            }
                        
        //            if (click)
        //                break;
        //        }
        //    }
        //_QUIT:
        //    return;
        //} 
        
        public event changecolor coloring;
        public event SendMsg1 sendMsg2;

        /// <summary>
        /// 抓屏按钮
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button1_Click(object sender, EventArgs e)
        {
            Thread temp = new Thread(ServerMsg);
            temp.Start();
            sendMsg2("B", mID, -1, "");
            //click = true;
        }

        /// <summary>
        /// 关闭窗口
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void FormPicture_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (testSocket.Connected)
            {
                //close = true;
                sendMsg2("0", mID, -1, "");
                //System.Environment.Exit(System.Environment.ExitCode);
            }
            else
            {
                coloring(true);
            }
 
        }

    }
}
