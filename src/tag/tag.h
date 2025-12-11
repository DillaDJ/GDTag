#pragma once

#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class Tag : public Resource {
    GDCLASS(Tag, Resource);
    
public:
    StringName get_linked_path() { return linked_tag_path; }
    class TagTreeItem *get_tag();
    
    void set_tag(class TagTreeItem *tag);

protected:
    static void _bind_methods();

private:
    class StringName linked_tag_path;
};