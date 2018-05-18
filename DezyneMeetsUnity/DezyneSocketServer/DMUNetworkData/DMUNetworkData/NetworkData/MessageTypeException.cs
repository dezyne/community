using System;

namespace DMUNetworkData
{
    /// <summary>
    /// Exception thrown when a NetworkMessage can not be deserialized to it's expected type.
    /// </summary>
    public class MessageTypeException : Exception
    {
        /// <summary>
        /// Instantiates a new message type exception with the given message.
        /// </summary>
        /// <param name="message">Exception message</param>
        public MessageTypeException(string message) : base(message) { }

        /// <summary>
        /// Instantiates a new message type exception.
        /// </summary>
        public MessageTypeException() : base() { }
    }
}
