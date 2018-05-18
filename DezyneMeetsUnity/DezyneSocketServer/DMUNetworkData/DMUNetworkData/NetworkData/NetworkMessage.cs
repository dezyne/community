using System;
using System.Collections.Generic;
using System.Runtime.Serialization;
using Newtonsoft.Json;

namespace DMUNetworkData
{
    /// <summary>
    /// Json.NET is used for the (de)serialization. See: https://www.newtonsoft.com/json
    /// See DMUNetworkData.md for information regarding this file.
    /// </summary>
    [JsonObject]
    public class NetworkMessage
    {
        /// <summary>
        /// Type of the serialized object. Allows polymorphism through reflection.
        /// See: <seealso cref="Type.GetType(string)"/> and <seealso cref="object.GetType"/>.
        /// </summary>
        [JsonProperty]
        public virtual string ObjectType { get; private set; }

        /// <summary>
        /// Network message type. Represents if a NetworkMessage is sent or received.
        /// </summary>
        public enum NetworkMessageType
        {
            Sent,
            Received
        }
        
        /// <summary>
        /// Gets the message type. This indicates whether the message was sent or received.
        /// Deserialized messages are marked as <see cref="NetworkMessageType.Received"/>, and constructed messages are marked as <see cref="NetworkMessageType.Sent"/>.
        /// </summary>
        [JsonIgnore]
        public NetworkMessageType MessageType { get; private set; }
        
        /// <summary>
        /// The time at which this object was initialized (usually directly after receiving it).
        /// </summary>
        [JsonIgnore]
        public DateTime Timestamp { get; private set; }

        /// <summary>
        /// The ID of the message, if defined by the sending side.
        /// If not set, will default to -1.
        /// </summary>
        [JsonProperty]
        public int MessageId { get; set; }

        /// <summary>
        /// Data message. Can be used to transmit or receive a single message, or add a description to data.
        /// </summary>
        [JsonProperty(NullValueHandling=NullValueHandling.Ignore)]
        public string Message { get; set; }

        /// <summary>
        ///  Gets the sender of this message. May be null.
        ///  If set, this should be the Dezyne path to the component.
        /// </summary>
        [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
        public string Sender { get; set; }

        /// <summary>
        /// Gets the supposed receiver of this message. May be null.
        /// If set, this should be the Dezyne path to the component.
        /// </summary>
        [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
        public string Receiver { get; set; }

        /// <summary>
        /// Data storage. Can be used to serialize custom data.
        /// It is recommended to make derived classes, rather than using this property.
        /// </summary>
        [JsonProperty(NullValueHandling = NullValueHandling.Ignore)]
        public Dictionary<string, object> Data { get; set; }

        /// <summary>
        /// Initializes a new empty network message with a <see cref="MessageId"/>.
        /// The message will be marked as <see cref="NetworkMessageType.Sent"/>.
        /// </summary>
        /// <param name="messageId">Id of the message.</param>
        public NetworkMessage(int messageId)
        {
            MessageType = NetworkMessageType.Sent;
            MessageId = messageId;

            Data = new Dictionary<string, object>();
        }

        /// <summary>
        /// Initializes a new empty network message without a <see cref="MessageId"/>.
        /// </summary>
        public NetworkMessage() : this(-1) { }

        /// <summary>
        /// Adds the assembly qualified object type name, so that the data can be serialized to derived classes as well.
        /// </summary>
        [OnSerializing]
        internal void OnSerializing(StreamingContext context)
        {
            ObjectType = GetType().Name;
        }

        /// <summary>
        /// Sets the time this message was deserialized.
        /// This represents the time the message was received and deserialized, not the time the message was sent.
        /// </summary>
        [OnDeserializing]
        internal void OnDeserializing(StreamingContext context)
        {
            Timestamp = DateTime.Now;
        }

        /// <summary>
        /// Sets the message type to received. The assumption is made that all deserialized messages are received, because serializing and deserializing data you own doesn't make a lot of sense.
        /// </summary>
        [OnDeserialized]
        internal void OnDeserialized(StreamingContext context)
        {
            MessageType = NetworkMessageType.Received;
        }
    }
}
