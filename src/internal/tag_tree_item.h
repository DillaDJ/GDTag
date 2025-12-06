#pragma once

#include <godot_cpp/core/object.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/dictionary.hpp>

using namespace godot;

class TagTreeItem : public Object {
    GDCLASS(TagTreeItem, Object);
    
public:
    TagTreeItem();
    ~TagTreeItem();

    void add_child(StringName name);
    TagTreeItem *get_child(StringName name);

    StringName get_name() { return name; }
    void set_name(StringName p_name) { name = p_name; }

protected:
    static void _bind_methods();

private:
    TagTreeItem *parent;

    Dictionary children;

    StringName name;
};