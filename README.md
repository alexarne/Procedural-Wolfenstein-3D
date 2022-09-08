# Procedural-Wolfenstein-3D

Game which closely resembles Wolfenstein 3D (1992), developed in C++ and the SFML library. The project is about exploring the rendering techniques from early game development and to achieve what appears to be 3D imagery through simple 2D calculations such as raycasting. It follows the techniques outlined in [lodev's tutorial](https://lodev.org/cgtutor/raycasting.html) but iterates upon it for increased performance and playability. 

## Demonstration

[YouTube video showcasing most of the features](link)

## Instructions



## TODOs



## Code Overview

A general overview of the code structure and what each file does is found in the [folder](./scripts/).

## Algorithm Implementations

[The implementation of Raycasting](link)

## Intuitive Understanding

* **Raycasting**

## Difficulties Encountered

* **Efficient Rendering**

    By working with the original tutorial, my initial plan was to use a frame buffer containing information about every individual pixel on the screen, which is then drawn after completion. This proved to be very inefficient, since all pixel calculations and assignments were done by the CPU, for upwards of 900 000 pixels, resulting in around 10 frames per second. To solve this, I took an approach similar to that of [Thomas van der Berg](https://github.com/tmsbrg/adventure3d/blob/master/src/main.cpp#L409-L419) by defining lines as two points and then sending that information to the GPU via SFML's [VertexArray](https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1VertexArray.php). The lines can then have texture mapped onto them, which works as a complete replacement to my implementation of affine texture mapping that I had previously implemented. The performance increase was phenomenal while drawing only the walls, which only required 2560 points per frame, but it is still limited by the amount of points (and lines) set by the CPU. While drawing the floor and the ceiling, many more lines are calculated, resulting in decreased performance when viewing a lot of the ceiling or floor.

## Known Bugs



## Final Words

