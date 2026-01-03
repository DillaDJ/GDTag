#pragma once

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <godot_cpp/classes/panel_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/confirmation_dialog.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/variant/vector2.hpp>

#include "editor/tag_editor_tree.hpp"

using namespace godot;

class TagEditor : public Control {
    GDCLASS(TagEditor, Control);

public:
    TagEditor();
    ~TagEditor();

    void _enter_tree() override;
    void _exit_tree() override;

    void set_mode(TagTreeMode mode) { tag_tree->set_mode(mode); }
    void check_tag(TypedArray<StringName> path_arr) { tag_tree->check_tag(path_arr); }
    void uncheck_tag(TypedArray<StringName> path_arr) { tag_tree->uncheck_tag(path_arr); }
    void uncheck_all_tags() { tag_tree->uncheck_all_tags(); }

    void refresh_tags();

protected:
    static void _bind_methods();
	
private:
    void toggle_database_signal_connections(bool on);

	void add_tag();
    void toggle_select_tag();
    
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
    
    class TagDatabase *database;

    PanelContainer *main_container;
    VBoxContainer *main_vbox;

    PanelContainer *header_container;
    HBoxContainer *header_hbox;

    Button *add_tag_btn;
    Button *del_tag_btn;

    PanelContainer *tree_container;
    TagEditorTree *tag_tree;

    ConfirmationDialog *delete_confirm;

    StringName old_tag_name;

    bool unnamed;
};