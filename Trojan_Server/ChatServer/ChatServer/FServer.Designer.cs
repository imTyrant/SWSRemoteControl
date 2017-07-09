namespace ChatServer
{
    partial class FServer
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.txtSendMsg = new System.Windows.Forms.TextBox();
            this.btnSendMsg = new System.Windows.Forms.Button();
            this.txtPORT = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.txtMsg = new System.Windows.Forms.TextBox();
            this.txtIP = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.btnServerConn = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.So = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // txtSendMsg
            // 
            this.txtSendMsg.Location = new System.Drawing.Point(103, 249);
            this.txtSendMsg.Name = "txtSendMsg";
            this.txtSendMsg.Size = new System.Drawing.Size(303, 21);
            this.txtSendMsg.TabIndex = 12;
            this.txtSendMsg.KeyDown += new System.Windows.Forms.KeyEventHandler(this.txtSendMsg_KeyDown);
            // 
            // btnSendMsg
            // 
            this.btnSendMsg.Location = new System.Drawing.Point(430, 247);
            this.btnSendMsg.Name = "btnSendMsg";
            this.btnSendMsg.Size = new System.Drawing.Size(75, 23);
            this.btnSendMsg.TabIndex = 11;
            this.btnSendMsg.Text = "发送信息";
            this.btnSendMsg.UseVisualStyleBackColor = true;
            this.btnSendMsg.Click += new System.EventHandler(this.btnSendMsg_Click);
            // 
            // txtPORT
            // 
            this.txtPORT.Location = new System.Drawing.Point(310, 40);
            this.txtPORT.Name = "txtPORT";
            this.txtPORT.Size = new System.Drawing.Size(84, 21);
            this.txtPORT.TabIndex = 10;
            this.txtPORT.Text = "1";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(257, 43);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(29, 12);
            this.label2.TabIndex = 6;
            this.label2.Text = "PORT";
            // 
            // txtMsg
            // 
            this.txtMsg.Location = new System.Drawing.Point(98, 89);
            this.txtMsg.Multiline = true;
            this.txtMsg.Name = "txtMsg";
            this.txtMsg.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.txtMsg.Size = new System.Drawing.Size(303, 134);
            this.txtMsg.TabIndex = 9;
            // 
            // txtIP
            // 
            this.txtIP.Location = new System.Drawing.Point(103, 40);
            this.txtIP.Name = "txtIP";
            this.txtIP.Size = new System.Drawing.Size(120, 21);
            this.txtIP.TabIndex = 8;
            this.txtIP.Text = "192.168.0.3";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(57, 43);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(17, 12);
            this.label1.TabIndex = 7;
            this.label1.Text = "IP";
            // 
            // btnServerConn
            // 
            this.btnServerConn.Location = new System.Drawing.Point(427, 40);
            this.btnServerConn.Name = "btnServerConn";
            this.btnServerConn.Size = new System.Drawing.Size(78, 23);
            this.btnServerConn.TabIndex = 5;
            this.btnServerConn.Text = "启动服务";
            this.btnServerConn.UseVisualStyleBackColor = true;
            this.btnServerConn.Click += new System.EventHandler(this.btnServerConn_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(39, 92);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(53, 12);
            this.label3.TabIndex = 13;
            this.label3.Text = "聊天内容";
            // 
            // So
            // 
            this.So.AutoSize = true;
            this.So.Location = new System.Drawing.Point(33, 252);
            this.So.Name = "So";
            this.So.Size = new System.Drawing.Size(59, 12);
            this.So.TabIndex = 14;
            this.So.Text = "So-flash:";
            // 
            // FServer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(528, 318);
            this.Controls.Add(this.So);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.txtSendMsg);
            this.Controls.Add(this.btnSendMsg);
            this.Controls.Add(this.txtPORT);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.txtMsg);
            this.Controls.Add(this.txtIP);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btnServerConn);
            this.Name = "FServer";
            this.Text = "服务端";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox txtSendMsg;
        private System.Windows.Forms.Button btnSendMsg;
        private System.Windows.Forms.TextBox txtPORT;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox txtMsg;
        private System.Windows.Forms.TextBox txtIP;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnServerConn;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label So;
    }
}

