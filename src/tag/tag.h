#pragma once

#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class Tag : public Resource {
    GDCLASS(Tag, Resource);
    
protected:
    static void _bind_methods();

private:
    class StringName linkedTagPath;
};