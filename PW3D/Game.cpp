#include "Game.h"

sf::RenderWindow* window;   // Pointer to the window
Settings* settings;         // Pointer to the settings object
Configuration* config;      // Pointer to the values/configuration to be used

bool running;               // If the game is running
bool settingsVisible;       // If settings are visible
bool prevSettingsVisible;   // If settings were visible last game loop

Map* map;
int** worldMap;
Player* player;

sf::VertexArray pixels(sf::Lines, 0);
sf::RenderStates* state;

int Game::start(sf::RenderWindow* win, Settings* set) {
    window = win;
    settings = set;
    running = true;
    settingsVisible = false;
    prevSettingsVisible = false;
    config = settings->getConfig();
    Player tempPlayer(11.5, 12);
    player = &tempPlayer;
    Map tempMap(window, player, config);
    map = &tempMap;
    worldMap = map->getMap();

    sf::Texture texture;
    texture.loadFromMemory(textures_file, textures_file_len);
    sf::RenderStates st(&texture);
    state = &st;

    window->setMouseCursorVisible(false);
    int value = loop();
    window->setMouseCursorVisible(true);

	return value;
}

int loop() {
    sf::Vector2u windowSize = window->getSize();
    sf::Mouse::setPosition(sf::Vector2i(windowSize.x / 2, windowSize.y / 2), *window);
    window->setView(sf::View(sf::FloatRect(-(windowSize.x * 0.5), -(windowSize.y * 0.5), windowSize.x * 2, windowSize.y * 2)));
    sf::Clock clock;
    float elapsedTime;
    float acc = 0;
    int frame_counter = 0;

    sf::Text fpsCounter;
    fpsCounter.setFillColor(sf::Color::White);
    fpsCounter.setCharacterSize(30);
    fpsCounter.setString("FPS: ");
    sf::Font f;
    f.loadFromMemory(&font, font_len);
    fpsCounter.setFont(f);

	while (running) {
        acc += clock.getElapsedTime().asSeconds();
        if (acc > 0.2) {
            fpsCounter.setString("FPS: " + std::to_string((int) (frame_counter / acc)));
            frame_counter = 0;
            acc = 0;
        }
        frame_counter++;
        elapsedTime = clock.restart().asSeconds();
		window->clear(sf::Color::Black);

        prevSettingsVisible = settingsVisible;
        settingsVisible = settings->visible();

        sf::Vector2i mouse = sf::Mouse::getPosition(*window);
        sf::Vector2u windowSize = window->getSize();
        // Get mouse movement information
        if (prevSettingsVisible != settingsVisible) {
            window->setMouseCursorVisible(settingsVisible);
            if (!settingsVisible) sf::Mouse::setPosition(sf::Vector2i(windowSize.x / 2, windowSize.y / 2), *window);
        }
        else {
            if (!settingsVisible) {
                player->turnRight(config->sensitivity * (mouse.x - (float)windowSize.x / 2));
                player->lookUp(config->sensitivity * ((float)windowSize.y / 2 - mouse.y));
                sf::Mouse::setPosition(sf::Vector2i(windowSize.x / 2, windowSize.y / 2), *window);
            }
        }
        //printf("move: %f sens: %f fov: %f vis: %f useVis: %s res: %i\n", config->movementSpeed, config->sensitivity, config->fov, config->visibilityDepth, config->useVisibility ? "true" : "false", config->res);
        
        if (!settingsVisible) {
            // Get player movement information
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) player->moveForward(config->movementSpeed * elapsedTime);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) player->moveLeft(config->movementSpeed * elapsedTime);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) player->moveBackward(config->movementSpeed * elapsedTime);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) player->moveRight(config->movementSpeed * elapsedTime);
        }

        drawScreen(windowSize.x, windowSize.y);

        map->draw();
        settings->draw(mouse);

        fpsCounter.setScale(windowSize.y / 720.0, windowSize.y / 720.0);
        fpsCounter.setOrigin(fpsCounter.getLocalBounds().left, fpsCounter.getLocalBounds().top);
        fpsCounter.setPosition(10, windowSize.y - fpsCounter.getLocalBounds().height * windowSize.y / 720.0 - 10);
        window->draw(fpsCounter);

        window->display();
        
        sf::Event event;
        while (window->pollEvent(event)) {
            // Universal events (Regardless of settings visibility)
            sf::Vector2i click(event.mouseButton.x, event.mouseButton.y);
            switch (event.type) {
            case sf::Event::Closed:
                window->close();
                return 0;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) settings->toggle();
                break;
            }
            
            // Specific events (depending on settings visibility)
            if (settingsVisible) {
                settings->handleEvent(event);
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (settings->isInsideLeave(click)) {
                            settings->toggle();
                            return 0;
                        }
                        if (settings->isInsideRestart(click)) {
                            settings->toggle();
                            return 1;
                        }
                    }
                }
            }
            else {
                if (event.type == sf::Event::LostFocus) {
                    settings->toggle();
                }
                if (event.type == sf::Event::KeyPressed)
                    if (event.key.code == sf::Keyboard::M) map->toggleView();
            }
        }
	}
	return 0;
}

void drawScreen(int w_pre, int h_pre) {
    int w = (w_pre + config->quality - 1) / config->quality;
    //int h = (h_pre + config->quality - 1) / config->quality;
    int h = h_pre;
    int heightOrigin = player->getHeightOrigin(h);
    pixels.resize(0);

    // FLOOR & CEILING CASTING
    /*
    sf::Uint8* screenPixels = new sf::Uint8[w * h * 4];
    for (int y = 0; y < h; y++) {
        sf::Vector2f dir = player->getDir();
        sf::Vector2f plane = player->getPlane(config->fov);
        sf::Vector2f pos = player->getPos();

        // rayDir for leftmost ray (x = 0) and rightmost ray (x = w)
        sf::Vector2f rayDir0(dir.x - plane.x, dir.y - plane.y);
        sf::Vector2f rayDir1(dir.x + plane.x, dir.y + plane.y);

        // Current y position compared to the center of the screen (the horizon)
        int p = y - heightOrigin;
        if (y < heightOrigin) p *= -1;

        // Vertical position of the camera.
        float posZ = 0.5 * h;

        // Horizontal distance from the camera to the floor for the current row.
        // 0.5 is the z position exactly in the middle between floor and ceiling.
        float rowDistance = posZ / p;

        // calculate the real world step vector we have to add for each x (parallel to camera plane)
        // adding step by step avoids multiplications with a weight in the inner loop
        float floorStepX = rowDistance * (rayDir1.x - rayDir0.x) / w;
        float floorStepY = rowDistance * (rayDir1.y - rayDir0.y) / w;

        // real world coordinates of the leftmost column. This will be updated as we step to the right.
        float floorX = pos.x + rowDistance * rayDir0.x;
        float floorY = pos.y + rowDistance * rayDir0.y;

        for (int x = 0; x < w; ++x) {
            // the cell coord is simply got from the integer parts of floorX and floorY
            int cellX = (int)(floorX);
            int cellY = (int)(floorY);

            // get the texture coordinate from the fractional part
            int tx = (int)(TEXTURE_WIDTH * (floorX - cellX)) % TEXTURE_WIDTH;
            int ty = (int)(TEXTURE_HEIGHT * (floorY - cellY)) % TEXTURE_HEIGHT;

            floorX += floorStepX;
            floorY += floorStepY;

            // choose texture and draw the pixel
            int floorTexture = 2;
            int ceilingTexture = 8;
            sf::Color color = Assets::getTextureColor(y < heightOrigin ? ceilingTexture : floorTexture, tx, ty, y > heightOrigin);
            setPixel(screenPixels, x, y, w, h, color);
        }
    }
    */

    // WALL CASTING
    for (int x = 0; x < w; x++) {
        const float cameraHeight = 0.5f; // height of player camera(1.0 is ceiling, 0.0 is floor)

        double cameraX = 2 * x / float(w) - 1;      // pixel in x of [-1, 1)
        sf::Vector2f plane = player->getPlane(config->fov);
        sf::Vector2f dir = player->getDir();
        sf::Vector2f rayDir(dir.x + plane.x * cameraX, dir.y + plane.y * cameraX);

        // which box of the map we're in
        sf::Vector2f pos = player->getPos();
        sf::Vector2i mapPos(int(pos.x), int(pos.y));

        // length of ray from current position to next x or y-side
        sf::Vector2f sideDist(0, 0);

        // length of ray from one x or y-side to next x or y-side
        sf::Vector2f deltaDist(
            (rayDir.x == 0) ? 1e30 : std::abs(1 / rayDir.x), 
            (rayDir.y == 0) ? 1e30 : std::abs(1 / rayDir.y)
        );
        double perpWallDist;

        // what direction to step in x or y-direction (either +1 or -1)
        int stepX;
        int stepY;

        bool hit = false; // was there a wall hit?
        bool shadowed; // was a NS or a EW wall hit?

        //calculate step and initial sideDist
        if (rayDir.x < 0) {
            stepX = -1;
            sideDist.x = (pos.x - mapPos.x) * deltaDist.x;
        }
        else {
            stepX = 1;
            sideDist.x = (mapPos.x + 1.0 - pos.x) * deltaDist.x;
        }
        if (rayDir.y < 0) {
            stepY = -1;
            sideDist.y = (pos.y - mapPos.y) * deltaDist.y;
        }
        else {
            stepY = 1;
            sideDist.y = (mapPos.y + 1.0 - pos.y) * deltaDist.y;
        }

        int lineHeight; // height of wall to draw on the screen at each distance
        int ceilingPixel = 0; // position of ceiling pixel on the screen
        int floorPixel = h; // position of ground pixel on the screen
        int ceilingTexture = 12;
        int floorTexture = 39;
        sf::Color currIntensity = sf::Color::White;
        sf::Color prevIntensity = sf::Color::White;
        sf::Vector2i currPoint;
        sf::Vector2i prevPoint(pos.x, pos.y);

        //perform DDA
        while (!hit) {
            // Jump to next map square, either in x-direction, or in y-direction
            if (sideDist.x < sideDist.y)
            {
                sideDist.x += deltaDist.x;
                mapPos.x += stepX;
                shadowed = false;
            }
            else {
                sideDist.y += deltaDist.y;
                mapPos.y += stepY;
                shadowed = true;
            }

            //Calculate distance projected on camera direction (Euclidean distance would give fisheye effect!)
            if (!shadowed) perpWallDist = (sideDist.x - deltaDist.x);
            else           perpWallDist = (sideDist.y - deltaDist.y);

            //Calculate height of line to draw on screen
            lineHeight = (int) (h / perpWallDist);

            if (config->useVisibility) {
                prevIntensity = currIntensity;
                float intensity = 1 - perpWallDist / config->visibilityDepth;
                if (intensity > 1) intensity = 1;
                if (intensity < 0) intensity = 0;
                currIntensity.a = intensity * 255;
            }

            //calculate value of wallX
            double wallX = pos.x + perpWallDist * rayDir.x;
            double wallY = pos.y + perpWallDist * rayDir.y;
            wallX -= floor((wallX));
            wallY -= floor((wallY));
            int texX = int(wallX * double(TEXTURE_WIDTH));
            int texY = int(wallY * double(TEXTURE_HEIGHT));
            currPoint = sf::Vector2i(texX, texY);
            sf::Color c = (mapPos.x + mapPos.y) % 2 == 0 ? sf::Color::Green : sf::Color::Cyan;
            // add floor
            pixels.append(sf::Vertex(sf::Vector2f((float)x, (float)floorPixel), c//,
                //Assets::getTextureCoords(floorTexture, prevPoint.x, prevPoint.y, false)
            ));
            floorPixel = int(heightOrigin + lineHeight * cameraHeight);
            pixels.append(sf::Vertex(sf::Vector2f((float)x, (float)floorPixel), c//, 
                //Assets::getTextureCoords(floorTexture, currPoint.x, currPoint.y, false)
            ));

            // add ceiling
            pixels.append(sf::Vertex(sf::Vector2f((float)x, (float)ceilingPixel), c//, 
                //Assets::getTextureCoords(ceilingTexture, prevPoint.x, prevPoint.y, true)
            ));
            ceilingPixel = int(heightOrigin - lineHeight * (1.0f - cameraHeight));
            pixels.append(sf::Vertex(sf::Vector2f((float)x, (float)ceilingPixel), c//, 
                //Assets::getTextureCoords(ceilingTexture, currPoint.x, currPoint.y, true)
            ));

            prevPoint = currPoint;
            if (prevPoint.x == 0) prevPoint.x = TEXTURE_WIDTH - 1;
            if (prevPoint.x == TEXTURE_WIDTH - 1) prevPoint.x = 0;
            if (prevPoint.y == 0) prevPoint.y = TEXTURE_HEIGHT - 1;
            if (prevPoint.y == TEXTURE_HEIGHT - 1) prevPoint.y = 0;

            // Check if ray has hit a wall
            if (worldMap[mapPos.y][mapPos.x] > 0) hit = true;
            //if (config->useVisibility && perpWallDist > config->visibilityDepth) break;
        }
        if (!hit) continue;

        //calculate lowest and highest pixel to fill in current stripe
        int drawStart = heightOrigin - lineHeight / 2 - 1;
        int drawEnd = heightOrigin + lineHeight / 2;

        //map->seenWall(pos.x, pos.y, perpWallDist * rayDir.x, perpWallDist * rayDir.y);

        //calculate value of wallX
        double wallX;   //where exactly the wall was hit
        double distX, distY;
        distY = perpWallDist * rayDir.y;
        distX = perpWallDist * rayDir.x;
        if (!shadowed) wallX = pos.y + distY;
        else           wallX = pos.x + distX;
        wallX -= floor((wallX));

        //x coordinate on the texture
        int texX = int(wallX * double(TEXTURE_WIDTH));
        bool invert = (shadowed && rayDir.y > 0) || (!shadowed && rayDir.x < 0);    // Top and right sides
        if (invert) texX = TEXTURE_WIDTH - 1 - texX;

        sf::Color color = sf::Color::White;
        if (config->useVisibility) {
            float sqDist = distX * distX + distY * distY;
            float intensity = 1 - sqDist / (config->visibilityDepth * config->visibilityDepth);
            if (intensity > 1) intensity = 1;
            if (intensity < 0) intensity = 0;
            color.a = intensity * 255;
        }
        for (int i = 0; i < config->quality; i++) {
            pixels.append(sf::Vertex(
                sf::Vector2f(config->quality * x + i, drawStart),
                color,
                Assets::getTextureCoords(worldMap[mapPos.y][mapPos.x], texX, 0, shadowed)
            ));
            pixels.append(sf::Vertex(
                sf::Vector2f(config->quality * x + i, drawEnd),
                color,
                Assets::getTextureCoords(worldMap[mapPos.y][mapPos.x], texX, TEXTURE_HEIGHT - 1, shadowed)
            ));
        }
    }
    window->draw(pixels, *state);
}

void setPixel(sf::Uint8* screen, int x, int y, int w, int h, sf::Color color) {
    if (x < 0 || x >= w || y < 0 || y >= h) return;
    int i = (y * w + x) * 4;
    screen[i] = color.r;
    screen[i + 1] = color.g;
    screen[i + 2] = color.b;
    screen[i + 3] = color.a;
}

void drawBuffer(sf::Uint8* buffer, int w, int h) {
    sf::Texture screenTexture;
    screenTexture.create(w, h);
    screenTexture.setSmooth(false);
    sf::Sprite screenSprite(screenTexture);
    screenTexture.update(buffer);
    screenSprite.setScale(config->quality, config->quality);
    window->draw(screenSprite);
}
