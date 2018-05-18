# DezyneMeetsUnity Tutorial
###### *Connecting a Dezyne Model to Unity.*

In this tutorial, we'll be looking at the integration of a simple Dezyne Model with Unity, in C#.

At the end of the tutorial, you'll have a server that interacts with both the Dezyne model and Unity, and a Unity project with virtualized hardware responding to and raising events.

The tutorial is split up into multiple parts, as the server and client implementation can be made separate from one another.

## Table of Contents

* [Prerequisites](#prerequisites)
* [Setting up Visual Studio](#setting-up-visual-studio)
* [Setting up Unity](#setting-up-unity)
* [Sources](#sources)
* [Tutorial](#tutorial)
  * [Bouncing Ball Dezyne Model](#bouncing-ball-dezyne-model)
  * [Continue](#continue)

## Prerequisites

* [Visual Studio](https://www.visualstudio.com/downloads/).
  * See [Setting up Visual Studio](#setting-up-visual-studio).
* [Unity](https://store.unity.com/) (`2017.2` or up recommended).
  * See [Setting up Unity](#setting-up-unity).
* [.NET Framework 4.5](https://www.microsoft.com/en-us/download/details.aspx?id=30653).
* [DezyneMeetsUnity Libraries](https://github.com/dezyne/community/DezyneMeetsUnity/DezyneMeetsUnity-Libs.zip).

## Setting up Visual Studio

If you already have Visual Studio set up for Unity projects, you may skip this.  

It is important to note that the tutorial has only been tested on Visual Studio Community 2017, due to the unavailability of other versions.

#### Visual Studio 2017

* [Visual Studio Downloads](https://www.visualstudio.com/downloads/)
  * Any edition will work.

When installing, make sure to select the following options:

* .NET desktop development (for the server).
* Game development with Unity (for the client).
* .NET Framework 4.5 targeting pack.

The .NET Framework options can be founder under `Individual Components` rather than `Workloads`.

#### Older versions

Older versions of Visual Studio should still work, as long as they support the NuGet client tools and Unity tools.

* Make sure your Visual Studio installation supports C# code (.NET desktop development, though it may be named differently).
* The NuGet Package Manager should be available by default in Visual Studio 2012 and later. If you do not have it, see: [Installing NuGet client tools](https://docs.microsoft.com/en-us/nuget/guides/install-nuget).
* Install the [Visual Studio Tools for Unity](https://msdn.microsoft.com/en-us/library/dn940025.aspx) for your version of Visual Studio.

## Setting up Unity

If you already have Unity set up, you may skip this.

* Grab the latest version of [Unity](https://store.unity.com/). The personal edition will work just fine, as we're not making a commercial product.
* Install Unity with the components selected by default.
* Sign in with your Unity ID, Google or Facebook account.
  * If you do not have an Unity ID, and do not wish to sign in with Google or Facebook, you can create an [Unity ID on the Unity website](https://id.unity.com/en/conversations/48de9bdc-c9e6-4d4e-993e-18b05e74df6e01af).

## Sources

To get started, you can download the **Server Startup** and **Client Startup** sources from the below table. Make sure to unpack the downloaded zip files before using any of the files.

The tutorial does not cover creating scenes and moving objects in Unity, so the Client Startup project should be used as a base.  
You can choose to start with the Server Startup project, but you can also make a new application. The tutorial covers everything needed to do that.

| Download | Description |
|---|---|
| [Server Startup](https://github.com/dezyne/community/DezyneMeetsUnity/Tutorial/BouncingBallServer2D-Begin) | Contains the Dezyne Runtime files and generated Dezyne model. JSON.Net can be installed using NuGet Package Restore. `Program.cs` still needs to be coded. |
| [Client Startup](https://github.com/dezyne/community/DezyneMeetsUnity/Tutorial/BouncingBallClient2D-Begin) | Contains the scene and components Unity uses. Does not contain the needed libraries nor any code related to the Dezyne integration. |
| [Final Server](https://github.com/dezyne/community/DezyneMeetsUnity/Tutorial/BouncingBallServer2D-Final) | A working version of the Bouncing Ball Server. |
| [Final Client](https://github.com/dezyne/community/DezyneMeetsUnity/Tutorial/BouncingBallServer2D-Final) | A working version of the Bouncing Ball Client. |

Documents we recommend reading:

| Link | Description |
|---|---|
| [DMUnity](DMUnity.md) | More detailed information about the DMUnity (DezyneMeetsUnity) package for Unity. Contains information on how to use all of the components, and how to write your own code that interacts with these components. |
| [DMUNetworkData](DMUNetworkData.md) | Information on the DMUNetworkData library, which is used to send and receive network messages. Describes the actual serialized message format, and shows how you could make derived network messages for custom data. |

## Tutorial

The tutorial is split up into multiple parts. First, we'll explain what the Dezyne model does, and then we'll dive into the integration.

The integration in Unity is separate from the server integration, so these two steps can be followed in any order. We suggest implementing the server first.

### Bouncing Ball Dezyne Model

The Dezyne Model we'll be using is small and simple, but should cover the basics needed to start integrating other models.

The model represents a system where an object (ball) bounces between two solid objects (walls), each on one side of the ball. When the ball collides with either of the walls, the ball will raise an event (`wallHit`) and in response the Dezyne model will reverse the ball (`reverse`).

The `iCommand` interface is used to interact with the model as a user; to start the ball moving.  
The `iUnity` interface contains all events related to the hardware (sends `reverseBall` and receives `wallHit`).

![](../Images/Tutorial/DezyneModel.png)

The Dezyne model itself doesn't know anything about the walls, meaning the positions or the amount of walls won't matter. As long as the ball will hit a wall at some point in time, it will be reversed.

The entire model can be copied from the below code, but the generated code is already present in the server [sources](#sources).

* `bouncingBall.dzn`:

```cpp
interface iCommand {
	in void start();
	behaviour {
		on start: {}
	}
}

interface iUnity {
	in void start();
	in void reverse();
	out void wallHit();

	behaviour {
		on inevitable: wallHit;
		on reverse, start: {}
	}
}

component Controller {
	provides iCommand ia;
	requires iUnity iu;

	behaviour {
		on ia.start(): {iu.start();}
		on iu.wallHit(): {iu.reverse();}
	}
}
```

### Continue

You can choose to implement the server or the client first.  
The server application will interact with the Dezyne model, and the client (Unity) application will represent the hardware of the machine.

#### [Tutorial: Server](Tutorial-Server.md#server)
#### [Tutorial: Client](Tutorial-Client.md#client)
