#include "tag_editor.hpp"

#include "macros.h"
#include <godot_cpp/core/class_db.hpp>
#include "internal/tag_database.hpp"
#include "internal/tag_tree_item.h"

TagEditor* TagEditor::singleton = nullptr;

TagEditor::TagEditor() {
    singleton = this;

    main_vbox = memnew(VBoxContainer);
    header_hbox = memnew(HBoxContainer);
    add_tag_btn = memnew(Button);
    del_tag_btn = memnew(Button);
    del_tag_btn_hard = memnew(Button);
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
	del_tag_btn->connect("pressed", callable_mp(this, &TagEditor::prompt_soft_delete_tag));
    del_tag_btn->set_button_icon(get_theme_icon(SNAME("Remove"), SNAME("EditorIcons")));
    del_tag_btn->set_tooltip_text("Delete the selected tag, any child tags will be reparented to the parent of the deleted tag.");
	header_hbox->add_child(del_tag_btn);
    
	del_tag_btn_hard->set_theme_type_variation("FlatMenuButton");
	del_tag_btn_hard->connect("pressed", callable_mp(this, &TagEditor::prompt_hard_delete_tag));
    del_tag_btn_hard->set_button_icon(get_theme_icon(SNAME("Remove"), SNAME("EditorIcons")));
    del_tag_btn_hard->set_tooltip_text("Delete the selected tag, deleting all child tags as well.");
	header_hbox->add_child(del_tag_btn_hard);

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

    if (tag_tree->get_next_selected(selected_item) != nullptr) {
        return;
    }

    TreeItem *parent = selected_item == nullptr ? root : selected_item;
    TreeItem *item = tag_tree->create_item(parent);

    item->set_text(0, "NewTag" + UtilityFunctions::str(parent->get_child_count()));
    item->select(0);

    call_deferred(SNAME("prompt_selected_tag_rename"));
}

void TagEditor::prompt_soft_delete_tag() {
    hard_delete = false;
    prompt_delete_tag();
}

void TagEditor::prompt_hard_delete_tag() {
    hard_delete = true;
    prompt_delete_tag();
}

void TagEditor::prompt_delete_tag() {
    TreeItem *selected_item = tag_tree->get_selected();

    if (selected_item == nullptr) {
        return;
    }

    delete_confirm->set_text("Delete tag: \'" + selected_item->get_text(0) + "\'" + (hard_delete ? ", as well as all children" : "") + "?");
    delete_confirm->reset_size();
    delete_confirm->popup_centered();
}

void TagEditor::delete_selected_tag() {
    TreeItem *selected_item = tag_tree->get_selected();

    if (selected_item == nullptr) {
        return;
    }

    if (!hard_delete) {
        reparent_children(selected_item);
    }
    
    memdelete(selected_item);
}

void TagEditor::reparent_children(TreeItem *selected_item) {
    TypedArray<TreeItem> children = selected_item->get_children();
    TreeItem *parent = selected_item->get_parent();

    for (size_t i = 0; i < children.size(); i++)
    {
        TreeItem *child = cast_to<TreeItem>(children[i]);
        selected_item->remove_child(child);
        parent->add_child(child);
    }
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

    if (new_name == old_tag_name) {
        return;
    }

    TypedArray<StringName> tag_path = get_selected_tag_path();
    if (tag_path.size() == 0) {
        database->add_tag(new_name);
    }

    tag_path.pop_back();
    TagTreeItem *parent = database->get_tag(tag_path);
    database->add_tag(new_name, parent);
}

void TagEditor::empty_clicked(Vector2 position, MouseButton button) {
    tag_tree->deselect_all();
}

TypedArray<StringName> TagEditor::get_selected_tag_path() {    
    TreeItem *current_item = tag_tree->get_selected();

    TypedArray<StringName> path = TypedArray<StringName>();

    while (current_item != nullptr)
    {
        path.append(current_item);
        current_item = current_item->get_parent();
    }

    path.reverse();

	return path;
}
