#include "tag_editor.hpp"

#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_undo_redo_manager.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/line_edit.hpp>
#include <godot_cpp/classes/style_box.hpp>

#include "internal/helpers.hpp"
#include "internal/tag_database.hpp"
#include "internal/internal_tag.hpp"

TagEditor::TagEditor() {
    set_name("Tag Editor");
    unnamed = false;

    database = TagDatabase::get_singleton();
    toggle_database_signal_connections(true);

    main_container = memnew(PanelContainer);
    main_container->set_anchors_preset(PRESET_FULL_RECT);
    add_child(main_container);
    
    main_vbox = memnew(VBoxContainer);
    main_vbox->set_v_size_flags(SIZE_EXPAND | SIZE_FILL);
	main_vbox->add_theme_constant_override("separation", 0);
    main_container->add_child(main_vbox);

    header_container = memnew(PanelContainer);
    main_vbox->add_child(header_container);
    
    header_hbox = memnew(HBoxContainer);
    header_container->add_child(header_hbox);
    
    add_tag_btn = memnew(Button);
    add_tag_btn->set_theme_type_variation("FlatMenuButton");
	add_tag_btn->connect("pressed", callable_mp(this, &TagEditor::add_tag));
    add_tag_btn->set_tooltip_text("Add a New Tag, if you have a tag selected, the new tag will be parented to it.");
	header_hbox->add_child(add_tag_btn);

    del_tag_btn = memnew(Button);
	del_tag_btn->set_theme_type_variation("FlatMenuButton");
	del_tag_btn->connect("pressed", callable_mp(this, &TagEditor::prompt_delete_tag));
    del_tag_btn->set_tooltip_text("Delete the selected tag and its children.");
	header_hbox->add_child(del_tag_btn);

    delete_confirm = memnew(ConfirmationDialog);
	delete_confirm->connect("confirmed", callable_mp(this, &TagEditor::delete_selected_tag));
    add_child(delete_confirm);
    
    tree_container = memnew(PanelContainer);
    tree_container->set_v_size_flags(SIZE_EXPAND | SIZE_FILL);

    tag_tree = memnew(TagEditorTree);
    tag_tree->connect("empty_clicked", callable_mp(this, &TagEditor::empty_clicked));
    tag_tree->connect("item_activated", callable_mp(this, &TagEditor::prompt_selected_tag_rename));
    tag_tree->connect("item_edited", callable_mp(this, &TagEditor::update_tag_database));

    tree_container->add_child(tag_tree);
    main_vbox->add_child(tree_container);
}

TagEditor::~TagEditor() {
    database = nullptr;
}

void TagEditor::_enter_tree() {
    add_tag_btn->set_button_icon(get_theme_icon(SNAME("Add"), SNAME("EditorIcons")));

    main_container->add_theme_stylebox_override("panel",
    get_theme_stylebox("sub_inspector_bg1", SNAME("EditorStyles")));

    del_tag_btn->set_button_icon(get_theme_icon(SNAME("Remove"), SNAME("EditorIcons")));
}

void TagEditor::_exit_tree() {
    tree_container->remove_child(tag_tree);
    memdelete(tag_tree);

    main_vbox->remove_child(tree_container);
    memdelete(tree_container);
    
    remove_child(delete_confirm);
    memdelete(delete_confirm);

	header_hbox->remove_child(del_tag_btn);
    memdelete(del_tag_btn);

	header_hbox->remove_child(add_tag_btn);
    memdelete(add_tag_btn);

    header_container->remove_child(header_hbox);
    memdelete(header_hbox);

    main_vbox->remove_child(header_container);
    memdelete(header_container);

    main_container->remove_child(main_vbox);
    memdelete(main_vbox);
    
    remove_child(main_container);
    memdelete(main_container);
}

void TagEditor::_bind_methods() {
    ClassDB::bind_method(D_METHOD("prompt_selected_tag_name"), &TagEditor::prompt_selected_tag_name);
    ClassDB::bind_method(D_METHOD("prompt_selected_tag_rename"), &TagEditor::prompt_selected_tag_rename);
    ClassDB::bind_method(D_METHOD("check_tag"), &TagEditor::check_tag);
    ClassDB::bind_method(D_METHOD("uncheck_tag"), &TagEditor::uncheck_tag);
    ClassDB::bind_method(D_METHOD("uncheck_all_tags"), &TagEditor::uncheck_all_tags);
    ClassDB::bind_method(D_METHOD("remove_unnamed"), &TagEditor::remove_unnamed);

    ADD_SIGNAL(MethodInfo("tag_selected", PropertyInfo(Variant::ARRAY, "tag_path")));
    ADD_SIGNAL(MethodInfo("tag_unselected", PropertyInfo(Variant::ARRAY, "tag_path")));
}

void TagEditor::refresh_tags() {
    tag_tree->push_last_config();
    tag_tree->reset();
    tag_tree->pop_last_config();
}

void TagEditor::toggle_database_signal_connections(bool on) {
    if (on) {
        database->connect("tag_added", callable_mp(this, &TagEditor::refresh_tags));
        database->connect("tag_renamed", callable_mp(this, &TagEditor::refresh_tags));
        database->connect("tag_moved", callable_mp(this, &TagEditor::refresh_tags));
        database->connect("tag_removed", callable_mp(this, &TagEditor::refresh_tags));
        return;
    }
    
    database->disconnect("tag_added", callable_mp(this, &TagEditor::refresh_tags));
    database->disconnect("tag_renamed", callable_mp(this, &TagEditor::refresh_tags));
    database->disconnect("tag_moved", callable_mp(this, &TagEditor::refresh_tags));
    database->disconnect("tag_removed", callable_mp(this, &TagEditor::refresh_tags));
}

void TagEditor::add_tag() {
	TreeItem *selected_item = tag_tree->get_selected();
    TreeItem *parent = selected_item == nullptr ? tag_tree->root() : selected_item;
    
    TreeItem *item = tag_tree->create_tree_item(selected_item);
    item->set_text(0, "NewTag" + UtilityFunctions::str(parent->get_child_count()));
    item->select(0);

    // If this is called in the same frame, the textlabel is positioned incorrectly
    call_deferred(SNAME("prompt_selected_tag_name"));
}

void TagEditor::toggle_select_tag() {
    TypedArray<StringName> path_arr = get_edited_tag_path_arr();
    TreeItem *checked_item = tag_tree->get_edited();
    
    if (!checked_item->is_checked(1)) {
        if (tag_tree->get_mode() == TagTreeMode::SELECT) {
            checked_item->set_checked(1, true);
            return;
        }
        
        emit_signal("tag_unselected", path_arr);
        // UtilityFunctions::print("Unselect");
        return;
    }
    
    if (tag_tree->get_mode() == TagTreeMode::SELECT) {
        uncheck_all_tags();
        checked_item->set_checked(1, true);
    }

    emit_signal("tag_selected", path_arr);
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

    if (tag_tree->get_mode() != TagTreeMode::READ) {
        selected_item->set_checked(1, false);
    }
    
	// UtilityFunctions::print("\nDeleting tag...");
    InternalTag *tag = database->get_tag(get_selected_tag_path_arr());
    if (tag == nullptr) {
        memdelete(selected_item);
        return;
    }

    toggle_database_signal_connections(false);
    
    database->remove_tag(tag);
    memdelete(selected_item);

    toggle_database_signal_connections(true);
}

// Prevents attempt to rename on creating new tag
void TagEditor::prompt_selected_tag_name() {
    TreeItem *selected_item = tag_tree->get_selected();
    
    if (selected_item == nullptr) {
        return;
    }
    
    old_tag_name = SNAME("");
    tag_tree->edit_selected(true);
    get_line_edit(tag_tree);
    unnamed = true;
}

void TagEditor::prompt_selected_tag_rename() {
    TreeItem *selected_item = tag_tree->get_selected();
    
    if (selected_item == nullptr || !selected_item->is_selected(0)) {
        return;
    }
    
    old_tag_name = selected_item->get_text(0);
    tag_tree->edit_selected(true);
}

void TagEditor::update_tag_database() {
    toggle_database_signal_connections(false);
    unnamed = false;

    TreeItem *selected_item = tag_tree->get_edited();
    StringName new_name = selected_item->get_text(0);

	// UtilityFunctions::print("\nUpdating tags...");

    TypedArray<StringName> tag_path = get_edited_tag_path_arr();
    InternalTag *current = database->get_tag(tag_path);
    
    if (new_name == SNAME("")) {
        UtilityFunctions::push_warning("Empty tags are forbidden!");

        selected_item->set_text(0, old_tag_name);
        call_deferred(SNAME("prompt_selected_tag_rename"));
        toggle_database_signal_connections(true);
        return;
    }
    
    if (current != nullptr) {
        if (new_name == old_tag_name) {
            UtilityFunctions::push_warning("New tag name is the same as the old name!");
            toggle_database_signal_connections(true);
            return;
        }
        
        if (tag_tree->get_mode() != TagTreeMode::READ && selected_item->is_selected(1)) {
            toggle_select_tag();
            toggle_database_signal_connections(true);
            return;
        }
        
        UtilityFunctions::push_warning("There is already a tag with that name!");
        selected_item->set_text(0, old_tag_name);
        toggle_database_signal_connections(true);

        if (old_tag_name == SNAME("")) {
            call_deferred(SNAME("prompt_selected_tag_name"));
        }
        else {
            call_deferred(SNAME("prompt_selected_tag_rename"));
        }

        return;
    }
    
    if (old_tag_name == SNAME("")) {        
        add_new_tag(tag_path, new_name);
        toggle_database_signal_connections(true);
        return;
    }
    
    rename_selected_tag(tag_path, new_name);
    toggle_database_signal_connections(true);
}

void TagEditor::add_new_tag(TypedArray<StringName> selected_tag_path_arr, StringName tag_name) {
    if (selected_tag_path_arr.size() <= 1) {
        database->add_tag(tag_name, nullptr);
        database->save();
        return;
    }

    selected_tag_path_arr.pop_back();
    
    // UtilityFunctions::print("Getting parent...");
    InternalTag *parent = database->get_tag(selected_tag_path_arr);
    
    // UtilityFunctions::print("Adding tag with parent...");
    database->add_tag(tag_name, parent);
    database->save();   
}

void TagEditor::rename_selected_tag(TypedArray<StringName> selected_tag_path_arr, StringName new_name) {
	// UtilityFunctions::print("Trying to rename old tag...");

    selected_tag_path_arr.pop_back();
    selected_tag_path_arr.append(old_tag_name);
    InternalTag *old = database->get_tag(selected_tag_path_arr);

    if (old != nullptr) {
        database->rename_tag(old, new_name);
        old_tag_name = "";
    }

    old_tag_name = SNAME("");
}

void TagEditor::empty_clicked(Vector2 position, MouseButton button) {
    tag_tree->deselect_all();
}

bool TagEditor::get_line_edit(Node *parent) {
    TypedArray<Node> children = parent->get_children(true);

    for (size_t i = 0; i < children.size(); i++)
    {
        LineEdit *line = cast_to<LineEdit>(children[i]);
        Node *node = cast_to<Node>(children[i]);
        // UtilityFunctions::print(node->get_name());

        if (line == nullptr) {
            if (get_line_edit(node)) {
                return true;
            }

            continue;
        }

        // UtilityFunctions::print("Found line");
        line->connect("editing_toggled", callable_mp(this, &TagEditor::remove_from_escape).bind(line));
        return true;
    }

    // UtilityFunctions::print("No line...");
    return false;
}

void TagEditor::remove_from_escape(bool on, LineEdit *line) {
    // UtilityFunctions::print(on);
    if (on) {
        return;
    }

    line->disconnect("editing_toggled", callable_mp(this, &TagEditor::remove_from_escape));
    call_deferred("remove_unnamed");
}

void TagEditor::remove_unnamed() {
    if (!unnamed) {
        return;
    }
    
    TreeItem *selected_item = tag_tree->get_selected();
    memdelete(selected_item);
}

TypedArray<StringName> TagEditor::get_selected_tag_path_arr() {    
    TreeItem *item = tag_tree->get_selected();
    return tag_tree->get_tag_path_arr(item);
}

TypedArray<StringName> TagEditor::get_edited_tag_path_arr() {
    TreeItem *item = tag_tree->get_edited();
    return tag_tree->get_tag_path_arr(item);
}
