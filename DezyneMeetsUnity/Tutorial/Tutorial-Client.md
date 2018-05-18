# DezyneMeetsUnity - Client

##### [- Return to Server Integration](Tutorial-Server.md)

The client is a Unity application that will act as the 'hardware' of the system.

## Table of Contents

* [Base Project](#base-project)
* [Installing libraries](#installing-libraries)
* [Connecting to Server](#connecting-to-server)
* [Receiving Messages](#receiving-messages)
* [Sending Messages](#sending-messages)
* [Client Implementation Done](#client-implementation-done)
  * [Continue](#continue)

## Base Project

Near the start of this tutorial, there's a [link to the Client Startup project](Tutorial-Begin.md#sources). Make sure you have downloaded and unpacked this project.

* Open the project.
  * You can open Unity projects by first starting up Unity, and then selecting <kbd>Open</kbd>.
  * Make sure you select the unpacked `BouncingBallClient2D` folder.

Unity might warn you that your Unity Editor version does not match the project version. You may ignore this warning, as long as your version is newer than the project version. If this is not the case, please consider updating Unity.

This project includes a ball and two walls.
The ball is idle but can start and stop moving. When the ball collides with either of the walls, it will raise a collision event that currently does nothing. The ball can also be reversed with a simple method call.

* After opening the project, open the `MainScene` file from the Assets window to see the scene.


## Installing libraries

> ##### <span style="color:red">The libraries are already present in the Startup project. This information is here in case you want to use the libraries in other Unity projects.</span>

For Unity, we also need to install a couple of libraries (packages).

* Install `Json.NET` for Unity by dragging the `JsonNet.9.0.1.unitypackage` file into your Assets.
  * Uncheck the sample files (`JsonNetSample`) and select <kbd>Import</kbd>.
* Install `DMUnity` by dragging the `DMUnity.unitypackage` file into your Assets.
  * Make sure everything is checked, and select <kbd>Import</kbd>.

## Connecting to Server

To connect to and communicate with the server, there should be one DezyneConnector component present in the scene.

* In the Hierarchy, create an empty gameobject (`Create Empty`).
  * For convenience, change the name of the object to `DezyneConnector` by pressing <kbd>F2</kbd> after selecting the object.
* In the Inspector, add a `DezyneConnector` component to the gameobject by pressing <kbd>Add Component</kbd>.
* If you changed the port from 80 to another value on the server, make the same change for the newly added `DezyneConnector` component (in the Inspector).

With the `DezyneConnector` component in the scene, we still need something to tell it to connect to the server. The sample project contains a button that will be used to connect to the server.

* In the Hierarchy, select the `ConnectButton`.
  * You can search for it by entering the name in the search box, or you can unfold `Canvas` and `Background`.
* In the Inspector there's an `On Click ()` section. This represents the event(s) to invoke when the button is pressed.
* Select <kbd>`+`</kbd> to add a new event.
* Drag the `DezyneConnector` gameobject from the Hierachy into the `None (Object)` field.
* Select `No Function` to the right of the object field, and select `DezyneConnector > Connect()` from the dropdown.

Now, whenever the Connect button is pressed it will try to connect to the server.

Assuming the server will start the model when the connection is made, the Unity application is now idle until the server sends its first message.

## Receiving Messages

The server sends two different possible messages to Unity. One to start the ball, and one to reverse the ball.  
As both messages are handled by the ball, we're only going to be making changes to the `Ball` gameobject.

* In the Hierarchy, select the `Ball` gameobject.
* In the Inspector, add a `DznMessageReceiver` component by pressing <kbd>Add Component</kbd>.
  * You may need to scroll down to see the button.
* Unfold the `Message Handlers`, and change the `Size` to 2.
* Unfold `Element 0` and `Element 1`.
* Under `Element 0`:
  * Change the `Message` to `StartBall`.
  * Select <kbd>`+`</kbd> to add a new event.
  * Drag the `Ball` gameobject to `None (Object)`.
  * Select `No Function` to the right, and select `MovingObject > StartMoving()`.
* Do the same for `Element 1`, but set the message to `ReverseBall` and the event to `MovingObject > Reverse()`.

Use the below image as a reference.  
![](../Images/Tutorial/DznMessageReceiver.png)

## Sending Messages

When the ball collides with a wall, it should send the message `"WallHit"` back to the server.

* In the Hierarchy, select the `Ball` gameobject.
* In the Inspector, add a `DznMessageSender` component by pressing <kbd>Add Component</kbd>.
  * You may need to scroll down to see the button.
* Unfold `Messages` and change the `Size` to 1.
* Unfold `Element 0` and set the `Message` to `WallHit`.

With the message defined, we still need to send it when the ball collides with a wall.

* Open the `MovingObject` code by opening it from the Assets view. It is located in the `Scripts` folder. Visual Studio will open, which is used to write code for Unity projects.
* The method `OnCollisionEnter` is called when the ball hits either of the walls. This is where we want to send the message back.
* First, we get the `DznMessageSender` component using the Unity method `GetComponent<T>()`. This allows us to call the methods of the component.
* We use the `SendNetworkMessage("string")` method to send a message to the server.
  * The string should match the value entered in the Inspector (`"WallHit"`).

```cs
private void OnCollisionEnter(Collision collision)
{
  DznMessageSender sender = GetComponent<DznMessageSender>();
  sender.SendNetworkMessage("WallHit");
}
```

The `WallHit` message will now be sent when the ball collides with either of the two walls (granted the server and client are connected).

## Client Implementation Done

You now have the client set up.  
When pressing <kbd>Connect</kbd>, the client will connect to the server. For this to work, the server must already be running. After the connection is made, the server will tell the client to start moving the ball. When the ball collides with either wall, it will notify the server, and the server will tell the ball to reverse in response.

### Continue

Continue by seeing if the server and client work together.

#### [Tutorial - Results](Tutorial-Results.md)
