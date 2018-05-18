using System;
using System.Net;
using System.Net.Sockets;

namespace DezyneSocketThread.Socket
{
    /// <summary>
    /// Helper class for starting a TCP server and accepting connections.
    /// See <see cref="StartServer(string, int)"/>.
    /// </summary>
    public class SocketListener
    {
        private static TcpListener server;

        /// <summary>
        /// Starts a <see cref="TcpListener"/> server on the given address and port.
        /// Awaits and returns the first connection.
        /// This is an alternative to calling <seealso cref="StartServer(string, int)"/> followed by <seealso cref="AwaitConnection()"/>.
        /// </summary>
        /// <param name="ip">Local IP address of server (usually 127.0.0.1).</param>
        /// <param name="port">Port number.</param>
        /// <returns>Connection handler.</returns>
        public static TcpClient AwaitConnection(string ip, int port)
        {
            StartServer(ip, port);

            return AwaitConnection();
        }

        /// <summary>
        /// Awaits a new connection, and returns the handler.
        /// </summary>
        /// <returns>Connection handler.</returns>
        public static TcpClient AwaitConnection()
        {
            TcpClient client = server.AcceptTcpClient();
            return client;
        }

        /// <summary>
        /// Starts a <see cref="TcpListener"/> server on the given address and port.
        /// The server is idle until a connection is accepted (see <seealso cref="AwaitConnection()"/>).
        /// </summary>
        /// <param name="ip">Local IP address of server (usually 127.0.0.1).</param>
        /// <param name="port">Port number.</param>
        public static void StartServer(string ip, int port)
        {
            if (server == null)
            {
                IPAddress ipAddress;
                if (!IPAddress.TryParse(ip, out ipAddress))
                {
                    throw new ArgumentException("Invalid IP");
                }
                if (port < 1)
                {
                    throw new ArgumentException("Invalid port");
                }

                server = new TcpListener(ipAddress, port);
                server.Start();
            }
        }
    }
}
