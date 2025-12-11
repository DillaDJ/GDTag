#pragma once

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <godot_cpp/classes/panel_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/confirmation_dialog.hpp>
#include <godot_cpp/classes/tree.hpp>

using namespace godot;

enum TagEditorMode {
    READ, SELECT, SELECT_MULTI
};

class TagEditor : public Control {
    GDCLASS(TagEditor, Control);

public:
    TagEditor();
    ~TagEditor() { }

    void _enter_tree() override;
        
    void set_mode(TagEditorMode p_mode);

    void refresh_tags();

protected:
    static void _bind_methods();
	
private:
    void populate_tags();
    void populate_children_recursive(TreeItem *parent_item, class TagTreeItem *parent_tag);
    void toggle_database_signal_connections(bool on);
    
    TreeItem *create_tree_item(TreeItem *parent = nullptr);

	void add_tag();
    void toggle_select_tag();
    void unselect_recursive(TreeItem *item);

	void prompt_delete_tag();
	void delete_selected_tag();

    void prompt_selected_tag_name();
    void prompt_selected_tag_rename();
    void update_tag_database();

    void add_new_tag(TypedArray<StringName> selected_tag_path, StringName tag_name);
    void rename_selected_tag(TypedArray<StringName> selected_tag_path, StringName new_name);

	void empty_clicked(Vector2 position, MouseButton button);


    TypedArray<StringName> get_selected_tag_path();
    TypedArray<StringName> get_edited_tag_path();
    TypedArray<StringName> get_tag_path(TreeItem *item);
    
    TagEditorMode mode;
    
    class TagDatabase *database;

    PanelContainer *main_container;
    VBoxContainer *main_vbox;

    PanelContainer *header_container;
    HBoxContainer *header_hbox;

    Button *add_tag_btn;
    Button *del_tag_btn;

    PanelContainer *tree_container;
    Tree *tag_tree;

    TreeItem *root;

    ConfirmationDialog *delete_confirm;

    StringName old_tag_name;
};