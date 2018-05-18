# DMUnity

Using the DMUnity package for Unity, we can send and receive messages to simulate hardware components. This allows us to test Dezyne model integrations without actual hardware.

## Package
The DMUnity package contains the base components that are necessary to send and receive messages to and from the application that integrates a Dezyne model. In this document we will refer to this application as the 'Dezyne application'.

### DezyneConnector

This component manages the connection between Unity and the Dezyne application. There should always be one instance of this component loaded.  
Want to keep the connector loaded when switching scenes? See [Object.DontDestroyOnLoad](https://docs.unity3d.com/ScriptReference/Object.DontDestroyOnLoad.html).

Other components will automatically use the DezyneConnector in your scene. As such, it does not matter where you put the component. You can put it on an existing game object (such as the main camera), or put it on a new empty game object.

#### Properties

These can be set in the Unity Inspector, by selecting the game object that has this component attached to it.

| Key | Value |
|---|---|
| Host | The IP address of the server running the Dezyne application. In most cases, this can be left at it's default value (`127.0.0.1`). |
| Port | The port the Dezyne application is using on the server. In most cases, this can be left at it's default value (`80`). |
| Case Sensitive | Indicates whether received messages should be handled case sensitive.<br>For example, enabling this prevents the message `startMoving` from activating a `DznMessageReceiver` that is listening to `StartMoving`.

#### Connecting

Before you can send and receive messages, you must first connect to the Dezyne application (server). The Dezyne application should be running and listening to connections.

To connect to the server, you must call the `Connect` method from your own code.

```cs
dezyneConnector.Connect();
```

You can accomplish this by creating a reference to the DezyneConnector by having a public DezyneConnector field in another component. By pressing the circle next to the Dezyne Connector field under `My Script`, you can select any valid components for this field. Since there should only be one `DezyneConnector` present, it's easy to make the selection from the list.

##### MyScript.cs

```cs
using UnityEngine;

public class MyScript : MonoBehaviour
{
  public DezyneConnector dezyneConnector;

  void Start()
  {
    dezyneConnector.Connect();
  }
}
```

##### Reference in Inspector

![Reference to DezyneConnector](../Images/DMUnity/DezyneConnectorReference.png)
![Selecting the DezyneConnector](../Images/DMUnity/SelectDezyneConnector.png)

---

#### Receiving messages

The `DezyneConnector` will receive all messages sent from the Dezyne application, if a connection is made. There are two possible implementations to receive messages.  
Please note that messages are sent to every component or script listening to the message.

##### Receiving all messages

Through the `OnReceiveMessage` event you can handle all messages, regardless of the content. This event can be found in the `DezyneConnector` script.

```cs
using UnityEngine;

public class MyScript : MonoBehaviour
{
    public DezyneConnector dezyneConnector;

    void Start()
    {
        dezyneConnector.OnReceiveMessage += OnReceiveMessage;
        dezyneConnector.Connect();
    }

    private void OnReceiveMessage(DMUNetworkData.NetworkMessage message)
    {
        switch (message.Message.ToLowerInvariant())
        {
            case "doThis":
                Debug.Log("Do this.");
                break;
            case "doThat":
                Debug.Log("Do that.");
                break;
        }
    }
}
```

#### Receiving messages matching criteria

The `DznMessageReceiver` component can be attached to game objects where you only handle specific messages sent by the Dezyne application to Unity. This makes it easier to delegate different tasks (messages) to different components (virtualized hardware).

You can use as many `DznMessageReceiver` components as needed. These will automatically link themselves to the `DezyneConnector` at runtime. Do note that one game object may only contain a single `DznMessageReceiver` component.

The below settings for a `DznMessageReceiver` component will invoke `MySensor.Activate(networkMessage)` if the received network message matches the following criteria:

| Key | Value |
|---|---|
| Message | The received `networkMessage.Message` should match "activateSensor". |
| Sender (optional) | The message sender is not checked, as no value is given. |
| Receiver (optional) | The `networkMessage.Receiver` should match "MySensor". |

![MySensor MessageReceiver](../Images/DMUnity/MySensorMessageReceiver.png)

##### MySensor.cs

```cs
using UnityEngine;
using DMUNetworkData;

public class MySensor : MonoBehaviour
{
    public void Activate(NetworkMessage message)
    {
        // Do stuff.
    }
}
```

If you don't need access to the network message, you can also omit this parameter in the method to call (in this case `Activate`). You will still be able to select the method in the inspector.

To handle more messages, simply increase the `Size` of the `Message Handlers` collection in the inspector, and fill in the details for each message to handle.

---

#### Sending messages
There's also two ways of sending messages from Unity back to the Dezyne application. Some examples include sending a message when a button is pressed, or when a component is done handling an action.

Both methods are found in the `DznMessageSender`, which can be added to every game object where you wish to send messages. These components will link themselves to the `DezyneConnector` automatically. To access the methods of this component, you need to add a reference to the component in your own scripts.

![](../Images/DMUnity/DznMessageSenderReference.png)

```cs
using UnityEngine;

public class MySensor : MonoBehaviour
{
    public DznMessageSender messageSender;
}

```

##### Sending messages directly

You can send messages directly by calling `messageSender.SendNetworkMessage`.

`dezyoneConnector.client.AddSendMessages(networkMessage)`

This will add the message to a queue, and it will be sent to the server automatically.

##### MyScript.cs

```cs
using DMUNetworkData;
using UnityEngine;

public class MyScript : MonoBehaviour
{
    public DznMessageSender messageSender;

    void Start()
    {
        SendMessage();
    }

    public void SendMessage()
    {
        messageSender.SendNetworkMessage(new NetworkMessage()
        {
            Message = "HelloWorld"
        });
    }
}
```
##### Sending predefined messages

The `DznMessageSender` component can also be used to create a skeleton for messages.

![](../Images/DMUnity/MessageSenderSkeleton.png)

The below code will send a new message, containing `Message: SensorTriggered, Sender: MySensor, Receiver: MyHandler`.

```cs
using UnityEngine;

public class MySensor : MonoBehaviour
{
    public DznMessageSender messageSender;

    public void Start()
    {
        // This call will send a message back to the server.
        // Of course, the sensor doesn't actually trigger when the object is loaded, so Start isn't the best place to define it.
        TriggerSensor();
    }

    public void TriggerSensor()
    {
        messageSender.SendPredefinedNetworkMessage("SensorTriggered");
    }
}
```
