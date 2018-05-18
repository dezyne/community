using System;
using System.Reflection;
using Newtonsoft.Json.Linq;

namespace DMUNetworkData
{
    /// <summary>
    /// Class used for converting Json NetworkMessage data to actual NetworkMessage objects.
    /// </summary>
    public static class NetworkMessageReader
    {
        /// <summary>
        /// Deserializes the network message to a (sub-type of) <see cref="NetworkMessage"/>.
        /// </summary>
        /// <param name="json">Network message.</param>
        /// <returns>Deserialized network message.</returns>
        public static NetworkMessage Deserialize(string json)
        {
            // Deserialize to generic
            JObject obj = JObject.Parse(json);

            // Read type
            Type objType = GetMessageType(obj);

            // Cast
            return obj.ToObject(objType) as NetworkMessage;
        }

        /// <summary>
        /// Deserializes the network message to the given type.
        /// Type must inherit <see cref="NetworkMessage"/>.
        /// </summary>
        /// <typeparam name="T">NetworkMessage Type</typeparam>
        /// <param name="json">NNetwork message.</param>
        /// <returns>Deserialized network message to <paramref name="T"/>.</returns>
        public static T Deserialize<T>(string json) where T : NetworkMessage
        {
            return Deserialize(json) as T;
        }

        /// <summary>
        /// Gets the message type of the network message object. All loaded assemblies are checked.
        /// </summary>
        /// <param name="messageObject">Network Message object.</param>
        /// <returns>Type of the network message.</returns>
        /// <exception cref="MessageTypeException">Thrown if the object type could not be found, or is not defined.</exception>
        public static Type GetMessageType(JObject messageObject)
        {
            JToken tType = messageObject["ObjectType"];
            Type networkMessageType = typeof(NetworkMessage);

            if (tType != null && tType.Type == JTokenType.String)
            {
                string sType = tType.Value<string>();
                if (sType == "NetworkMessage") return networkMessageType;

                // Check all loaded types
                foreach (Assembly asm in AppDomain.CurrentDomain.GetAssemblies())
                {
                    foreach (Type type in asm.GetTypes())
                    {
                        // Match class name and inheritance.
                        if (type.Name == sType && type.IsSubclassOf(networkMessageType))
                        {
                            return type;
                        }
                    }
                }

                // ObjectType has value, but no valid type found.
                throw new MessageTypeException("The type of the NetworkMessage object could not be determined. Is it defined and is the class name present in the loaded assemblies?");
            }

            throw new MessageTypeException("The network message object does not define an ObjectType.");
        }

        /// <summary>
        /// Gets the message type of the network message. All loaded assemblies are checked.
        /// </summary>
        /// <param name="json">Serialized network message in Json.</param>
        /// <returns>Type of the network message.</returns>
        /// <exception cref="MessageTypeException">Thrown if the object type could not be found, or is not defined.</exception>
        public static Type GetMessageType(string json)
        {
            JObject messageObject = JObject.Parse(json);
            return GetMessageType(messageObject);
        }

        /// <summary>
        /// Gets the message type of the network message object. All loaded assemblies are checked.
        /// If the type could not be found, <see cref="NetworkMessage"/> is returned instead.
        /// </summary>
        /// <param name="messageObject">Network Message object.</param>
        /// <returns>Type of the network message.</returns>
        public static Type GetSafeMessageType(JObject messageObject)
        {
            try
            {
                return GetMessageType(messageObject);
            }
            catch
            {
                return typeof(NetworkMessage);
            }
        }

        /// <summary>
        /// Gets the message type of the network message. All loaded assemblies are checked.
        /// If the type could not be found, <see cref="NetworkMessage"/> is returned instead.
        /// </summary>
        /// <param name="json">Serialized network message in Json.</param>
        /// <returns>Type of the network message.</returns>
        public static Type GetSafeMessageType(string json)
        {
            try
            {
                return GetMessageType(json);
            }
            catch
            {
                return typeof(NetworkMessage);
            }
        }
    }
}
