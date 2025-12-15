#pragma once

#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class TagContainer : public Resource {
    GDCLASS(TagContainer, Resource);
    
public:
    TagContainer();

    void add_tag_path(StringName path);
    void remove_tag_path(StringName path);

    TypedArray<StringName> get_tag_paths() { return tag_paths.keys(); }

    int size() { return tag_paths.size(); }
    
    void set_tag_paths_map(Dictionary paths) { tag_paths = paths; }
    Dictionary get_tag_paths_map() { return tag_paths; }

protected:
    static void _bind_methods();

private:
    class Dictionary tag_paths; // StringName, true
};