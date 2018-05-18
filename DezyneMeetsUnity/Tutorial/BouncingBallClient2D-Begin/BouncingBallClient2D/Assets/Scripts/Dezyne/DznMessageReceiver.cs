using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Events;
using DMUNetworkData;

public class DznMessageReceiver : MonoBehaviour
{
    [System.Serializable]
    public struct DznMessageHandler
    {
        [Tooltip("The message to listen for. The event is only invoked if the received message matches this value. This value must be set.")]
        public string Message;
        [Tooltip("The expected sender in the message body. If set, the event is only invoked if the sender matches this value.")]
        public string Sender;
        [Tooltip("The expected receiver in the message body. If set, the event is only invoked if the receiver matches this value.")]
        public string Receiver;
        [Tooltip("The event to fire if the defined parameters match.")]
        public NetworkMessageEvent Event;
    }

    public DznMessageHandler[] MessageHandlers;

    // Use this for initialization
    void Start()
    {
        if (MessageHandlers != null && MessageHandlers.Length > 0)
        {
            DezyneConnector connector = FindObjectOfType(typeof(DezyneConnector)) as DezyneConnector;

            if (connector != null)
            {
                connector.Subscribe(MessageHandlers);
            }
        }
    }
}
