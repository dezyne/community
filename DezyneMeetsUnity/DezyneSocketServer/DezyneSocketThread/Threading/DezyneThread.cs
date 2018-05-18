using DMUNetworkData;

using Newtonsoft.Json;

using System;
using System.Text;
using System.Threading;
using System.Net.Sockets;
using System.Collections.Generic;
using System.Diagnostics;

namespace DezyneSocketThread.Threading
{
    /// <summary>
    /// Class used to communicate with a Unity application for a Dezyne model.
    /// Instances of this class can manage sending and receiving messages through queues, and will delegate events to listeners.
    /// See: <seealso cref="Subscribe(string, Action{NetworkMessage})"/>, <seealso cref="OnMessageReceived"/>, <seealso cref="OnMessageSend"/>.
    /// </summary>
    public class DezyneThread
    {
        // Connection handler
        private TcpClient client;
        // Data stream (bi-directional)
        private NetworkStream clientStream;
        // Queued messages to send.
        private List<NetworkMessage> sendMessageList = new List<NetworkMessage>();
        // Message-specific listeners. See Subscribe(message, action).
        public Dictionary<string, List<Action<NetworkMessage>>> eventList = new Dictionary<string, List<Action<NetworkMessage>>>();
        
        private Thread thread;
        public Thread Thread { get { return thread; } }

        public DezyneThread(TcpClient client)
        {
            if (client == null)
                throw new ArgumentNullException("The supplied TcpClient object was null");

            if (!client.Connected)
                throw new InvalidOperationException("The TcpClient was not connected");

            this.client = client;
            this.clientStream = this.client.GetStream();
        }

        /// <summary>
        /// Starts the thread. See <see cref="HandleConnection"/>.
        /// </summary>
        public void StartThread()
        {
            thread = Thread.CurrentThread;

            HandleConnection();
        }

        /// <summary>
        /// Terminates the connection thread.
        /// </summary>
        public void StopThread()
        {
            thread.Abort();
        }

        /// <summary>
        /// Handles the connection. Receives messages and delegates them to listeners, and sends pending messages.
        /// </summary>
        private void HandleConnection()
        {
            try
            {
                while (clientStream != null)
                {
                    // Abort
                    if(!client.Connected)
                    {
                        client.Close();
                        break;
                    }

                    // If messages came in, read them.
                    if(clientStream.DataAvailable)
                    {
                        NetworkMessage message = StreamToMessage();

                        if (message == null) continue;
                        if (message.Message == "CloseDezyneSocket")
                        {
                            Debug.WriteLine("INFO: We have received a message to close the Dezyne socket (Message: 'CloseDezyneSocket')");
                            break;
                        }
                    }

                    // If messages are pending, send them.
                    lock (sendMessageList)
                    {
                        foreach (NetworkMessage message in sendMessageList)
                        {
                            SendMessage(message);
                        }
                        sendMessageList.Clear();
                    }
                }
            }
            finally
            {
                client.Close();
                client = null;
            }
        }

        /// <summary>
        /// Receive a message from the connected client, and delegates it to listeners.
        /// </summary>
        /// <returns>Received NetworkMessage.</returns>
        private NetworkMessage StreamToMessage()
        {
            // Read message length
            byte[] lBytes = new byte[4];
            clientStream.Read(lBytes, 0, 4);
            int l = BytesToInt(lBytes);

            // Read message
            byte[] bytes = new byte[l];
            clientStream.Read(bytes, 0, bytes.Length);
            
            // Deserialize message
            string data = Encoding.UTF8.GetString(bytes);
            NetworkMessage message = null;
            try
            {
                message = NetworkMessageReader.Deserialize(data);
            }
            catch (Exception exc)
            {
                Console.WriteLine("Skipped null message");
                return null;
            }

            // Delegate event.
            if (OnMessageReceived != null)
                OnMessageReceived.Invoke(this, message);

            List<Action<NetworkMessage>> actionList;
            if (eventList.TryGetValue(message.Message, out actionList))
            {
                foreach (Action<NetworkMessage> action in actionList)
                {
                    action.Invoke(message);
                }
            }

            return message;
        }

        /// <summary>
        /// Queues a message. It will be sent to the client, assuming a connection is made.
        /// </summary>
        /// <param name="message">Message to send.</param>
        public void AddSendMessage(NetworkMessage message)
        {
            if (message != null)
            {
                lock (sendMessageList)
                {
                    sendMessageList.Add(message);
                }
            }
        }

        /// <summary>
        /// Sends a message to the client.
        /// </summary>
        /// <param name="message">Message to send.</param>
        private void SendMessage(NetworkMessage message)
        {
            if (client == null)
            {
                throw new Exception("Client is null, no messages can be send anymore");
            }

            try
            {
                if (!client.Connected) return;

                // Serialize message
                string serializedMessage = JsonConvert.SerializeObject(message);

                // Get message length
                byte[] data = Encoding.UTF8.GetBytes(serializedMessage);
                byte[] length = IntToBytes(data.Length);

                // Send length followed by message
                clientStream.Write(length, 0, length.Length);
                clientStream.Write(data, 0, data.Length);

                // Notify listeners.
                if (OnMessageSend != null)
                {
                    OnMessageSend.Invoke(this, message);
                }
            }
            catch (Exception e)
            {
                throw e;
            }
        }

        /// <summary>
        /// Message for logging. "ThreadID: id"
        /// </summary>
        /// <returns>Logging message.</returns>
        public override string ToString()
        {
            return "ThreadID: " + Thread.CurrentThread.ManagedThreadId;
        }

        //Event Handlers
        public event OnMessageReceivedEventHandler OnMessageReceived;
        public event OnMessageSendEventHandler OnMessageSend;

        /// <summary>
        /// Delegate for handling received messages.
        /// </summary>
        /// <param name="sender">The DezyneThread object.</param>
        /// <param name="message">Received message.</param>
        public delegate void OnMessageReceivedEventHandler(object sender, NetworkMessage message);

        /// <summary>
        /// Delegate for handling messages after they are sent (but may not yet be received!).
        /// </summary>
        /// <param name="sender">The DezyneThread object.</param>
        /// <param name="message">Sent message.</param>
        public delegate void OnMessageSendEventHandler(object sender, NetworkMessage message);

        /// <summary>
        /// Adds a message handler. <see cref="DznMessageReceiver.DznMessageHandler.Event"/> is invoked when this connector receives the message <see cref="DznMessageReceiver.DznMessageHandler.Message"/>.
        /// The message is case sensitive if <see cref="CaseSensitive"/> is set.
        /// </summary>
        /// <param name="messageHandler">Message handler to subscribe.</param>
        public void Subscribe(string message, Action<NetworkMessage> action)
        {
            if (!eventList.ContainsKey(message))
            {
                eventList.Add(message, new List<Action<NetworkMessage>>());
            }

            eventList[message].Add(action);
        }

        /// <summary>
        /// Removes a message handler. The event will no longer be invoked when this connector receives the message <see cref="DznMessageReceiver.DznMessageHandler.Message"/>.
        /// </summary>
        /// <param name="messageHandler">Message handler to unsubscribe.</param>
        public void Unsubscribe(string message, Action<NetworkMessage> action)
        {
            List<Action<NetworkMessage>> actions;

            if (eventList.TryGetValue(message, out actions))
            {
                actions.Remove(action);
            }
        }
        
        /// <summary>
        /// Converts an int32 stored in 4 bytes (big-endian order) to an int.
        /// </summary>
        /// <param name="bytes">4 bytes.</param>
        /// <returns>Int.</returns>
        private int BytesToInt(byte[] bytes)
        {
            int v = (bytes[0] << 24) |
                    (bytes[1] << 16) |
                    (bytes[2] << 8) |
                    bytes[3];
            return v;
        }

        /// <summary>
        /// Converts an int32 to 4 bytes in big-endian order.
        /// </summary>
        /// <param name="i"></param>
        /// <returns>Bytes in big-endian order.</returns>
        private byte[] IntToBytes(int i)
        {
            byte[] b = BitConverter.GetBytes(i);
            // GetBytes is in Little Endian order if this flag is true, so reverse it.
            if (BitConverter.IsLittleEndian)
                Array.Reverse(b);
            return b;
        }
    }
}
