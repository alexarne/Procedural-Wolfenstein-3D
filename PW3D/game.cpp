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

int Game::start(sf::RenderWindow* win, Settings* set) {
    window = win;
    settings = set;
    running = true;
    settingsVisible = false;
    prevSettingsVisible = false;
    config = settings->getConfig();
    Player tempPlayer(11.5, 12);
    player = &tempPlayer;
    Map tempMap(window, player, settings);
    map = &tempMap;
    worldMap = map->getMap();

    window->setMouseCursorVisible(false);
    int value = loop();
    window->setMouseCursorVisible(true);

	return value;
}

int loop() {
    sf::Vector2u windowSize = window->getSize();
    sf::Mouse::setPosition(sf::Vector2i(windowSize.x / 2, windowSize.y / 2), *window);
    
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

    // Cant use sf::Keyboard::isKeyPressed() because that triggers antivirus software
    bool W_PRESSED = false;
    bool A_PRESSED = false;
    bool S_PRESSED = false;
    bool D_PRESSED = false;

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
            if (W_PRESSED) player->moveForward(config->movementSpeed * elapsedTime);
            if (A_PRESSED) player->moveLeft(config->movementSpeed * elapsedTime);
            if (S_PRESSED) player->moveBackward(config->movementSpeed * elapsedTime);
            if (D_PRESSED) player->moveRight(config->movementSpeed * elapsedTime);
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
                if (event.key.code == sf::Keyboard::W) W_PRESSED = true;
                if (event.key.code == sf::Keyboard::A) A_PRESSED = true;
                if (event.key.code == sf::Keyboard::S) S_PRESSED = true;
                if (event.key.code == sf::Keyboard::D) D_PRESSED = true;
                if (event.key.code == sf::Keyboard::Escape) settings->toggle();
                break;
            case sf::Event::KeyReleased:
                if (event.key.code == sf::Keyboard::W) W_PRESSED = false;
                if (event.key.code == sf::Keyboard::A) A_PRESSED = false;
                if (event.key.code == sf::Keyboard::S) S_PRESSED = false;
                if (event.key.code == sf::Keyboard::D) D_PRESSED = false;
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
                    W_PRESSED = false;
                    A_PRESSED = false;
                    S_PRESSED = false;
                    D_PRESSED = false;
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
    int h = (h_pre + config->quality - 1) / config->quality;
    int heightOrigin = player->getHeightOrigin(h);
    sf::Uint8* screenPixels = new sf::Uint8[w * h * 4];

    for (int x = 0; x < w; x++) {
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
        int side; // was a NS or a EW wall hit?

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

        //perform DDA
        while (!hit) {
            //jump to next map square, either in x-direction, or in y-direction
            if (sideDist.x < sideDist.y)
            {
                sideDist.x += deltaDist.x;
                mapPos.x += stepX;
                side = 0;
            }
            else {
                sideDist.y += deltaDist.y;
                mapPos.y += stepY;
                side = 1;
            }
            //Check if ray has hit a wall
            if (worldMap[mapPos.y][mapPos.x] > 0) hit = true;
        }

        //Calculate distance projected on camera direction (Euclidean distance would give fisheye effect!)
        if (side == 0) perpWallDist = (sideDist.x - deltaDist.x);
        else           perpWallDist = (sideDist.y - deltaDist.y);

        //Calculate height of line to draw on screen
        int lineHeight = perpWallDist == 0 ? h : (int)(h / perpWallDist);

        //calculate lowest and highest pixel to fill in current stripe
        int drawStart = heightOrigin - lineHeight / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = heightOrigin + lineHeight / 2;
        if (drawEnd >= h) drawEnd = h - 1;

        for (int i = 0; i < drawStart; i++) setPixel(screenPixels, x, i, w, h, sf::Color::Black);
        for (int i = drawEnd; i < h; i++) setPixel(screenPixels, x, i, w, h, sf::Color::Black);

        //calculate value of wallX
        double wallX; //where exactly the wall was hit
        if (side == 0) wallX = pos.y + perpWallDist * rayDir.y;
        else           wallX = pos.x + perpWallDist * rayDir.x;
        wallX -= floor((wallX));

        //x coordinate on the texture
        int texX = int(wallX * double(TEXTURE_WIDTH));
        if (side == 0 && rayDir.x > 0) texX = TEXTURE_WIDTH - texX - 1;
        if (side == 1 && rayDir.y < 0) texX = TEXTURE_WIDTH - texX - 1;

        // How much to increase the texture coordinate per screen pixel
        double step = 1.0 * TEXTURE_HEIGHT / lineHeight;
        // Starting texture coordinate
        double texPos = (drawStart - heightOrigin + lineHeight / 2) * step;
        for (int y = drawStart; y <= drawEnd; y++) {
            // Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
            int texY = (int)texPos % TEXTURE_HEIGHT;
            texPos += step;
            sf::Color color = Assets::getTextureColor(worldMap[mapPos.y][mapPos.x], texX, texY);
            //make color darker for y-sides
            if (side == 1) color = sf::Color(color.r * 0.5, color.g * 0.5, color.b * 0.5);
            setPixel(screenPixels, x, y, w, h, color);
        }
    }

    drawBuffer(screenPixels, w, h);
    delete[] screenPixels;
}

void setPixel(sf::Uint8* screen, int x, int y, int w, int h, sf::Color color) {
    if (x < 0 || x >= w || y < 0 || y >= h) return;
    int i = (y * w + x) * 4;
    screen[i]     = color.r;
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