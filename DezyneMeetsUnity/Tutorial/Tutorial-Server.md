# DezyneMeetsUnity - Server

##### [- Return to Begin](Tutorial-Begin.md)

The server application houses the Dezyne model, and will communicate with Unity (the client).  
For this tutorial, we recommend using the Server Startup project. You can also create a C# Console Application (.NET Framework).

* If you're using the Server Startup project, make sure to unpack it.
* Open the `BouncingBallServer.sln` file.
* Go to `Tools > NuGet Package Manager > Manage NuGet Packages for Solution...` to restore Newtonsoft.Json. It should automatically show a warning at the top of the window:

![](../Images/Tutorial/NuGetRestore.png)

If, for some reason, the compiling still lists errors related to the Json.NET library, consider reinstalling the package entirely. Make sure to install version `9.0.1`, for best compatibility with Unity.

## Table of Contents

* [Installing Libraries](#installing-libraries)
* [Adding Generated Code](#adding-generated-code)
* [Implementing the Model](#implementing-the-model)
* [Connecting to Unity](#connecting-to-unity)
* [Outgoing Messages](#outgoing-messages)
* [Incoming Messages](#incoming-messages)
  * [Global Message Handler](#global-message-handler)
  * [Key-based Message Handler](#key-based-message-handler)
* [Server Implementation Done](#server-implementation-done)
  * [Continue](#continue)

## Installing Libraries

> ##### *<span style="color:red">If you downloaded and opened the Server Startup project, you may skip this step.</span>*

</span>

Multiple libraries are necessary to set up the server. One is used for the actual server communication, and the other is a data library that defines the network message structure. We'll also need [Json.NET](https://www.newtonsoft.com/json), as it is used to (de)serialize network messages.

* Install `Json.NET` using the NuGet package manager. The recommended version is `9.0.1`.
  * For information on how to do this, refer to the [official documentation on Microsoft Docs](https://docs.microsoft.com/en-us/nuget/tools/package-manager-ui).
  * In the package manager, `Json.NET` may show up as `Newtonsoft.Json`.
* Add the following files to the references of your project:
  * `DMUNetworkData.dll`
  * `DezyneSocketThread.dll`

![](../Images/Tutorial/ServerAddReference.png)

You'll also need the Dezyne runtime library for C#. You can download the files from the Dezyne tool.

![](../Images/Tutorial/CsRuntime.png)

* Add the C# Dezyne Runtime files to the project by dragging the `dzn` folder onto your project in the Solution Explorer.
   * There may be some odd symbols present in the `pump.cs` file (checked 24-11-2017).  
   On line 134 and 144, replace `Deadline,â€‚Action` with `Deadline,Action`.

## Adding Generated Code

> ##### *<span style="color:red">If you downloaded and opened the Server Startup project, you may skip this step.</span>*

* Generate C# code for the Bouncing Ball Dezyne Model.
  You should end up with the following files:
  * `Controller.cs`
  * `iCommands.cs`
  * `iUnity.cs`
* Add these files to your project by dragging them onto your project in the Solution Explorer.
  * You can group them in a folder (I.e. `Model`), or just put the files in the root of your project.

With the libraries and generated code in your project, your project structure should contain the following (and the standard references such as `System`):

![](../Images/Tutorial/ProjectStructure.png)

---

## Implementing the Model

* Open the `Program.cs` file.
  * This is where the application starts, and where you'll have to implement your code.
* Add `using dzn;` at the top of the code.
* Create a `runtime` and `locator` object (minimum required Dezyne objects).
* Link the runtime to the locator.
* Create a new instance of the base component of the model (`Controller`).

```cs
class Program
{
  static Locator locator;
  static Runtime runtime;
  static Controller controller;

  static void Main(string[] args)
  {
    locator = new Locator();
    runtime = new Runtime();
    locator.set(runtime);

    controller = new Controller(locator, "Controller");
  }
}
```

* Set up the bindings of the model.
  * The provided `iCommand` interface has no out-events, so there's no need to bind anything.
  * The required `iUnity` interface has the in-events `start` and `reverse`, which we'll need to bind in our code.

```cs
static void Main(string[] args)
{
  locator = new Locator();
  runtime = new Runtime();
  locator.set(runtime);

  controller = new Controller(locator, "Controller");
  controller.iu.inport.reverse = ReverseBall;
  controller.iu.inport.start = StartBall;
  controller.check_bindings(); // Verify
}

static void StartBall()
{
  // TODO: Send message to Unity.  
}

static void ReverseBall()
{
  // TODO: Send message to Unity.
}
```

## Connecting to Unity

To connect to Unity, we must set up some objects that manage the transmitting and receiving of messages. The `DezyneSocketThread.dll` library and `System.Net.Sockets` namespace provide these objects.

In the `Program.cs` code:

* Add the following references at the top of the code:  
  ```cs
  using DezyneSocketThread.Threading;
  using DezyneSocketThread.Socket;
  using DMUNetworkData;
  using System.Threading;
  using System.Net.Sockets;
  ```
* Create a `TcpClient` object that manages the connection between the server and the client.
  *  The `SocketListener` helper class provides a static `AwaitConnection` method that will return a connected `TcpClient` object. Note that this method will block the current thread until Unity connects.
  * The following code is added above the `Main` method:
  ```cs
  static DezyneThread dezyneThread;
  static Thread thread;
  ```
  * The following code is added under the previously written code in the `Main` method:  
  ```cs
  TcpClient client = SocketListener.AwaitConnection("127.0.0.1", 80); // Blocks until Unity connects.
  dezyneThread = new DezyneThread(client);
  // TODO: Message handlers.
  thread = new Thread(dezyneThread.StartThread);
  thread.Start();
  ```

**It is important to note that port 80 may already be in use by your system. Feel free to change this port, but make sure you use the same port in Unity.**

Your server should now be able to wait for a connection from Unity. After the connection is made, the model can be interacted with. To start the model, we'll call `start` in `controller.ia` (`iCommand`) directly. You could also do this from a UI.

Under the line `thread.Start()`, we'll add:

```cs
controller.ia.inport.start();
```

## Outgoing Messages

The `StartBall` method will send a message to Unity, telling the ball to start moving.  
The `ReverseBall` method will make the ball reverse.

We've already created empty methods for these two messages, so let's add the necessary code.  

Using the `dezyneThread` instance, we can queue new messages. The messages will be sent in the order they are queued automatically. There should be virtually no delay here.

The messages we are going to send are `"StartBall"` and `"ReverseBall"`. Remember this, as we'll need them for the Unity implementation.

```cs
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
```

It is important to note that the Message, Sender and Receiver properties of the network message can have any value, as long as you keep them consistent on both the client and server.  
In this case, we leave out the Sender and Receiver because there's only one component in the model. In bigger models, you might want to specify which motor should handle `"Run"`.


## Incoming Messages

At some point in time Unity will raise the `"WallHit"` event and send it to the server. We need to handle this event, and there's two ways to achieve this.

Although you only need to follow one, both ways are described below. Do **not** implement the Global Message Handler** *and* Key-based Message Handler section, or messages will be handled twice.


### Global Message Handler

A global message handler responds to every single message. By parsing the message data, you could call the right method on the Dezyne model.

* In the `Main` method, add a listener to the `DezyneThread.OnMessageReceived` event.
  * Make sure you add this line in front of `controller.ia.inport.start();`.

```cs
dezyneThread.OnMessageReceived += OnMessageReceived;
```

* In the `OnMessageReceived` method, handle the message.

```cs
private static void OnMessageReceived(object sender, NetworkMessage message)
{
  switch (message.Message)
  {
    case "WallHit":
      controller.iu.outport.wallHit();
      break;
  }
}
```

### Key-based Message Handler

Instead of handling every message, you can also target specific messages. This may be a better choice for bigger models, as you won't end up with a bulky switch statement.

* In the `Main` method, subscribe a method specifically to the `WallHit` message.
  * Make sure you add this line in front of `controller.ia.inport.start();`.

```cs
dezyneThread.Subscribe("WallHit", WallHit);
```

* In the `WallHit` method, handle the message.
  * In this case, we don't need to parse the network message because we know the message is `WallHit`. With multiple components, you could parse additional data such as the sender or receiver.

```cs
private static void WallHit(NetworkMessage message)
{
  controller.iu.outport.wallHit();
}
```

---

### Server Implementation Done
You now have the server set up. When the Unity application connects to the server, the server will send the message `"StartBall"`,  which makes the ball start moving. After that, whenever Unity sends the message `"WallHit"` back, the server will respond with a `"Reverse"` message.  
Each interaction here passes through the Dezyne model:

  * `controller.ia.start()` will call `controller.iu.start()`, which is bound to `StartBall()`.
  * `controller.iu.wallHit()` will call `controller.iu.reverse()`, which is bound to `ReverseBall()`.

### Continue

Continue by implementing the Unity client.

#### [Tutorial - Client](Tutorial-Client.md)
