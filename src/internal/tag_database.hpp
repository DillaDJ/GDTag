#pragma once

#include <godot_cpp/core/object.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <godot_cpp/variant/dictionary.hpp>

using namespace godot;

class TagDatabase : public Object {
    GDCLASS(TagDatabase, Object);
    
public:
    static TagDatabase *get_singleton();

    TagDatabase();
    ~TagDatabase();
    
    void initialize();
    static void uninitialize();

    class TagTreeItem *get_tag(TypedArray<StringName> path_arr);
    class Array get_tags() { return nodes.values(); }
    
    void add_tag(StringName name, TagTreeItem *parent = nullptr);
    void remove_tag(TagTreeItem *tag);
    void rename_tag(TagTreeItem *tag, StringName new_name);

    void save() { write_to_file(); }
    
protected:
    static void _bind_methods();
    
private:
    void remove_tag_recursive(TagTreeItem *tag);

    void read_from_file();
    void load_tags(Array loaded_tags);

    void write_to_file();

    static TagDatabase *singleton;

    Dictionary nodes; // StringName, TagTreeItem
};