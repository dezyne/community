using System.Collections.Generic;
using DMUNetworkData;
using UnityEngine;

/// <summary>
/// Component 
/// </summary>
public class DezyneConnector : MonoBehaviour
{
    [Tooltip("The IP of the 'server'. Generally '127.0.0.1'.")]
    public string Host = "127.0.0.1";
    [Tooltip("The port of the 'server'. Generally 80.")]
    public int Port = 80;

    [Tooltip("Indicates if messages are case sensitive. This only affects subscribed DznMessageHandlers, and does not affect OnReceiveMessage. This value should not change runtime, as already subscribed events won't be affected by the change.")]
    public bool CaseSensitive = true;

    /// <summary>
    /// Object that connects to the server and handles sending and receiving messages.
    /// </summary>
    public ClientSocket client;

    /// <summary>
    /// Event handler delegate for network messages.
    /// </summary>
    /// <param name="message">Received message.</param>
    public delegate void NetworkMessageHandler(NetworkMessage message);

    /// <summary>
    /// Event invoked when the client receives any messages from the server.
    /// </summary>
    public event NetworkMessageHandler OnReceiveMessage;

    /// <summary>
    /// Events that are only invoked when the received <see cref="NetworkMessage.Message"/> matches <see cref="DznMessageReceiver.DznMessageHandler.Message"/>.
    /// SeeL <see cref="Subscribe(DznMessageReceiver.DznMessageHandler)"/>.
    /// </summary>
    private Dictionary<string, List<DznMessageReceiver.DznMessageHandler>> events;

    /// <summary>
    /// Initializes the connector.
    /// </summary>
    private void Awake()
    {
        if (FindObjectsOfType<DezyneConnector>().Length > 1)
        {
            Debug.LogErrorFormat("(GameObject {0}) Multiple DezyneConnector components are present in the scene. DznMessageReceiver components will only subscribe their message handlers on one DezyneConnector.", gameObject.name);
        }

        events = new Dictionary<string, List<DznMessageReceiver.DznMessageHandler>>();
    }

    /// <summary>
    /// Reads all received messages that haven't been handled yet.
    /// </summary>
    private void Update()
    {
        if (client != null)
        {
            while (client.ReceivedMessages.Count > 0)
            {
                ReadMessage();
            }
        }
    }

    /// <summary>
    /// Disposes the connection, if any.
    /// </summary>
    private void OnDestroy()
    {
        if (client != null)
        {
            client.Dispose();
            client = null;
        }
    }

    /// <summary>
    /// Connects to the server, if not already connected.
    /// </summary>
    public void Connect()
    {
        if (client == null)
        {
            client = new ClientSocket();
        }
        else if (client.runningThread)
        {
            return;
        }

        client.StartSocket(Host, Port);
    }

    /// <summary>
    /// Adds a message handler. <see cref="DznMessageReceiver.DznMessageHandler.Event"/> is invoked when this connector receives the message <see cref="DznMessageReceiver.DznMessageHandler.Message"/>.
    /// The message is case sensitive if <see cref="CaseSensitive"/> is set.
    /// </summary>
    /// <param name="messageHandler">Message handler to subscribe.</param>
    public void Subscribe(DznMessageReceiver.DznMessageHandler messageHandler)
    {
        string message = CaseSensitive ? messageHandler.Message : messageHandler.Message.ToLowerInvariant();

        if (!events.ContainsKey(message))
        {
            events.Add(message, new List<DznMessageReceiver.DznMessageHandler>());
        }

        events[message].Add(messageHandler);
    }

    /// <summary>
    /// Adds multiple message handlers. See <see cref="Subscribe(DznMessageReceiver.DznMessageHandler)"/>.
    /// </summary>
    /// <param name="messageHandlers">Message handlers to subscribe</param>
    public void Subscribe(ICollection<DznMessageReceiver.DznMessageHandler> messageHandlers)
    {
        foreach (DznMessageReceiver.DznMessageHandler handler in messageHandlers)
        {
            Subscribe(handler);
        }
    }

    /// <summary>
    /// Removes a message handler. The event will no longer be invoked when this connector receives the message <see cref="DznMessageReceiver.DznMessageHandler.Message"/>.
    /// </summary>
    /// <param name="messageHandler">Message handler to unsubscribe.</param>
    public void Unsubscribe(DznMessageReceiver.DznMessageHandler messageHandler)
    {
        List<DznMessageReceiver.DznMessageHandler> evt;

        string message = CaseSensitive ? messageHandler.Message : messageHandler.Message.ToLowerInvariant();

        if (events.TryGetValue(message, out evt))
        {
            evt.Remove(messageHandler);
        }
    }

    /// <summary>
    /// Removes multiple message handlers. See <see cref="Unsubscribe(DznMessageReceiver.DznMessageHandler)"/>.
    /// </summary>
    /// <param name="messageHandlers">Message handlers to unsubscibe.</param>
    public void Unsubscribe(ICollection<DznMessageReceiver.DznMessageHandler> messageHandlers)
    {
        foreach (DznMessageReceiver.DznMessageHandler handler in messageHandlers)
        {
            Unsubscribe(handler);
        }
    }

    /// <summary>
    /// Reads the first received message that hasn't been handled yet.
    /// Invokes <see cref="OnReceiveMessage"/> and any subscribed events for the specific <see cref="NetworkMessage.Message"/>.
    /// </summary>
    private void ReadMessage()
    {
        NetworkMessage networkMessage = client.ReceivedMessages[0];
        client.RemoveReceivedMessages(0);

        if (networkMessage == null || string.IsNullOrEmpty(networkMessage.Message))
        {
            return;
        }

        string message = CaseSensitive ? networkMessage.Message : networkMessage.Message.ToLowerInvariant();

        // Always invoke OnReceiveMessage
        if (OnReceiveMessage != null)
        {
            OnReceiveMessage.Invoke(networkMessage);
        }

        // Invoke message handlers for this specific message.
        List<DznMessageReceiver.DznMessageHandler> evts;
        if (events.TryGetValue(message, out evts))
        {
            foreach (DznMessageReceiver.DznMessageHandler evtHandler in evts)
            {
                if (!string.IsNullOrEmpty(networkMessage.Receiver) && evtHandler.Receiver != networkMessage.Receiver)
                {
                    continue;
                }
                if (!string.IsNullOrEmpty(networkMessage.Sender) && evtHandler.Sender != networkMessage.Sender)
                {
                    continue;
                }

                evtHandler.Event.Invoke(networkMessage);
            }
        }
    }
}
