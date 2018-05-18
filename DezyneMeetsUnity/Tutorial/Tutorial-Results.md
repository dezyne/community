# DezyneMeetsUnity - Results

##### [- Return to Client Integration](Tutorial-Client.md)

With both the server and client implemented, it is time to show the model in action.

## Table of Contents

* [Build Server](#build-server)
* [Build Client](#build-client)
* [Running the model](#running-the-model)

## Build Server

> You can also run the code directly from Visual Studio. This will allow you to debug, if necessary.

* Change the Build Target to **Release**.
* In the Build menu, press <kbd>Build Solution</kbd>.
* Right click the project in the Solution Explorer, and select <kbd>Open Folder in File Explorer</kbd>.
* Open the folder `bin` and then `Release`.
* Copy the following files (for example to a USB stick), that are needed to run the applications:
  * `BouncingBallServer.exe` (or the name of your project)
  * `DezyneSocketThread.dll`
  * `DMUNetworkData.dll`
  * `Newtonsoft.Json.dll`
    * This library is licensed under a [MIT license](https://raw.githubusercontent.com/JamesNK/Newtonsoft.Json/master/LICENSE.md).

## Build Client

> You can also run the application directly from the Unity Editor.

* In the File menu, press <kbd>Build and Run</kbd>.
* Choose a file name for the executable (I.e. `BouncingBallClient.exe`), and press <kbd>Save</kbd>.
  * The build process will create more files than just the executable. Please use an empty folder as the destination.
* When the build is complete, it will open a configuration screen. You can close this, unless you want to run the application.

## Running the model

* Start the server by opening your `BouncingBallServer.exe` file.
* Start the client by opening your `BouncingBallClient.exe` file.
  * You may want to check `Windowed` and use a lower resolution, to see both the client and server.
* In the Unity client, press <kbd>Connect</kbd> to start the model.

---

![](../Images/Tutorial/Client.png)
