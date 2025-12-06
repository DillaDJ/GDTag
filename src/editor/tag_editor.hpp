#pragma once

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/tree.hpp>
#include <godot_cpp/classes/confirmation_dialog.hpp>

using namespace godot;

class TagEditor : public Control {
    GDCLASS(TagEditor, Control);

public:
    TagEditor();
    ~TagEditor() { }

	static TagEditor *get_singleton() { return singleton; }
    
	void initialize();
    
protected:
    static void _bind_methods() { }
	
	void add_tag();

	void prompt_soft_delete_tag();
	void prompt_hard_delete_tag();
	void prompt_delete_tag();

	void delete_selected_tag();

	void reparent_children(TreeItem *selected_item);

	void empty_clicked(Vector2 position, MouseButton button);

private:
    static TagEditor *singleton;

    VBoxContainer *main_vbox;
    HBoxContainer *header_hbox;

    Button *add_tag_btn;
    Button *del_tag_btn;
    Button *del_tag_btn_hard;

    Tree *tag_tree;

    TreeItem *root;

    ConfirmationDialog *delete_confirm;

    bool hard_delete;
};