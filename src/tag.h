#pragma once

#include <godot_cpp/classes/object.hpp>

using namespace godot;

class Tag : public Object {
    GDCLASS(Tag, Object);
    
protected:
    static void _bind_methods();
};