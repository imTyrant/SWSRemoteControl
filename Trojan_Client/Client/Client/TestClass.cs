using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Windows.Forms;

namespace Client
{
    class TestClass //extend
    {
        private Socket netClientSocket = null;
        private Form father;
        public TestClass(Socket initSocket, Form main)
        {
            this.netClientSocket = initSocket;
            this.father = main;
        }

        public void netSendData(byte[] sendBuffer)
        {
            netClientSocket.Send(sendBuffer);
            if (netClientSocket.Connected)
            {

            }
        }
    }
}
