# DMUNetworkData
###### *DezyneMeetsUnity C# Network Data Library*

This C# library contains the serializable data class that is sent and received by the Unity application.

Data is serialized to JSON, so that it can easily be parsed by just about any language. For the C# library, the Json.NET library by Newtonsoft is used for the serialization and deserialization (hence the Json-specific attributes in the source code).

Any C# project that uses Json.NET can directly use this library, but for other languages other (de)serializers would have to be used.

| Library | Link |
|---|---|
| Json.NET for .NET | https://www.newtonsoft.com/json |
| Json.NET for Unity | https://github.com/SaladLab/Json.Net.Unity3D |

For use outside of Unity, it is recommended to use [version 9.0.1 of Json.NET](https://www.nuget.org/packages/Newtonsoft.Json/9.0.1). This is the same version as the port for Unity (linked above).

## Installing DMUNetworkData

### Visual Studio

To make use of the C# library, all you have to do is add a reference to the `DMUNetworkData.dll` file in your project. It will automatically be added to your output folder when creating debug or release versions of your application.

Adding the library can be done by following the below steps.
1. Under **Project**, select **Add Reference**.
2. Select **Browse** and locate the `DMUNetworkData.dll`.
3. Confirm adding the reference by pressing **OK**.

### Unity

In Unity, adding references is done automatically simply by putting the library into the project assets.
It is recommended to put the file(s) into a folder named `DMUNetworkData`.

1. Drag and drop the `DMUNetworkData.dll` file into your assets.
2. Optionally, drag and drop the `DMUNetworkData.xml` file into your assets, for access to documentation.

![Files in Unity](../Images/DMUNetworkData/Unity-Reference.png "Files in Unity")

---

You can now make use of the `NetworkMessage` class in your code by including the `DMUNetworkData` namespace.
```cs
using DMUNetworkData;
```

## NetworkMessage

This section will describe the JSON format of the `NetworkMessage` data object, allowing platform and language independent implementations for (de)serialization of the same data.

### Length prefixing

Relying on only one Json object being present in the network stream at any given point can lead to unexpected results. To solve this, each network message is prefixed with the length of the object (in a 32-bit signed integer / 4 bytes).
Reading the first 4 bytes, and then reading the amount of bytes this number represents, allows you to read one Json object (network message).

These bytes should always be stored in big-endian order.

| Number | Bytes[0] | Bytes[1] | Bytes[2] | Bytes[3] |
|---|---|---|---|---|
| 124 | `0x00` | `0x00` | `0x00` | `0x7C` |
| 256 | `0x00` | `0x00` | `0x01` | `0x00` |

A network message that serializes to `{"ObjectType":"NetworkMessage","Message":"wallHit"}` would be sent as `51{"ObjectType":"NetworkMessage","Message":"wallHit"}` (not literally this message, as the number here is 2 characters rather than 4 bytes).

#### Serialized

These properties are serialized, if defined. Strings that haven't been assigned a value (`null`) will not be serialized by Unity. The `Data` object is always serialized, even if empty.

| Name | Type | Value |
|---|---|---|
| ObjectType | `string` | Stores the name of the (derived) NetworkMessage class, which can be used to deserialize the data to the same class type. |
| MessageId | `int` | Identifier that should increment every message. Defaults to `-1`. |
| Message | `string` | Plain text message. |
| Sender | `string` | Dezyne model path for the component responsible for sending the message. |
| Receiver | `string` | Dezyne model path for the component that should handle the message. |
| Data | `object` | Key mapped data. Can be used to provide additional data (or even other serializable objects). |

#### Non-Serialized
These properties are not serialized, but they automatically defined when the data is (de)serialized.

| Name | Type | Value |
|---|---|---|
| MessageType | `int` | `0` (sent) or `1` (received). |
| Timestamp | [DateTime](https://msdn.microsoft.com/en-us/library/system.datetime) | Time of deserialization (time of receiving the message). |

#### Examples

All below messages are valid. As they increase in complexity more data becomes available, which can make it easier to implement the message handlers.

```json
{
  "ObjectType": "NetworkMessage",
  "MessageId": -1,
  "Message": "wallHit"
}
```

```json
{
  "ObjectType": "NetworkMessage",
  "MessageId": -1,
  "Message": "wallHit",
  "Sender": "Unity.ballA",
  "Receiver": "Controller.ballA.wallHit"
}
```

```json
{
  "ObjectType": "NetworkMessage",
  "MessageId": -1,
  "Message": "wallHit",
  "Sender": "Unity.ballA",
  "Receiver": "Controller.ballA.wallHit",
  "Data": {
    "Position": [127, 0, 12.125],
    "Collider": "Unity.wallA"
  }
}
```

### Derived Classes

In C#, it is easy to make derived classes that get properly (de)serialized by the existing code. These classes should inherit the `NetworkMessage` class.

[Json Serialization Attributes](https://www.newtonsoft.com/json/help/html/SerializationAttributes.htm) can be added to define the (de)serialization behaviour. These are part of the Json.NET framework. Omitting these attributes will not cause any issues.

The below example exposes a new property `Status`.

```cs
using DMUNetworkData;

public class DerivedNetworkMessage : NetworkMessage
{
    public string Status { get; set; }
}
```

For this example class, the serialized Json would look like:

```json
{
  "ObjectType": "DerivedNetworkMessage",
  "MessageId": -1,
  "Status": "MyStatusValue"
}
```
