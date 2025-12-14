#pragma once

#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class TagContainer : public Resource {
    GDCLASS(TagContainer, Resource);
    
public:    
    TypedArray<StringName> get_tag_paths() { return tag_paths; }

protected:
    static void _bind_methods();

private:
    class TypedArray<StringName> tag_paths;
};