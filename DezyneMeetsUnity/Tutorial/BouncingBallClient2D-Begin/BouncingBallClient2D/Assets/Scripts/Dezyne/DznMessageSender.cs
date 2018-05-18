using DMUNetworkData;
using System.Linq;
using System.Collections.Generic;
using UnityEngine;

public class DznMessageSender : MonoBehaviour
{
    [System.Serializable]
    public struct DznMessage
    {
        [Tooltip("The message to sent. This value must be set")]
        public string Message;
        [Tooltip("The sender in the message body. If set, the event is only invoked if the sender matches this value.")]
        public string Sender;
        [Tooltip("The expected receiver in the message body. If set, only the object that matches this name will fire off its event.")]
        public string Receiver;
    }

    private DezyneConnector connector;

    public List<DznMessage> Messages;

    void Start()
    {
        connector = FindObjectOfType<DezyneConnector>();

        if(connector == null)
        {
            Debug.LogFormat("{0} could not find the DezyneConnector, make sure there is a DezyneConnector component present in the scene!", gameObject.name);
        }

    }

    /// <summary>
    /// Converts a DznMessage struct to a NetworkMessage class
    /// </summary>
    /// <param name="message">The predefined message</param>
    /// <returns></returns>
    private NetworkMessage PredefinedToMessage(DznMessage message)
    {
        return new NetworkMessage()
        {
            Message = message.Message,
            Sender = !string.IsNullOrEmpty(message.Sender) ? message.Sender : null,
            Receiver = !string.IsNullOrEmpty(message.Receiver) ? message.Receiver : null
        };
    }

    /// <summary>
    /// Gets the predefined message with the same messageName and sends it as a network message to the server
    /// </summary>
    /// <param name="messageName">Name of the predefined message</param>
    public void SendPredefinedNetworkMessage(string messageName)
    {
        DznMessage message = Messages.Where(m => m.Message == messageName).FirstOrDefault();
        if (!string.IsNullOrEmpty(message.Message))
        {
            SendNetworkMessage(PredefinedToMessage(message));
        }
        else
        {
            Debug.LogFormat("{0}, tried to send a predefined message with name {1}. No message found.", gameObject.name, messageName);
        }
    }

    /// <summary>
    /// Gets the predifend message at the index in the Messages list and sends it as a network message to the server
    /// </summary>
    /// <param name="index">Index of the predefined message</param>
    public void SendPredefinedNetworkMessage(int index)
    {
        if(index < 0 || index > Messages.Count - 1)
        {
            Debug.LogFormat("{0}, tried to send a predefined message with index {1}. No message found.", gameObject.name, index);
            return;
        }

        SendNetworkMessage(PredefinedToMessage(Messages[index]));
    }

    /// <summary>
    /// Sends the provided NetworkMessage object to the server
    /// </summary>
    /// <param name="message"></param>
    public void SendNetworkMessage(NetworkMessage message)
    {
        if(connector != null)
        {
            connector.client.AddSendMessages(message);
        }
        else
        {
            Debug.LogFormat("{0}, connector is null", gameObject.name);
        }
    }

    /// <summary>
    /// Takes the parameters and makes a NetworkMessage that is send to the server
    /// </summary>
    /// <param name="message">Message name</param>
    /// <param name="sender">Sender name, OPTIONAL</param>
    /// <param name="receiver">Receiver name, OPTIONAL</param>
    /// <param name="data">Data object, OPTIONAL</param>
    public void SendNetworkMessage(string message, string sender = null, string receiver = null, Dictionary<string, object> data = null)
    {
        SendNetworkMessage(new NetworkMessage()
        {
            Message = message,
            Sender = sender,
            Receiver = receiver,
            Data = data
        });
    }
}
