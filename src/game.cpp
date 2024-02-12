#include <game.h>

using namespace BEG;

void Game::framebufferSizeHandler(int width, int height) {
    mWindowWidth = width;
    mWindowHeight = height;

    glViewport(0, 0, mWindowWidth, mWindowHeight);
}

Game::Game(const std::string& name, int width, int height)
    : mWindowWidth{ width }, mWindowHeight{ height }, mWindow{ nullptr }, mKeyboard{}, mMouse{}, scene{}, camera{} {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    mWindow = glfwCreateWindow(mWindowWidth, mWindowHeight, name.c_str(), NULL, NULL);
    if (mWindow == NULL) {
        throw Game::GameError::GLFWWindowError;
    }
    glfwMakeContextCurrent(mWindow);

    /* disable cursor & enable raw mouse input */
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(mWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        throw Game::GameError::GLADInitError;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glViewport(0, 0, mWindowWidth, mWindowHeight);

    glfwSetWindowUserPointer(mWindow, this);

    glfwSetFramebufferSizeCallback(mWindow, [](GLFWwindow* window, int nWidth, int nHeight) {
        static_cast<Game*>(glfwGetWindowUserPointer(window))->framebufferSizeHandler(nWidth, nHeight);
    });

    glfwSetKeyCallback(mWindow, [](GLFWwindow* window, int key, int, int action, int) {
        if (action == GLFW_PRESS) {
            static_cast<Game*>(glfwGetWindowUserPointer(window))->mKeyboard.setKeyDown(Keyboard::fromGLFWKey(key));
        } else if (action == GLFW_RELEASE) {
            static_cast<Game*>(glfwGetWindowUserPointer(window))->mKeyboard.setKeyUp(Keyboard::fromGLFWKey(key));
        }
    });

    glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* window, int button, int action, int) {
        if (action == GLFW_PRESS) {
            static_cast<Game*>(glfwGetWindowUserPointer(window))->mMouse.setButtonDown(Mouse::fromGLFWButton(button));
        } else if (action == GLFW_RELEASE) {
            static_cast<Game*>(glfwGetWindowUserPointer(window))->mMouse.setButtonUp(Mouse::fromGLFWButton(button));
        }
    });

    glfwSetCursorPosCallback(mWindow, [](GLFWwindow* window, double x, double y) {
        static_cast<Game*>(glfwGetWindowUserPointer(window))->mMouse.setMousePosition(static_cast<float>(x), static_cast<float>(y));
    });
}

Game::~Game() {
    glfwTerminate();
}

int Game::windowWidth() const { return mWindowWidth; }
int Game::windowHeight() const { return mWindowHeight; }
float Game::aspectRatio() const {
    return static_cast<float>(mWindowWidth)/static_cast<float>(mWindowHeight);
}

bool Game::isKeyDown(Keyboard::Key key) const { return mKeyboard.isDown(key); }
bool Game::isKeyUp(Keyboard::Key key) const { return mKeyboard.isUp(key); }

bool Game::isButtonDown(Mouse::Button button) const { return mMouse.isDown(button); }
bool Game::isButtonUp(Mouse::Button button) const { return mMouse.isUp(button); }

float Game::mouseX() const { return mMouse.x(); }
float Game::mouseY() const { return mMouse.y(); }
float Game::changeInMouseX() const { return mMouse.changeInX(); }
float Game::changeInMouseY() const { return mMouse.changeInY(); }


float Game::deltaTime() const { return mDeltaTime; }

void Game::begin() {
    scene.setupSystems(*this);

    while (!glfwWindowShouldClose(mWindow)) {
        float currTime{ static_cast<float>(glfwGetTime()) };
        mDeltaTime = currTime - mPrevTime;
        mPrevTime = currTime;

        if(isKeyDown(Keyboard::Key::Escape))
            glfwSetWindowShouldClose(mWindow, true);

        scene.updateSystems(*this);

        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
}