#ifndef BEG_INPUT_H
#define BEG_INPUT_H

#include <array>
#include <vector>

namespace BEG {

class Keyboard {
private:
    std::vector<bool> mMap{};
public:
    enum class Key {
        Space = 32,

        Apostrophe = 39,

        Comma = 44,
        Minus,
        Period,
        Slash,

        Key0,
        Key1,
        Key2,
        Key3,
        Key4,
        Key5,
        Key6,
        Key7,
        Key8,
        Key9,

        SemiColon = 59,
        
        Equal = 61,

        A = 65,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,

        LeftBracket,
        BackSlash,
        RightBracket,
        
        GraveAccent = 96,

        Escape = 256,
        Enter,
        Tab,
        Backspace,
        Insert,
        Delete,
        Right,
        Left,
        Down,
        Up,
        PageUp,
        PageDown,
        Home,
        End,

        CapsLock = 280,
        ScrollLock,
        NumLock,
        PrintScreen,
        Pause,

        F1 = 290,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        F13,
        F14,
        F15,
        F16,
        F17,
        F18,
        F19,
        F20,
        F21,
        F22,
        F23,
        F24,
        F25,

        Keypad0 = 320,
        Keypad1,
        Keypad2,
        Keypad3,
        Keypad4,
        Keypad5,
        Keypad6,
        Keypad7,
        Keypad8,
        Keypad9,
        KeypadDecimal,
        KeypadDivide,
        KeypadMultiply,
        KeypadSubtract,
        KeypadAdd,
        KeypadEnter,
        KeypadEqual,

        LeftShift = 340,
        LeftControl,
        LeftAlt,
        LeftSuper,
        RightShift,
        RightControl,
        RightAlt,
        RightSuper,
        Menu,

        Max
    };
    
    static int toGLFWKey(Key key);
    static Key fromGLFWKey(int key);

    Keyboard() : mMap(static_cast<size_t>(Key::Max), false) {}

    void setKeyDown(Key key);
    void setKeyUp(Key key);

    bool isDown(Key key) const;
    bool isUp(Key key) const;
};

class Mouse {
private:
    float mX{}, mY{};
    float mDX{}, mDY{}; /* change in mouse position */

    std::array<bool, 8> mMap;
public:
    enum class Button {
        Left = 0,
        Right,
        Middle,
        Button4,
        Button5,
        Button6,
        Button7,
        Button8,
    };

    static int toGLFWButton(Button button);
    static Button fromGLFWButton(int button);

    Mouse() : mX{}, mY{}, mMap{} {
        mMap.fill(false);
    }

    void setButtonDown(Button button);
    void setButtonUp(Button button);

    bool isDown(Button button) const;
    bool isUp(Button button) const;

    void setMousePosition(float x, float y);

    float x() const;
    float y() const;

    float changeInX() const;
    float changeInY() const;
};

}

#endif