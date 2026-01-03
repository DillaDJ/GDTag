#pragma once

#include <godot_cpp/core/object.hpp>
#include <godot_cpp/variant/dictionary.hpp>

using namespace godot;

class InternalTag : public Object {
    GDCLASS(InternalTag, Object);
    
public:
    InternalTag();
    ~InternalTag() {};
    
    void clear() { children.clear(); }

    InternalTag *get_parent() { return parent; }
    void set_parent(InternalTag *p_parent) { parent = p_parent; }

    void add_child(InternalTag *tag);
    
    InternalTag *get_child(int id);
    InternalTag *get_child(StringName name);
    Array get_children() { return children.values(); }

    void remove_child(InternalTag *tag);
    
    int get_id() { return id; }
    void set_id(int value) { id = value; }

    StringName get_tag_name() { return tag_name; }
    void set_tag_name(StringName p_name) { tag_name = p_name; }

    Array get_child_ids() { return children.keys(); }

    TypedArray<StringName> get_path_arr();
    StringName get_path();

protected:
    static void _bind_methods() {};

private:
    int id;

    InternalTag *parent;

    Dictionary children; // ID, InternalTag

    StringName tag_name;
};