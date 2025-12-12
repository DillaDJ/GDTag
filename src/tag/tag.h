#pragma once

#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class Tag : public Resource {
    GDCLASS(Tag, Resource);
    
public:    
    class TagTreeItem *get_tag();

    void set_tag_path(StringName path) { tag_path = path; }
    StringName get_tag_path() { return tag_path; }

protected:
    static void _bind_methods();

private:
    class StringName tag_path;
};