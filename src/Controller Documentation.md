# SDL Controller guide. For future reference, not to be taken seriously

```
std::cout << (bool) SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_A) << "\n";
bool isAPressed = SDL_GameControllerGetButton(gameController, SDL_CONTROLLER_BUTTON_A);
if (isAPressed) {
    // Handle 'A' button press
}
```

You can replace SDL_CONTROLLER_BUTTON_A with any of the following constants to check for other buttons on the game controller:

```
SDL_CONTROLLER_BUTTON_B
SDL_CONTROLLER_BUTTON_X
SDL_CONTROLLER_BUTTON_Y
SDL_CONTROLLER_BUTTON_BACK
SDL_CONTROLLER_BUTTON_GUIDE
SDL_CONTROLLER_BUTTON_START
SDL_CONTROLLER_BUTTON_LEFTSTICK
SDL_CONTROLLER_BUTTON_RIGHTSTICK
SDL_CONTROLLER_BUTTON_LEFTSHOULDER
SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
SDL_CONTROLLER_BUTTON_DPAD_UP
SDL_CONTROLLER_BUTTON_DPAD_DOWN
SDL_CONTROLLER_BUTTON_DPAD_LEFT
SDL_CONTROLLER_BUTTON_DPAD_RIGHT
```

```
SDL_CONTROLLER_BUTTON_LEFTSHOULDER: for the left shoulder button
SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: for the right shoulder button
SDL_CONTROLLER_BUTTON_LEFT_TRIGGER: for the left trigger
SDL_CONTROLLER_BUTTON_RIGHT_TRIGGER: for the right trigger
```

Taking Minecraft as an example, shoulder is the hotbar selection buttons, trigger is the action buttons.

```
Triangle button -> SDL_CONTROLLER_BUTTON_Y
Circle button -> SDL_CONTROLLER_BUTTON_B
Cross button -> SDL_CONTROLLER_BUTTON_A
Square button -> SDL_CONTROLLER_BUTTON_X

SELECT button -> SDL_CONTROLLER_BUTTON_BACK
START button -> SDL_CONTROLLER_BUTTON_START
PS button -> SDL_CONTROLLER_BUTTON_GUIDE
```