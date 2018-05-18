using System;
using System.Text;
using System.Threading;
using System.Collections.Generic;
using System.Net.Sockets;
using System.Collections.ObjectModel;
using DMUNetworkData;
using Newtonsoft.Json;
using UnityEngine;

/// <summary>
/// Communication object for communication between a Dezyne application and and this Unity application.
/// See <see cref="DezyneConnector"/> instead.
/// </summary>
public class ClientSocket : IDisposable
{
    private List<NetworkMessage> receivedMessages = new List<NetworkMessage>();
    private List<NetworkMessage> sendMessages = new List<NetworkMessage>();

    public ReadOnlyCollection<NetworkMessage> ReceivedMessages { get { return receivedMessages.AsReadOnly(); } }
    public ReadOnlyCollection<NetworkMessage> SendMessages { get { return sendMessages.AsReadOnly(); } }

    public string Host { get; set; }
    public int Port { get; set; }

    public bool runningThread = false;

    private TcpClient client;
    private NetworkStream stream;

    private Thread socketThread;

    /// <summary>
    /// Creates a new client socket.
    /// The client socket will not be active until <see cref="StartSocket"/> is called.
    /// </summary>
    public ClientSocket() { }

    /// <summary>
    /// Locks the sendMessages object and adds the provided NetworkMessage object to the list
    /// </summary>
    /// <param name="message">NetworkMessage to be added</param>
    public void AddSendMessages(NetworkMessage message)
    {
        lock (sendMessages)
        {
            sendMessages.Add(message);
        }
    }

    /// <summary>
    /// Locks the receivedMessages object and adds the provided NetworkMessage object to the list
    /// </summary>
    /// <param name="message">NetworkMessage to be added</param>
    public void AddReceivedMessages(NetworkMessage message)
    {
        lock (receivedMessages)
        {
            receivedMessages.Add(message);
        }
    }

    /// <summary>
    /// Locks the sendMessages object and takes the NetworkMessage at index[0].
    /// Removes the NetworkMessage from the list and returns it.
    /// If the list is empty, return NULL
    /// </summary>
    /// <returns>The NetworkMessage that was at index[0]</returns>
    public NetworkMessage TakeSendMessage()
    {
        NetworkMessage msg;
        lock (sendMessages)
        {
            if (sendMessages.Count == 0) return null;
            msg = sendMessages[0];
            sendMessages.RemoveAt(0);
        }
        return msg;
    }

    /// <summary>
    /// If the provided value is outside the range of the list nothing happens. 
    /// Else locks the sendMessages object and removes the NetworkMessage object on the provided value
    /// </summary>
    /// <param name="index">Index of the NetworkMessage that needs to be removed</param>
    public void RemoveSendMessages(int index)
    {
        if (index < 0 || index > sendMessages.Count - 1)
            return;

        lock (sendMessages)
        {
            sendMessages.RemoveAt(index);
        }
    }

    /// <summary>
    /// If the provided value is outside the range of the list nothing happens. 
    /// Else locks the receivedMessages object and removes the NetworkMessage object on the provided value
    /// </summary>
    /// <param name="index">Index of the NetworkMessage that needs to be removed</param>
    public void RemoveReceivedMessages(int index)
    {
        if (index < 0 || index > receivedMessages.Count - 1)
            return;

        lock (receivedMessages)
        {
            receivedMessages.RemoveAt(index);
        }
    }

    /// <summary>
    /// Starts a socket on the provided host (IP) and port
    /// </summary>
    /// <param name="host">The IP on which the socket server is running</param>
    /// <param name="port">The port on whicht the socket server is running</param>
    public void StartSocket(string host, int port)
    {
        Host = host;
        Port = port;
        StartSocket();
    }

    /// <summary>
    /// Checks if there currently is a socketThread running
    /// If not, create a new Thread and pass start the StartClient method on it
    /// </summary>
    public void StartSocket()
    {
        if (socketThread != null && socketThread.IsAlive)
        {
            socketThread.Abort();
        }

        socketThread = new Thread(() => StartClient(Host, Port));
        socketThread.Start();
    }

    /// <summary>
    /// Starts the client with the provided port and host.
    /// </summary>
    private void StartClient(string host, int port)
    {
        //Set the thread to running
        runningThread = true;

        // This will automatically connect.
        client = new TcpClient(Host, Port);

        // Get the stream
        stream = client.GetStream();

        RunningClient();
    }

    /// <summary>
    /// Runs a while loop
    /// At the start of the loop it receives all data from the socket
    /// Then it will loop untill all messages that need to be send have
    /// been send.
    /// </summary>
    private void RunningClient()
    {
        try
        {
            while (runningThread)
            {
                ReceiveMessages();

                NetworkMessage message = null;
                while (SendMessages.Count > 0)
                {
                    message = TakeSendMessage();
                    SendMessage(message);

                    if(message.Message == "CloseDezyneSocket")
                    {
                        Debug.Log("Unity has sent a close socket command, socket is being disposed (Message: 'CloseDezyneSocket')");
                        break;
                    }
                }
            }
        }
        finally
        {

        }
    }

    /// <summary>
    /// Checks if the server sent any message.
    /// </summary>
    private void ReceiveMessages()
    {
        // The client is null or not connected anymore
        // Stop running the thread
        if (client == null || !client.Connected)
        {
            runningThread = false;
            return;
        }

        while (stream.DataAvailable)
        {
            // Try to read the message.
            try
            {
                byte[] lBytes = new byte[4];
                stream.Read(lBytes, 0, 4);
                int l = BytesToInt(lBytes);
                byte[] bytes = new byte[l];

                stream.Read(bytes, 0, bytes.Length);
                // Log data.
                string data = Encoding.UTF8.GetString(bytes);
                NetworkMessage networkMessage = NetworkMessageReader.Deserialize(data);

                if (networkMessage == null)
                {
                    Debug.LogError("The received message object could not be deserialized to NetworkMessage.");
                    return;
                }

                AddReceivedMessages(networkMessage);
            }
            catch (Exception e)
            {
                throw e;
            }
        }
    }

    /// <summary>
    /// If there is a client, it will send the message to it.
    /// </summary>
    private void SendMessage(NetworkMessage message)
    {
        if (client == null)
            throw new Exception("Client is null, no messages can be sent anymore");

        try
        {
            if (!client.Connected) return;

            string serializedMessage = JsonConvert.SerializeObject(message);
            byte[] data = Encoding.UTF8.GetBytes(serializedMessage);
            byte[] length = IntToBytes(data.Length);
            stream.Write(length, 0, length.Length);
            stream.Write(data, 0, data.Length);
        }
        catch
        {
            Dispose();
        }
    }

    /// <summary>
    /// Closes the stream and client.
    /// </summary>
    public void Dispose()
    {
        if (client != null && client.Connected)
        {
            stream.Close();
            client.Close();

            runningThread = false;

            stream = null;
            client = null;
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
