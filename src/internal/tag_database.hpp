#pragma once

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>

using namespace godot;

class TagDatabase : public Object {
    GDCLASS(TagDatabase, Object);
    
public:
    static TagDatabase *get_singleton() { return singleton; }

    void initialize();

    TagTreeItem *get_tag(TypedArray<StringName> path);
    
    void add_tag(StringName name, TagTreeItem* parent = nullptr);
    void rename_tag(TagTreeItem tag, StringName new_name);

protected:
    static void _bind_methods();

private:
    static TagDatabase *singleton;

    TypedDictionary<StringName, TagTreeItem> nodes;
};