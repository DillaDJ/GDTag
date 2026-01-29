#pragma once

#include <godot_cpp/classes/tree.hpp>
#include <godot_cpp/classes/label.hpp>

using namespace godot;

enum TagTreeMode {
    READ, SELECT, SELECT_MULTI
};

class TagEditorTree : public Tree {
    GDCLASS(TagEditorTree, Tree);

public:
    TagEditorTree();

    void reset();

    void set_mode(TagTreeMode p_mode);
    TagTreeMode get_mode() { return mode; }

    TreeItem *create_tree_item(TreeItem *parent = nullptr);
    
    void check_tag(TypedArray<StringName> path_arr);
    void uncheck_tag(TypedArray<StringName> path_arr);
    void uncheck_all_tags();
    
    void push_last_config();
    void pop_last_config();

    TypedArray<StringName> get_tag_path_arr(TreeItem *item);
	TreeItem *get_item_from_path_arr(TypedArray<StringName> path_arr);

    Variant _get_drag_data(const Vector2 &p_at_position) override;
    bool _can_drop_data(const Vector2 &p_at_position, const Variant &p_data);
    void _drop_data(const Vector2 &p_at_position, const Variant &p_data);

    TreeItem *root() { return root_item; }

protected:
    static void _bind_methods();

private:
    TreeItem *root_item;

    Label* drag_label;
    
	TagTreeMode mode;

    class TagDatabase *database;
    
    Array last_config;

    void collapse_all();

    void populate_tags_recursive(TreeItem *parent_item, class InternalTag *parent_tag);
    
    void push_child_config(TreeItem *item, Array container);
    void pop_child_config(TreeItem *item, Array container);

    void uncheck_recursive(TreeItem *item);
};