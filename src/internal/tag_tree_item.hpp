#pragma once

#include <godot_cpp/core/object.hpp>
#include <godot_cpp/variant/dictionary.hpp>

using namespace godot;

class TagTreeItem : public Object {
    GDCLASS(TagTreeItem, Object);
    
public:
    TagTreeItem();
    ~TagTreeItem();
    
    void clear() { children.clear(); }

    TagTreeItem *get_parent() { return parent; }
    void set_parent(TagTreeItem *p_parent) { parent = p_parent; }

    void add_child(StringName name);
    void add_child(TagTreeItem *tag);
    
    TagTreeItem *get_child(StringName name);
    Array get_children() { return children.values(); }

    void remove_child(StringName name);

    StringName get_name() { return name; }
    void set_name(StringName p_name) { name = p_name; }

    TypedArray<StringName> get_path_arr();
    StringName get_path();

    void populate_children(Array children);
    Array get_children_names_recursive();

protected:
    static void _bind_methods();

private:
    TagTreeItem *parent;

    Dictionary children;

    StringName name;
};