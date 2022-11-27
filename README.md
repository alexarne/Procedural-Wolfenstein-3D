# Procedural-Wolfenstein-3D

Game which closely resembles Wolfenstein 3D (1992), developed in C++ using the SFML library. The project is about exploring the rendering techniques from early game development and to achieve what appears to be 3D imagery through simple 2D calculations such as raycasting. It follows the techniques outlined in [lodev's tutorial](https://lodev.org/cgtutor/raycasting.html) but iterates upon it for increased performance and playability. 

> Due to difficulties with implementing textured floors and ceilings, this project has been put on pause.

![Pixel Rendering](/images/per_pixel.png)

Rendering the frames pixel-by-pixel was easy and straightforward, but lead to way too poor performance, especially considering this is basically 90's graphics on a modern computer. The issue (explained in detail below) was not utilizing the GPU properly and leaving everything to the CPU.

![Line Rendering](/images/gpu_lines.png)

When trying to fix that issue by rendering lines on the GPU instead of pixels, the issue became instead how to draw the floor and ceiling, since those lines aren't straight. This became a difficult and time consuming task, which is why I haven't been able to finish it yet.

## Demonstration (WIP)

[YouTube video showcasing most of the features (WIP)](link)

## Instructions (WIP)

To come

## TODOs (WIP)

To come

## Code Overview (WIP)

A general overview of the code structure and what each file does is found in the [folder](./scripts/).

## Algorithm Implementations

[The implementation of Raycasting](link)

## Intuitive Understanding

* **Raycasting**

    We choose to draw the walls as sequential vertical lines, one for each horizontal pixel, and by changing the height of the vertical lines we may create the appearance of a rotated wall and therefore depth. By letting walls only be vertical to the ground and the player's view direction to always be parallel to the ground we only have to draw the walls as straight lines. A wall line is (usually) centered in the middle so what is left to calculate is the height of the line. This is inversely proportional to the perpendicular distance from the player's camera plane. To get the distance, we need to know the position of where the wall was hit. For that, we send out a ray for each line that has to been drawn and use Digital Differential Analysis (DDA) to step through each tile until a wall has been hit.
    
* **Digital Differential Analysis**

    To come

## Difficulties Encountered

* **Efficient Rendering**

    By working with the original tutorial, my initial plan was to use a frame buffer containing information about every individual pixel on the screen, which is then drawn after completion. This proved to be very inefficient, since all pixel calculations and assignments were done by the CPU, for upwards of 900 000 pixels, resulting in around 10 frames per second. To solve this, I took an approach similar to that of [Thomas van der Berg](https://github.com/tmsbrg/adventure3d/blob/master/src/main.cpp#L409-L419) by defining lines as two points and then sending that information to the GPU via SFML's [VertexArray](https://www.sfml-dev.org/documentation/2.5.1/classsf_1_1VertexArray.php). The lines can then have texture mapped onto them, which works as a complete replacement to my implementation of affine texture mapping that I had previously implemented. The performance increase was phenomenal while drawing only the walls, which only required 2560 points per frame, but it is still limited by the amount of points (and lines) set by the CPU. While drawing the floor and the ceiling, many more lines are calculated, resulting in decreased performance when viewing a lot of the ceiling or floor.

## Known Bugs

To come

## Final Words

To come
