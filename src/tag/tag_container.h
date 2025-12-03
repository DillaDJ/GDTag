#pragma once

#include <godot_cpp/classes/object.hpp>

using namespace godot;

class TagContainer : public Object {
    GDCLASS(TagContainer, Object);
    
protected:
    static void _bind_methods();
};