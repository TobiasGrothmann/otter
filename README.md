# Otter – a pen plotting framework

![banner](https://user-images.githubusercontent.com/28928394/158377724-c0ce651a-3e40-4e87-a158-67625f359506.png)

Otter is a framework to create generative plotter art. It contains:

* **Otter** – core library
* **OtterUtil** – library for extended functionality using third party libraries:
	* Voronoi – [jc_voronoi by Mathias Westerdahl](https://github.com/JCash/voronoi)
	* Image handling – [OpenCV](https://opencv.org/)
	* clipping, offset and boolean operations of paths – [Clipper by Angus Johnson](http://www.angusj.com/delphi/clipper.php)
	* QR-Code generation – [QR Code generator library by Nayuki](https://github.com/nayuki/QR-Code-generator)
	* SVG – [Simple SVG by Mark Turney](https://github.com/MarkTurney/simple-svg) and [Nano SVG by Mikko Mononen](https://github.com/memononen/nanosvg)
	* Font handling – [ttf-parser by kv01](https://github.com/kv01/ttf-parser)
* **OtterCLI** – command line utility to create and manage otter projects
* **OtterUI** – preview to display projects and remotely control otterServer
* **OtterServer** – server running on a raspberry pi controlling the pen-plotter


## Who is this for?

This is a hobby project I personally use for recreational pen-plotter art. The development requirements for this repo are to implement functionality I currently need, in as little time as possible. Many parts of this framework are therefore likely to be unfinished and/or not tested.<br>Otter is kept fast-moving and is always adjusted to new creative constraints I find myself in. It's a nimble otter.

<p align="center">
	<img src="https://user-images.githubusercontent.com/28928394/158394767-f53f723f-86a3-45d1-84fd-8e63b3a6f897.jpg" alt="nimble otter" width="350"/>
</p>


## Some examples

Some results, created using Otter, can be seen on [instagram](https://www.instagram.com/tobias_grothmann/):

![truchet pattern](https://user-images.githubusercontent.com/28928394/158366954-50e8a964-d1cb-4aa4-ab8a-cc0a1925403c.jpg)
![truchet pattern](https://user-images.githubusercontent.com/28928394/158366290-a5b1fdd1-2b3b-4669-9d10-7ebd00dca64e.jpg)
![hexagonal truchet pattern](https://user-images.githubusercontent.com/28928394/158366314-a822e9c3-e134-4edc-9a4f-c86b8ece1f27.jpg)


## Build on macOS

Because some parts of the framework have to be compiled on Mac and Raspberry Pi, a mixture of Xcode and Makefiles is used for compilation.

Install requirements:

* boost (`brew install boost`)
* opencv 2 (`brew install opencv@2`)
* xcode build tools (`xcode-select --install`)

Building `Otter`, `OtterUtil`, `OtterUI` and `OtterServer` on **macOS Monterey** should be as simple as:

Clone the repository:

```
git checkout https://github.com/TobiasGrothmann/otter.git
cd otter
```

Compile:

```
make
```

The build products are placed in `build`.<br>
Once that has succeeded, you can install with:

```
sudo make install
```

This step will also install `OtterCLI` and set the environment variable `OTTER_HOME` to the path your otter repository is located.

----

If you only want to compile e.g. the Otter core library, change directory into the required folder (`cd Otter`) and use `make` and `sudo make install` there.


## Build on Raspberry Pi

OtterServer is designed to control my DIY plotter. If you'd like to use it, there'll likely be some adjustments you will have to make. A "configuration file" is given through [Settings.hpp](https://github.com/TobiasGrothmann/otter/blob/main/OtterServer/src/OtterServer/Settings.hpp). The hostname of the Raspberry Pi is expected to be `otter` by the library.

Install requirements:

* boost (`sudo apt install boost`)

On your Raspberry Pi:

```
make
```

and

```
sudo make install
```
will build and install only the **Otter core** (*libotter.a*) library and the **Otter Server** (*otterServer*) executable.



## Creating a project

If installed, the Otter CLI can be used to create a new project:

```
otter create path/myNewProject
```

A directory containing your new Xcode project will be created, including some boiler plate code to get you started:

<img width="1072" alt="Screenshot 2022-03-15 at 14 41 02" src="https://user-images.githubusercontent.com/28928394/158390513-1ec90cfb-5881-45d4-b453-8d766fea2634.png">


## Using Otter

Have a look at `otter/examples` to get started.

Creating shapes:

```c++
// circle at x:1, y:2 with radius:3
CirclePtr circle = Circle::create(Vec2(1, 2), 3);

// rectangle at bottomLeft: x:0 y:0; topRight: x:5 y:5
RectanglePtr rect = Rectangle::create(Vec2::xy(0), Vec2::xy(5));

// closed triangle
PathPtr path = Path::create({ Vec2(0, 0), Vec2(1, 0), Vec2(0.5, 1) });
path->close();
```

Creating a sketch:

```c++
Queue q = Queue();
q.add(circle);
q.add(rect);
q.add(path);
// alternatively:
q.circle(Vec2(5, 5), 5);
q.rectangle(Vec2(0, 0), Vec2(5, 5));
q.path({ Vec2(0, 0), Vec2(5, 5), Vec2(5, 0), Vec2(0, 0) });
```


## Gallery

![20211004-DSCF4974](https://user-images.githubusercontent.com/28928394/158399767-cab1ad4e-4d46-48b4-ad95-b05a8edf637c.jpg)
![truchet pattern](https://user-images.githubusercontent.com/28928394/158366293-5a5fd4a6-d61d-46fa-ad4c-4759ec53a5a3.jpg)
![swirly shape](https://user-images.githubusercontent.com/28928394/158366297-95e9673c-a57c-4981-aaf0-36b36b7a3532.jpg)
![random noise lines](https://user-images.githubusercontent.com/28928394/158366300-a7241a95-7f8f-4fd3-8206-173f1cb4dba3.jpg)
![truchet pattern](https://user-images.githubusercontent.com/28928394/158366273-a887dba8-4ee4-4eb5-a495-8db5c363d598.jpg)
![truchet pattern](https://user-images.githubusercontent.com/28928394/158366303-854ead3e-03d4-4f17-823e-8a8847b2bb0b.jpg)
![swirly shape](https://user-images.githubusercontent.com/28928394/158366307-1bae2cb3-6eda-4114-b35f-45adc1c8d76c.jpg)
![hexagonal truchet pattern](https://user-images.githubusercontent.com/28928394/158366309-d2844a7b-6216-4835-bcdb-64b70a84a47d.jpg)
![truchet pattern](https://user-images.githubusercontent.com/28928394/158366313-39dec85e-abb1-46d5-ace4-cb8f0e5e2e22.jpg)
![3d balls with hexagonal mesh](https://user-images.githubusercontent.com/28928394/158366316-70ae6ca0-6b57-43ba-a88e-6e39c944f352.jpg)
![pattern of squares distorted by noise](https://user-images.githubusercontent.com/28928394/158366305-bd4a5dab-db85-4eb3-a5ea-9b74188b0dde.jpg)
