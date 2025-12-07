#pragma once

#include <godot_cpp/core/object.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <godot_cpp/variant/dictionary.hpp>

using namespace godot;

class TagDatabase : public Object {
    GDCLASS(TagDatabase, Object);
    
public:
    static TagDatabase *get_singleton();

    void initialize();

    class TagTreeItem *get_tag(TypedArray<StringName> path);
    
    void add_tag(StringName name, TagTreeItem *parent = nullptr);
    void remove_tag(TagTreeItem *tag);
    
    void rename_tag(TagTreeItem *tag, StringName new_name);

protected:
    static void _bind_methods();

private:
    static TagDatabase *singleton;

    Dictionary nodes; // StringName, TagTreeItem
};