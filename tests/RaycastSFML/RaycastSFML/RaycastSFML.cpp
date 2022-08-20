// Raycasting 3D adventure game, based on http://lodev.org/cgtutor/raycasting.html
// Inspired by seeing https://github.com/TheMozg/awk-raycaster
//
// Copyright Thomas van der Berg, 2016, some parts taken from aforementioned tutorial copyrighted by its author
// Licensed under GNU GPLv3 (see LICENSE), tutorial parts liberally licensed(see LICENSE.tutorial)

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <unordered_map>
#include <SFML/Graphics.hpp>

const int screenWidth = 1280;
const int screenHeight = 720;

const float cameraHeight = 0.66f; // height of player camera(1.0 is ceiling, 0.0 is floor)

const int texture_size = 512; // size(width and height) of texture that will hold all wall textures
const int texture_wall_size = 128; // size(width and height) of each wall type in the full texture

const float fps_refresh_time = 1.1f; // time between FPS text refresh. FPS is smoothed out over this time

// list of wall texture types, in order as they appear in the full texture
enum class WallTexture {
    Smiley,
    Red,
    Bush,
    Sky,
    Pink,
    Wallpaper,
    Dirt,
    Exit,
};

// valid wall types and their texture for the world map
const std::unordered_map<char, WallTexture> wallTypes{
    {'#', WallTexture::Pink},
    {'=', WallTexture::Dirt},
    {'M', WallTexture::Wallpaper},
    {'N', WallTexture::Bush},
    {'~', WallTexture::Sky},
    {'!', WallTexture::Red},
    {'@', WallTexture::Smiley},
    {'^', WallTexture::Exit},
};

// size of the top-down world map in tiles
const int mapWidth = 24;
const int mapHeight = 24;

// top-down view of world map
const char worldMap[] =
"~~~~~~~~~~~~~~~~MMM@MMMM"
"~..............=M......M"
"~..............=M......M"
"~..............=@......@"
"~..............=M......M"
"~....N......N..........M"
"~..............=MMM@MM.M"
"~..............======M.M"
"~..............=MMMMMM.M"
"~..............=M......M"
"~...N....N.....=M..N..M#"
"~.....................M#"
"~..............=M..N..M#"
"~..............=M.....M#"
"~...........N..=MMMMM.M#"
"~..............======.=#"
"#.!!!!!!.!!!!!!........#"
"#.!....!.!..........=..#"
"#.!.N..!.!..==..=...=..#"
"#...........==..==..=..#"
"#.!!!!!!.!..==.........#"
"#.######.#..==....=....#"
"N......................^"
"########################";

// get a tile from worldMap. Not memory safe.
char getTile(int x, int y) {
    return worldMap[y * mapWidth + x];
}

// checks worldMap for errors
// returns: true on success, false on errors found
bool mapCheck() {
    // check size
    int mapSize = sizeof(worldMap) - 1; // - 1 because sizeof also counts the final NULL character
    if (mapSize != mapWidth * mapHeight) {
        fprintf(stderr, "Map size(%d) is not mapWidth * mapHeight(%d)\n", mapSize, mapWidth * mapHeight);
        return false;
    }

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            char tile = getTile(x, y);
            // check if tile type is valid
            if (tile != '.' && wallTypes.find(tile) == wallTypes.end()) {
                fprintf(stderr, "map tile at [%3d,%3d] has an unknown tile type(%c)\n", x, y, tile);
                return false;
            }
            // check if edges are walls
            if ((y == 0 || x == 0 || y == mapHeight - 1 || x == mapWidth - 1) &&
                tile == '.') {
                fprintf(stderr, "map edge at [%3d,%3d] is a floor (should be wall)\n", x, y);
                return false;
            }
        }
    }
    return true;
}

// check if a rectangular thing with given size can move to given position without colliding with walls or
// being outside of the map
// position is considered the middle of the rectangle
bool canMove(sf::Vector2f position, sf::Vector2f size) {
    // create the corners of the rectangle
    sf::Vector2i upper_left(position - size / 2.0f);
    sf::Vector2i lower_right(position + size / 2.0f);
    if (upper_left.x < 0 || upper_left.y < 0 || lower_right.x >= mapWidth || lower_right.y >= mapHeight) {
        return false; // out of map bounds
    }
    // loop through each map tile within the rectangle. The rectangle could be multiple tiles in size!
    for (int y = upper_left.y; y <= lower_right.y; ++y) {
        for (int x = upper_left.x; x <= lower_right.x; ++x) {
            if (getTile(x, y) != '.') {
                return false;
            }
        }
    }
    return true;
}

// rotate a given vector with given float value in radians and return the result
// see: https://en.wikipedia.org/wiki/Rotation_matrix#In_two_dimensions
sf::Vector2f rotateVec(sf::Vector2f vec, float value) {
    return sf::Vector2f(
        vec.x * std::cos(value) - vec.y * std::sin(value),
        vec.x * std::sin(value) + vec.y * std::cos(value)
    );
}

int main() {

    // if the map is not correct, we can have segmentation faults. So check it.
    if (!mapCheck()) {
        fprintf(stderr, "Map is invalid!\n");
        return EXIT_FAILURE;
    }

    sf::Font font;
    if (!font.loadFromFile("data/font/opensans.ttf")) {
        fprintf(stderr, "Cannot open font!\n");
        return EXIT_FAILURE;
    }

    sf::Texture texture;
    if (!texture.loadFromFile("data/texture/walls.png")) {
        fprintf(stderr, "Cannot open texture!\n");
        return EXIT_FAILURE;
    }

    // render state that uses the texture
    sf::RenderStates state(&texture);

    // player
    sf::Vector2f position(2.5f, 2.0f); // coordinates in worldMap
    sf::Vector2f direction(0.0f, 1.0f); // direction, relative to (0,0)
    sf::Vector2f plane(-0.66f, 0.0f); // 2d raycaster version of the camera plane,
    // must be perpendicular to rotation
    float size_f = 0.375f; // dimensions of player collision box, in tiles
    float moveSpeed = 5.0f; // player movement speed in tiles per second
    float rotateSpeed = 3.0f; // player rotation speed in radians per second

    sf::Vector2f size(size_f, size_f); // player collision box width and height, derived from size_f

    // create window
    sf::RenderWindow window(sf::VideoMode(screenWidth + 1, screenHeight), "Adventure 3D");
    window.setSize(sf::Vector2u(screenWidth, screenHeight)); // why add +1 and then set the size correctly?
    // Fixes some problem with the viewport. If you
    // don't do it, you'll see lots of gaps. Maybe
    // there's a better fix.

    bool hasFocus = true;

    // lines used to draw walls and floors on the screen
    sf::VertexArray lines(sf::Lines, 18 * screenWidth);

    sf::Text fpsText("", font, 50); // text object for FPS counter
    sf::Clock clock; // timer
    char frameInfoString[sizeof("FPS: *****.*, Frame time: ******")]; // string buffer for frame information

    float dt_counter = 0.0f; // delta time for multiple frames, for calculating FPS smoothly
    int frame_counter = 0; // counts frames for FPS calculation
    int64_t frame_time_micro = 0; // time needed to draw frames in microseconds

    while (window.isOpen()) {
        // get delta time
        float dt = clock.restart().asSeconds();

        // Update FPS, smoothed over time
        if (dt_counter >= fps_refresh_time) {
            float fps = (float)frame_counter / dt_counter;
            frame_time_micro /= frame_counter;
            snprintf(frameInfoString, sizeof(frameInfoString), "FPS: %3.1f, Frame time: %6ld", fps, frame_time_micro);
            fpsText.setString(frameInfoString);
            dt_counter = 0.0f;
            frame_counter = 0;
            frame_time_micro = 0;
        }
        dt_counter += dt;
        ++frame_counter;

        // handle SFML events
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::LostFocus:
                hasFocus = false;
                break;
            case sf::Event::GainedFocus:
                hasFocus = true;
                break;
            default:
                break;
            }
        }

        // handle keyboard input
        if (hasFocus) {
            using kb = sf::Keyboard;

            // moving forward or backwards (1.0 or -1.0)
            float moveForward = 0.0f;

            // get input
            if (kb::isKeyPressed(kb::Up)) {
                moveForward = 1.0f;
            }
            else if (kb::isKeyPressed(kb::Down)) {
                moveForward = -1.0f;
            }

            // handle movement
            if (moveForward != 0.0f) {
                sf::Vector2f moveVec = direction * moveSpeed * moveForward * dt;

                if (canMove(sf::Vector2f(position.x + moveVec.x, position.y), size)) {
                    position.x += moveVec.x;
                }
                if (canMove(sf::Vector2f(position.x, position.y + moveVec.y), size)) {
                    position.y += moveVec.y;
                }
            }

            // rotating rightwards or leftwards(1.0 or -1.0)
            float rotateDirection = 0.0f;

            // get input
            if (kb::isKeyPressed(kb::Left)) {
                rotateDirection = -1.0f;
            }
            else if (kb::isKeyPressed(kb::Right)) {
                rotateDirection = 1.0f;
            }

            // handle rotation
            if (rotateDirection != 0.0f) {
                float rotation = rotateSpeed * rotateDirection * dt;
                direction = rotateVec(direction, rotation);
                plane = rotateVec(plane, rotation);
            }
        }

        lines.resize(0);

        // loop through vertical screen lines, draw a line of wall for each
        for (int x = 0; x < screenWidth; ++x) {

            // ray to emit
            float cameraX = 2 * x / (float)screenWidth - 1.0f; // x in camera space (between -1 and +1)
            sf::Vector2f rayPos = position;
            sf::Vector2f rayDir = direction + plane * cameraX;

            // NOTE: with floats, division by zero gives you the "infinity" value. This code depends on this.

            // calculate distance traversed between each grid line for x and y based on direction
            sf::Vector2f deltaDist(
                sqrt(1.0f + (rayDir.y * rayDir.y) / (rayDir.x * rayDir.x)),
                sqrt(1.0f + (rayDir.x * rayDir.x) / (rayDir.y * rayDir.y))
            );

            sf::Vector2i mapPos(rayPos); // which box of the map we're in

            sf::Vector2i step; // what direction to step in (+1 or -1 for each dimension)
            sf::Vector2f sideDist; // distance from current position to next gridline, for x and y separately

            // calculate step and initial sideDist
            if (rayDir.x < 0.0f) {
                step.x = -1;
                sideDist.x = (rayPos.x - mapPos.x) * deltaDist.x;
            }
            else {
                step.x = 1;
                sideDist.x = (mapPos.x + 1.0f - rayPos.x) * deltaDist.x;
            }
            if (rayDir.y < 0.0f) {
                step.y = -1;
                sideDist.y = (rayPos.y - mapPos.y) * deltaDist.y;
            }
            else {
                step.y = 1;
                sideDist.y = (mapPos.y + 1.0f - rayPos.y) * deltaDist.y;
            }

            char tile = '.'; // tile type that got hit
            bool horizontal; // did we hit a horizontal side? Otherwise it's vertical

            float perpWallDist = 0.0f; // wall distance, projected on camera direction
            int wallHeight; // height of wall to draw on the screen at each distance
            int ceilingPixel = 0; // position of ceiling pixel on the screen
            int groundPixel = screenHeight; // position of ground pixel on the screen

            // colors for floor tiles
            sf::Color color1 = sf::Color::Green;
            sf::Color color2 = sf::Color::Cyan;

            // current floor color
            sf::Color color = ((mapPos.x % 2 == 0 && mapPos.y % 2 == 0) ||
                (mapPos.x % 2 == 1 && mapPos.y % 2 == 1)) ? color1 : color2;

            // cast the ray until we hit a wall, meanwhile draw floors
            while (tile == '.') {
                if (sideDist.x < sideDist.y) {
                    sideDist.x += deltaDist.x;
                    mapPos.x += step.x;
                    horizontal = true;
                    perpWallDist = (mapPos.x - rayPos.x + (1 - step.x) / 2) / rayDir.x;
                }
                else {
                    sideDist.y += deltaDist.y;
                    mapPos.y += step.y;
                    horizontal = false;
                    perpWallDist = (mapPos.y - rayPos.y + (1 - step.y) / 2) / rayDir.y;
                }

                wallHeight = screenHeight / perpWallDist;

                // add floor

                lines.append(sf::Vertex(sf::Vector2f((float)x, (float)groundPixel), color, sf::Vector2f(385.0f, 129.0f)));
                groundPixel = int(wallHeight * cameraHeight + screenHeight * 0.5f);
                lines.append(sf::Vertex(sf::Vector2f((float)x, (float)groundPixel), color, sf::Vector2f(385.0f, 129.0f)));

                // add ceiling

                sf::Color color_c = color;
                color_c.r /= 2;
                color_c.g /= 2;
                color_c.b /= 2;

                lines.append(sf::Vertex(sf::Vector2f((float)x, (float)ceilingPixel), color_c, sf::Vector2f(385.0f, 129.0f)));
                ceilingPixel = int(-wallHeight * (1.0f - cameraHeight) + screenHeight * 0.5f);
                lines.append(sf::Vertex(sf::Vector2f((float)x, (float)ceilingPixel), color_c, sf::Vector2f(385.0f, 129.0f)));

                // change color and find tile type

                color = (color == color1) ? color2 : color1;

                tile = getTile(mapPos.x, mapPos.y);
            }

            // calculate lowest and highest pixel to fill in current line
            int drawStart = ceilingPixel;
            int drawEnd = groundPixel;

            // get position of the wall texture in the full texture
            int wallTextureNum = (int)wallTypes.find(tile)->second;
            sf::Vector2i texture_coords(
                wallTextureNum * texture_wall_size % texture_size,
                wallTextureNum * texture_wall_size / texture_size * texture_wall_size
            );

            // calculate where the wall was hit
            float wall_x;
            if (horizontal) {
                wall_x = rayPos.y + perpWallDist * rayDir.y;
            }
            else {
                wall_x = rayPos.x + perpWallDist * rayDir.x;
            }
            wall_x -= floor(wall_x);

            // get x coordinate on the wall texture
            int tex_x = int(wall_x * float(texture_wall_size));

            // flip texture if we see it on the other side of us, this prevents a mirrored effect for the texture
            if ((horizontal && rayDir.x <= 0) || (!horizontal && rayDir.y >= 0)) {
                tex_x = texture_wall_size - tex_x - 1;
            }

            texture_coords.x += tex_x;

            // illusion of shadows by making horizontal walls darker
            color = sf::Color::White;
            if (horizontal) {
                color.r /= 2;
                color.g /= 2;
                color.b /= 2;
            }

            // add line to vertex buffer
            lines.append(sf::Vertex(
                sf::Vector2f((float)x, (float)drawStart),
                color,
                sf::Vector2f((float)texture_coords.x, (float)texture_coords.y + 1)
            ));
            lines.append(sf::Vertex(
                sf::Vector2f((float)x, (float)drawEnd),
                color,
                sf::Vector2f((float)texture_coords.x, (float)(texture_coords.y + texture_wall_size - 1))
            ));
        }

        window.clear();
        window.draw(lines, state);
        window.draw(fpsText);
        frame_time_micro += clock.getElapsedTime().asMicroseconds();
        window.display();
    }

    return EXIT_SUCCESS;
}