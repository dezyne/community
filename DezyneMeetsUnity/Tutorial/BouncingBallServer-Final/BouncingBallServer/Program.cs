using DezyneSocketThread.Socket;
using DezyneSocketThread.Threading;
using dzn;
using System.Net.Sockets;
using System.Threading;
using DMUNetworkData;
using System;

namespace BouncingBallServer
{
    class Program
    {
        static Locator locator;
        static Runtime runtime;
        static Controller controller;
        static DezyneThread dezyneThread;
        static Thread thread;

        static void Main(string[] args)
        {
            locator = new Locator();
            runtime = new Runtime();
            locator.set(runtime);

            controller = new Controller(locator, "Controller");
            controller.iu.inport.reverse = ReverseBall;
            controller.iu.inport.start = StartBall;
            controller.check_bindings();

            TcpClient client = SocketListener.AwaitConnection("127.0.0.1", 80); // Blocks until Unity connects.
            dezyneThread = new DezyneThread(client);
            dezyneThread.Subscribe("WallHit", WallHit);
            thread = new Thread(dezyneThread.StartThread);
            thread.Start();

            controller.ia.inport.start();
        }
        
        private static void WallHit(NetworkMessage message)
        {
            controller.iu.outport.wallHit();
        }

        static void StartBall()
        {
            dezyneThread.AddSendMessage(new NetworkMessage()
            {
                Message = "StartBall" // Event Unity has to handle.
            });
        }

        static void ReverseBall()
        {
            dezyneThread.AddSendMessage(new NetworkMessage()
            {
                Message = "ReverseBall" // Event Unity has to handle.
            });
        }
    }
}
