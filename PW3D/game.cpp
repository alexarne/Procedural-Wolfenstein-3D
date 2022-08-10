#include "game.h"

sf::RenderWindow* window;   // Pointer to the window
Settings* settings;         // Pointer to the settings object
Configuration* config;      // Pointer to the values/configuration to be used

bool running;               // If the game is running
bool settingsVisible;       // If settings are visible
bool prevSettingsVisible;   // If settings were visible last game loop

Map* map;
int** worldMap;
Player* player;

int game::start(sf::RenderWindow* win, Settings* set) {
    window = win;
    settings = set;
    running = true;
    settingsVisible = false;
    prevSettingsVisible = false;
    config = settings->getConfig();
    Player tempPlayer(12, 12);
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
    sf::Uint64 acc = 0;
    int acc_counter = 0;

    sf::Text fpsCounter;
    fpsCounter.setFillColor(sf::Color::White);
    fpsCounter.setCharacterSize(30);
    sf::Font f;
    f.loadFromMemory(&font, font_len);
    fpsCounter.setFont(f);

	while (running) {
        acc += clock.getElapsedTime().asMicroseconds();
        if (++acc_counter > 10) {
            fpsCounter.setString("FPS: " + std::to_string(10 * 1000000 / acc));
            acc_counter = 0;
            acc = 0;
        }
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
            case sf::Event::MouseButtonPressed:
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
                break;
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
            }
            else {
                if (event.type == sf::Event::KeyPressed)
                    if (event.key.code == sf::Keyboard::M) map->toggleView();
            }
        }
	}
	return 0;
}

void drawScreen(int w, int h) {
    sf::Uint8* screenPixels = new sf::Uint8[w * h * 4];
    sf::Texture screenTexture;
    screenTexture.create(w, h);
    screenTexture.setSmooth(true);
    sf::Sprite screenSprite(screenTexture);

    for (int x = 0; x < w; x++) {
        // Fill black
        //for (int y = 0; y < h; y++) setPixel(screenPixels, x, y, w, sf::Color::Black);

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

        int hit = 0; // was there a wall hit?
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
        while (hit == 0) {
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
            if (worldMap[mapPos.y][mapPos.x] > 0) hit = 1;
        }

        //Calculate distance projected on camera direction (Euclidean distance would give fisheye effect!)
        if (side == 0) perpWallDist = (sideDist.x - deltaDist.x);
        else           perpWallDist = (sideDist.y - deltaDist.y);

        //Calculate height of line to draw on screen
        int lineHeight = (int)(h / perpWallDist);

        //calculate lowest and highest pixel to fill in current stripe
        int drawStart = h / 2 - lineHeight / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = h / 2 + lineHeight / 2;
        if (drawEnd >= h) drawEnd = h - 1;

        //choose wall color
        sf::Color color;
        switch (worldMap[mapPos.y][mapPos.x]) {
        case 1:
            color = sf::Color::Blue;
            break;
        case 2:
            color = sf::Color::Red;
            break;
        case 3:
            color = sf::Color::Green;
            break;
        case 4:
            color = sf::Color::Cyan;
            break;
        case 5:
            color = sf::Color::Magenta;
            break;
        default: color = sf::Color::Yellow; break;  // yellow
        }

        //give x and y sides different brightness
        if (side == 1) { color = sf::Color(color.r * 0.5, color.g * 0.5, color.b * 0.5); }

        //draw the pixels of the stripe as a vertical line
        for (int y = drawStart; y <= drawEnd; y++) {
            setPixel(screenPixels, x, y, w, color);
        }
    }

    screenTexture.update(screenPixels);
    window->draw(screenSprite);
    delete[] screenPixels;
}

void setPixel(sf::Uint8* screen, int x, int y, int w, sf::Color color) {
    int i = (y * w + x) * 4;
    screen[i]     = color.r;
    screen[i + 1] = color.g;
    screen[i + 2] = color.b;
    screen[i + 3] = color.a;
}