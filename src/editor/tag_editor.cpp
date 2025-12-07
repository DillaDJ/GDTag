#include "tag_editor.hpp"

#include "internal/macros.h"
#include <godot_cpp/core/class_db.hpp>
#include "internal/tag_database.hpp"
#include "internal/tag_tree_item.hpp"

TagEditor* TagEditor::singleton = nullptr;

TagEditor::TagEditor() {
    singleton = this;

    main_vbox = memnew(VBoxContainer);
    header_hbox = memnew(HBoxContainer);
    add_tag_btn = memnew(Button);
    del_tag_btn = memnew(Button);
    tag_tree = memnew(Tree);
    delete_confirm = memnew(ConfirmationDialog);
}

void TagEditor::initialize() {
    database = TagDatabase::get_singleton();

    set_name("Tag Editor");

    add_child(main_vbox);
    
    main_vbox->add_child(header_hbox);
    main_vbox->set_anchors_preset(PRESET_FULL_RECT);

	add_tag_btn->set_theme_type_variation("FlatMenuButton");
	add_tag_btn->connect("pressed", callable_mp(this, &TagEditor::add_tag));
    add_tag_btn->set_button_icon(get_theme_icon(SNAME("Add"), SNAME("EditorIcons")));
    add_tag_btn->set_tooltip_text("Add a New Tag, if you have a tag selected, the new tag will be parented to it.");
	header_hbox->add_child(add_tag_btn);
    
	del_tag_btn->set_theme_type_variation("FlatMenuButton");
	del_tag_btn->connect("pressed", callable_mp(this, &TagEditor::prompt_delete_tag));
    del_tag_btn->set_button_icon(get_theme_icon(SNAME("Remove"), SNAME("EditorIcons")));
    del_tag_btn->set_tooltip_text("Delete the selected tag and its children.");
	header_hbox->add_child(del_tag_btn);

	delete_confirm->connect("confirmed", callable_mp(this, &TagEditor::delete_selected_tag));
    add_child(delete_confirm);

    tag_tree->connect("empty_clicked", callable_mp(this, &TagEditor::empty_clicked));
    tag_tree->connect("item_activated", callable_mp(this, &TagEditor::prompt_selected_tag_rename));
    tag_tree->connect("item_edited", callable_mp(this, &TagEditor::update_tag_database));

    tag_tree->set_v_size_flags(SIZE_EXPAND | SIZE_FILL);
    tag_tree->set_hide_root(true);

    root = tag_tree->create_item();
    main_vbox->add_child(tag_tree);
}

void TagEditor::_bind_methods() {
    ClassDB::bind_method(D_METHOD("prompt_selected_tag_rename"), &TagEditor::prompt_selected_tag_rename);
}

void TagEditor::add_tag() {
	TreeItem *selected_item = tag_tree->get_selected();
    TreeItem *parent = selected_item == nullptr ? root : selected_item;
	UtilityFunctions::print("\nAdding new tag to " + (parent == root ? "root" : parent->get_text(0)) + "...");
    
    TreeItem *item = tag_tree->create_item(parent);

    item->set_text(0, "NewTag" + UtilityFunctions::str(parent->get_child_count()));
    item->select(0);

    // If this is called in the same frame, the textlabel is positioned incorrectly
    call_deferred(SNAME("prompt_selected_tag_rename"));
}

void TagEditor::prompt_delete_tag() {
    TreeItem *selected_item = tag_tree->get_selected();

    if (selected_item == nullptr) {
        return;
    }

    delete_confirm->set_text("Delete tag: \'" + selected_item->get_text(0) + "" + 
        "\', as well as all its children?");
    delete_confirm->reset_size();
    delete_confirm->popup_centered();
}

void TagEditor::delete_selected_tag() {
    TreeItem *selected_item = tag_tree->get_selected();

    if (selected_item == nullptr) {
        return;
    }

	UtilityFunctions::print("\nDeleting tag...");
    TagTreeItem *tag = database->get_tag(get_selected_tag_path());
    
    database->remove_tag(tag);
    memdelete(selected_item);
}

void TagEditor::prompt_selected_tag_rename() {
    TreeItem *selected_item = tag_tree->get_selected();
    
    if (selected_item == nullptr) {
        return;
    }
    
    old_tag_name = selected_item->get_text(0);
    tag_tree->edit_selected(true);
}

void TagEditor::update_tag_database() {
    TreeItem *selected_item = tag_tree->get_selected();
    StringName new_name = selected_item->get_text(0);
	UtilityFunctions::print("\nUpdating tags...");

    TypedArray<StringName> tag_path = get_selected_tag_path();
    TagTreeItem *current = database->get_tag(tag_path);
    
    if (current != nullptr) {
        if (new_name == old_tag_name) {
            UtilityFunctions::push_warning("New tag name is the same as the old name!");
            return;
        }

        tag_path.pop_back();
        tag_path.append(new_name);
        TagTreeItem *next = database->get_tag(tag_path);
        if (next != nullptr) {
            UtilityFunctions::push_warning("There is already a tag with that name!");
            selected_item->set_text(0, old_tag_name);
            call_deferred(SNAME("prompt_selected_tag_rename"));
            return;
        }

        current->set_name(new_name);
        old_tag_name = "";
        return;
    }

    old_tag_name = "";

    if (tag_path.size() <= 1) {
        database->add_tag(new_name);
        return;
    }

    tag_path.pop_back();
    UtilityFunctions::print("Getting parent...");
    TagTreeItem *parent = database->get_tag(tag_path);
    UtilityFunctions::print("Adding tag with parent...");
    database->add_tag(new_name, parent);
}

void TagEditor::empty_clicked(Vector2 position, MouseButton button) {
    tag_tree->deselect_all();
}

TypedArray<StringName> TagEditor::get_selected_tag_path() {    
    TreeItem *current_item = tag_tree->get_selected();
    TypedArray<StringName> path = TypedArray<StringName>();
    
    if (current_item == nullptr || current_item == root) {
        return path;
    }
    
    while (current_item != root) {
        path.append(current_item->get_text(0));
        current_item = current_item->get_parent();
    }

    path.reverse();

	return path;
}
