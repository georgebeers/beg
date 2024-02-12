#ifndef BEG_GAME_H
#define BEG_GAME_H

#include <camera.h>
#include <ecs.h>
#include <input.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

namespace BEG {

class Game {
private:
    int mWindowWidth{}, mWindowHeight{};

    GLFWwindow* mWindow{};

    Keyboard mKeyboard{};
    Mouse mMouse{};

    float mPrevTime{}, mDeltaTime{};

    void framebufferSizeHandler(int width, int height);
public:
    enum class GameError {
        GLFWWindowError,
        GLADInitError
    };

    Scene scene{};
    Camera camera{};

    Game(const std::string& name, int width, int height);
    ~Game();

    Game() = default;
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    int windowWidth() const;
    int windowHeight() const;
    float aspectRatio() const;

    bool isKeyDown(Keyboard::Key key) const;
    bool isKeyUp(Keyboard::Key key) const;

    bool isButtonDown(Mouse::Button button) const;
    bool isButtonUp(Mouse::Button button) const;

    float mouseX() const;
    float mouseY() const;
    float changeInMouseX() const;
    float changeInMouseY() const;

    float deltaTime() const;

    void begin();
};

}

#endif