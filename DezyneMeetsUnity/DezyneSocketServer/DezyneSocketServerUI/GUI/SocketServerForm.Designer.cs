namespace DezyneSocketServerUI.GUI
{
    partial class SocketServerForm
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
            this.btnSocketStatus = new System.Windows.Forms.Button();
            this.gbStatus = new System.Windows.Forms.GroupBox();
            this.numPort = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.lblIP = new System.Windows.Forms.Label();
            this.txtIP = new System.Windows.Forms.TextBox();
            this.lblStatus = new System.Windows.Forms.Label();
            this.tbThreadInfo = new System.Windows.Forms.TextBox();
            this.lbEvents = new System.Windows.Forms.ListBox();
            this.gbStatus.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numPort)).BeginInit();
            this.SuspendLayout();
            // 
            // btnSocketStatus
            // 
            this.btnSocketStatus.Location = new System.Drawing.Point(94, 71);
            this.btnSocketStatus.Name = "btnSocketStatus";
            this.btnSocketStatus.Size = new System.Drawing.Size(75, 23);
            this.btnSocketStatus.TabIndex = 2;
            this.btnSocketStatus.Text = "Start";
            this.btnSocketStatus.UseVisualStyleBackColor = true;
            this.btnSocketStatus.Click += new System.EventHandler(this.btnSocketStatus_Click);
            // 
            // gbStatus
            // 
            this.gbStatus.Controls.Add(this.numPort);
            this.gbStatus.Controls.Add(this.label1);
            this.gbStatus.Controls.Add(this.lblIP);
            this.gbStatus.Controls.Add(this.txtIP);
            this.gbStatus.Controls.Add(this.lblStatus);
            this.gbStatus.Controls.Add(this.btnSocketStatus);
            this.gbStatus.Location = new System.Drawing.Point(15, 12);
            this.gbStatus.Name = "gbStatus";
            this.gbStatus.Size = new System.Drawing.Size(178, 99);
            this.gbStatus.TabIndex = 3;
            this.gbStatus.TabStop = false;
            this.gbStatus.Text = "Status";
            // 
            // numPort
            // 
            this.numPort.Location = new System.Drawing.Point(94, 46);
            this.numPort.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
            this.numPort.Name = "numPort";
            this.numPort.Size = new System.Drawing.Size(75, 20);
            this.numPort.TabIndex = 9;
            this.numPort.Value = new decimal(new int[] {
            80,
            0,
            0,
            0});
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(11, 48);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(29, 13);
            this.label1.TabIndex = 8;
            this.label1.Text = "Port:";
            // 
            // lblIP
            // 
            this.lblIP.AutoSize = true;
            this.lblIP.Location = new System.Drawing.Point(11, 22);
            this.lblIP.Name = "lblIP";
            this.lblIP.Size = new System.Drawing.Size(20, 13);
            this.lblIP.TabIndex = 6;
            this.lblIP.Text = "IP:";
            // 
            // txtIP
            // 
            this.txtIP.Location = new System.Drawing.Point(69, 19);
            this.txtIP.Name = "txtIP";
            this.txtIP.Size = new System.Drawing.Size(100, 20);
            this.txtIP.TabIndex = 5;
            this.txtIP.Text = "127.0.0.1";
            // 
            // lblStatus
            // 
            this.lblStatus.AutoSize = true;
            this.lblStatus.Location = new System.Drawing.Point(11, 76);
            this.lblStatus.Name = "lblStatus";
            this.lblStatus.Size = new System.Drawing.Size(77, 13);
            this.lblStatus.TabIndex = 4;
            this.lblStatus.Text = "Socket Status:";
            // 
            // tbThreadInfo
            // 
            this.tbThreadInfo.Location = new System.Drawing.Point(15, 117);
            this.tbThreadInfo.Multiline = true;
            this.tbThreadInfo.Name = "tbThreadInfo";
            this.tbThreadInfo.Size = new System.Drawing.Size(178, 448);
            this.tbThreadInfo.TabIndex = 0;
            // 
            // lbEvents
            // 
            this.lbEvents.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lbEvents.FormattingEnabled = true;
            this.lbEvents.Location = new System.Drawing.Point(199, 15);
            this.lbEvents.Name = "lbEvents";
            this.lbEvents.Size = new System.Drawing.Size(593, 550);
            this.lbEvents.TabIndex = 0;
            // 
            // SocketServerForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(804, 577);
            this.Controls.Add(this.lbEvents);
            this.Controls.Add(this.tbThreadInfo);
            this.Controls.Add(this.gbStatus);
            this.Name = "SocketServerForm";
            this.Text = "Server";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.SocketServerForm_FormClosed);
            this.gbStatus.ResumeLayout(false);
            this.gbStatus.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numPort)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Button btnSocketStatus;
        private System.Windows.Forms.GroupBox gbStatus;
        private System.Windows.Forms.Label lblStatus;
        private System.Windows.Forms.TextBox tbThreadInfo;
        private System.Windows.Forms.TextBox txtIP;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label lblIP;
        private System.Windows.Forms.ListBox lbEvents;
        private System.Windows.Forms.NumericUpDown numPort;
    }
}

