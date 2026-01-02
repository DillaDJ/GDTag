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

    class InternalTag *get_tag(TypedArray<StringName> path_arr);
    class Array get_tags() { return nodes.values(); }
    
    InternalTag *add_tag(StringName name, InternalTag *parent = nullptr, bool recalculate = false);
    void remove_tag(InternalTag *tag);
    void rename_tag(InternalTag *tag, StringName new_name);

    void save() { write_to_file(); }
    
protected:
    static void _bind_methods();
    
private:
    static TagDatabase *singleton;
    
    int next_id = 0;

    Dictionary nodes; // StringName, InternalTag
    Dictionary id_map; // int, InternalTag

    int get_next_id(InternalTag *tag);

    void remove_tag_recursive(InternalTag *tag);

    void read_from_file();
    void load_tags_recursive(Array loaded_tags, InternalTag *parent = nullptr);
    
    void write_to_file();
    Array get_children_recursive(InternalTag *tag = nullptr);

    StringName get_file_path();
};