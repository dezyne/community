using System;
using System.Windows.Forms;
using DezyneSocketThread.Threading;

using DMUNetworkData;

namespace DezyneSocketServerUI.GUI
{
    /// <summary>
    /// A simple WinForms GUI that will print all received network messages.
    /// </summary>
    public partial class SocketServerForm : Form
    {
        private DezyneThread dezyneThread;
        public DezyneThread DezyneThread
        {
            get
            {
                return dezyneThread;
            }
            set
            {
                if (dezyneThread != null)
                {
                    dezyneThread.OnMessageReceived -= Thread_OnMessageReceived;
                    dezyneThread.OnMessageSend -= Thread_OnMessageSend;
                }

                dezyneThread = value;

                dezyneThread.OnMessageReceived += Thread_OnMessageReceived;
                dezyneThread.OnMessageSend += Thread_OnMessageSend;
            }
        }


        public SocketServerForm()
        {
            InitializeComponent();
        }

        public SocketServerForm(DezyneThread thread)
        {
            InitializeComponent();
            DezyneThread = thread;
        }

        private void Thread_OnMessageSend(object sender, NetworkMessage message)
        {
            // Tell GUI to add a sent message to the list.
            lbEvents.Invoke((MethodInvoker)delegate
            {
                lbEvents.Items.Add(GetMessageDetails(message));
            });
        }

        private void Thread_OnMessageReceived(object sender, NetworkMessage message)
        {
            // Tell GUI to add a received message to the list.
            lbEvents.Invoke((MethodInvoker)delegate
            {
                lbEvents.Items.Add(GetMessageDetails(message));
            });
        }

        /// <summary>
        /// Returns a string representation of the given message.
        /// </summary>
        private string GetMessageDetails(NetworkMessage message)
        {
            return message.Timestamp + " - " + message.MessageType + " - " + message.Message;
        }

        private void btnSocketStatus_Click(object sender, EventArgs e)
        {
            OnStartListenerEvent.Invoke(this, txtIP.Text, (int)numPort.Value);
        }

        public event OnCloseFormEventHandler OnCloseForm;
        public event OnStartSocketListenerEventHandler OnStartListenerEvent;

        public delegate void OnCloseFormEventHandler(object sender);
        public delegate void OnStartSocketListenerEventHandler(object sender, string ip, int port);

        private void SocketServerForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (OnCloseForm != null)
            {
                OnCloseForm.Invoke(this);
            }
        }
    }
}
