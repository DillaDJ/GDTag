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

    class InternalTag *get_tag(int id);
    class InternalTag *get_tag(TypedArray<StringName> path_arr);
    class Array get_nodes();
    
    InternalTag *add_tag(StringName name, InternalTag *parent = nullptr);
    void move_tag(InternalTag *moving, InternalTag *to, int positioning);
    void remove_tag(InternalTag *tag);
    void rename_tag(InternalTag *tag, StringName new_name);

    void save() { write_to_file(); }
    
    bool sort_id(Variant a, Variant b);
    bool sort_tag(InternalTag *a, InternalTag *b);

protected:
    static void _bind_methods();
    
private:
    static TagDatabase *singleton;
    
    int next_id = 0;

    Dictionary nodes; // int, InternalTag
    Dictionary id_map; // int, InternalTag
    Dictionary order_map; // int (id), int (order)

    int get_next_id(InternalTag *tag);

    InternalTag *get_node(StringName name);

    void remove_tag_recursive(InternalTag *tag);

    void read_from_file();
    void load_tags_recursive(Array loaded_tags, InternalTag *parent = nullptr);
    
    void write_to_file();
    Array get_children_recursive(InternalTag *tag = nullptr);

    StringName get_file_path();

	InternalTag *get_child_with_order(InternalTag *parent, int order);

	void recalculate_order(InternalTag *parent, InternalTag *around = nullptr);
    void recalculate_order_from_reposition(int on_tag_id, int with_tag_id, bool above);

};