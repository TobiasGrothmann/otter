# Otter – a pen plotting framework

![banner](https://user-images.githubusercontent.com/28928394/158377724-c0ce651a-3e40-4e87-a158-67625f359506.png)

## What?

Framework to create generative plotter art. It contains:

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

This is a hobby project I personally use to create pen-plotter art. The development requirements for this repo are to implement functionality I currently need, in as little time as possible. Many parts of this framework are therefore likely to be unfinished and bug-infested.<br>

Some results, created using Otter, can be seen on [instagram](https://www.instagram.com/tobias_grothmann/):


![truchet pattern](https://user-images.githubusercontent.com/28928394/158366284-a0d17881-4360-4dd7-ad9a-5d27eabe36bf.jpg)
![truchet pattern](https://user-images.githubusercontent.com/28928394/158366954-50e8a964-d1cb-4aa4-ab8a-cc0a1925403c.jpg)
![truchet pattern](https://user-images.githubusercontent.com/28928394/158366290-a5b1fdd1-2b3b-4669-9d10-7ebd00dca64e.jpg)
![hexagonal truchet pattern](https://user-images.githubusercontent.com/28928394/158366314-a822e9c3-e134-4edc-9a4f-c86b8ece1f27.jpg)
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
