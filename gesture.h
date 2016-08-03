#ifndef GESTURE_H
#define GESTURE_H

struct Gesture {
    enum GestureType {
        TYPE_ELLIPS,
        TYPE_RECTANGLE,
        TYPE_LINE
    };

    GestureType type;

};

#endif // GESTURE_H
