#pragma once

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <godot_cpp/classes/panel_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/confirmation_dialog.hpp>
#include <godot_cpp/classes/tree.hpp>
#include <godot_cpp/classes/input_event_key.hpp>

using namespace godot;

enum TagEditorMode {
    READ, SELECT, SELECT_MULTI
};

class TagEditor : public Control {
    GDCLASS(TagEditor, Control);

public:
    TagEditor();
    ~TagEditor();

    void _enter_tree() override;
    void _exit_tree() override;
        
    void set_mode(TagEditorMode p_mode);

    void refresh_tags();

    void check_tag(TypedArray<StringName> path_arr);
    void uncheck_tag(TypedArray<StringName> path_arr);
    void uncheck_all_tags();

protected:
    static void _bind_methods();
	
private:
    void populate_tags_recursive(TreeItem *parent_item, class InternalTag *parent_tag);
    void toggle_database_signal_connections(bool on);
    
    TreeItem *create_tree_item(TreeItem *parent = nullptr);

	void add_tag();
    void toggle_select_tag();
    void uncheck_recursive(TreeItem *item);

	void prompt_delete_tag();
	void delete_selected_tag();

    void prompt_selected_tag_name();
    void prompt_selected_tag_rename();
    void update_tag_database();

    void add_new_tag(TypedArray<StringName> selected_tag_path_arr, StringName tag_name);
    void rename_selected_tag(TypedArray<StringName> selected_tag_path_arr, StringName new_name);

	void empty_clicked(Vector2 position, MouseButton button);

    bool get_line_edit(Node *parent);

    void remove_from_escape(bool on, LineEdit *line);

    void remove_unnamed();

    TypedArray<StringName> get_selected_tag_path_arr();
    TypedArray<StringName> get_edited_tag_path_arr();
    TypedArray<StringName> get_tag_path_arr(TreeItem *item);

	TreeItem *get_item_from_path_arr(TypedArray<StringName> path_arr);

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

    bool unnamed;
};