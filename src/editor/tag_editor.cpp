#include "tag_editor.hpp"

#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_undo_redo_manager.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/style_box.hpp>

#include "internal/helpers.hpp"
#include "internal/tag_database.hpp"
#include "internal/tag_tree_item.hpp"

TagEditor::TagEditor() {
    set_name("Tag Editor");
    mode = TagEditorMode::READ;

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
    header_hbox = memnew(HBoxContainer);
    header_container->add_child(header_hbox);
    main_vbox->add_child(header_container);

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

    tag_tree = memnew(Tree);
    tag_tree->connect("empty_clicked", callable_mp(this, &TagEditor::empty_clicked));
    tag_tree->connect("item_activated", callable_mp(this, &TagEditor::prompt_selected_tag_rename));
    tag_tree->connect("item_edited", callable_mp(this, &TagEditor::update_tag_database));
    tag_tree->set_hide_root(true);

    tree_container->add_child(tag_tree);
    main_vbox->add_child(tree_container);
}

void TagEditor::_bind_methods() {
    ClassDB::bind_method(D_METHOD("prompt_selected_tag_name"), &TagEditor::prompt_selected_tag_name);
    ClassDB::bind_method(D_METHOD("prompt_selected_tag_rename"), &TagEditor::prompt_selected_tag_rename);
    ClassDB::bind_method(D_METHOD("check_tag"), &TagEditor::check_tag);
    ClassDB::bind_method(D_METHOD("uncheck_tags"), &TagEditor::uncheck_tags);

    ADD_SIGNAL(MethodInfo("tag_selected", PropertyInfo(Variant::ARRAY, "tag_path")));
    ADD_SIGNAL(MethodInfo("tag_unselected", PropertyInfo(Variant::ARRAY, "tag_path")));
}

void TagEditor::_enter_tree() {
    add_tag_btn->set_button_icon(get_theme_icon(SNAME("Add"), SNAME("EditorIcons")));

    main_container->add_theme_stylebox_override("panel",
        get_theme_stylebox("sub_inspector_bg1", SNAME("EditorStyles")));

    del_tag_btn->set_button_icon(get_theme_icon(SNAME("Remove"), SNAME("EditorIcons")));    
}

void TagEditor::set_mode(TagEditorMode p_mode) {
    tag_tree->set_columns(p_mode == TagEditorMode::READ ? 1 : 2);
    mode = p_mode; 
    
    if (p_mode == TagEditorMode::READ) { return; }

    tag_tree->set_column_expand(1, false);
}

void TagEditor::refresh_tags() {
    tag_tree->clear();
    root = tag_tree->create_item();
    populate_tags();
}

void TagEditor::check_tag(TypedArray<StringName> path_arr) {
    // UtilityFunctions::print("Checking tag with path:");
    // UtilityFunctions::print(path);

    if (root->get_child_count() == 0 || path_arr.size() == 0) {
        return;
    }

    StringName node = (StringName) path_arr.pop_front();
    TreeItem *child = root->get_first_child();

    while (child != nullptr)
    {
        // UtilityFunctions::print("\nNode: '" + node + "' Child: '" + child->get_text(0) + "'");

        if (child->get_text(0) == node) {
            if (path_arr.size() == 0) {
                child->set_checked(1, true);
                // UtilityFunctions::print("Found node to check!");
                return;
            }

            // UtilityFunctions::print("Found node... getting first child.");
            child = child->get_first_child();
            node = (StringName) path_arr.pop_front();
        }
        else {
            child = child->get_next();
            // UtilityFunctions::print("Not this... getting next child");
        }
    }
}

void TagEditor::uncheck_tags() {
    uncheck_recursive(root);
}

void TagEditor::populate_tags() {
	Array tags = database->get_tags();

    for (size_t i = 0; i < tags.size(); i++)
    {
        TagTreeItem *tag = cast_to<TagTreeItem>(tags[i]);
        
        TreeItem *item = create_tree_item(root);
        item->set_text(0, tag->get_name());

        populate_children_recursive(item, tag);
    }
}

void TagEditor::populate_children_recursive(TreeItem *parent_item, TagTreeItem *parent_tag) {
    Array children = parent_tag->get_children();

    for (size_t i = 0; i < children.size(); i++)
    {
        TagTreeItem *tag = cast_to<TagTreeItem>(children[i]);

        TreeItem *item = create_tree_item(parent_item);
        item->set_text(0, tag->get_name());
        
        populate_children_recursive(item, tag);
    }
}

void TagEditor::toggle_database_signal_connections(bool on) {
    if (on) {
        database->connect("tag_added", callable_mp(this, &TagEditor::refresh_tags));
        database->connect("tag_renamed", callable_mp(this, &TagEditor::refresh_tags));
        database->connect("tag_removed", callable_mp(this, &TagEditor::refresh_tags));
        return;
    }
    
    database->disconnect("tag_added", callable_mp(this, &TagEditor::refresh_tags));
    database->disconnect("tag_renamed", callable_mp(this, &TagEditor::refresh_tags));
    database->disconnect("tag_removed", callable_mp(this, &TagEditor::refresh_tags));
}

TreeItem *TagEditor::create_tree_item(TreeItem *parent) {
	// UtilityFunctions::print("\nAdding new tag to " + (parent == root ? "root" : parent->get_text(0)) + "...");
    TreeItem *item = tag_tree->create_item(parent);
    
    if (mode != TagEditorMode::READ) {
        item->set_cell_mode(1, TreeItem::CELL_MODE_CHECK);
        item->set_editable(1, true);
    }

    return item;
}

void TagEditor::add_tag() {
	TreeItem *selected_item = tag_tree->get_selected();
    TreeItem *parent = selected_item == nullptr ? root : selected_item;
    
    TreeItem *item = create_tree_item(selected_item);
    item->set_text(0, "NewTag" + UtilityFunctions::str(parent->get_child_count()));
    item->select(0);

    // If this is called in the same frame, the textlabel is positioned incorrectly
    call_deferred(SNAME("prompt_selected_tag_name"));
}

void TagEditor::toggle_select_tag() {
    TypedArray<StringName> path_arr = get_edited_tag_path_arr();
    TreeItem *checked_item = tag_tree->get_edited();
    
    if (!checked_item->is_checked(1)) {
        if (mode == TagEditorMode::SELECT) {
            checked_item->set_checked(1, true);
            return;
        }
        
        emit_signal("tag_unselected", path_arr);
        UtilityFunctions::print("Unselect");
        checked_item = nullptr;
        return;
    }
    
    if (mode == TagEditorMode::SELECT) {
        uncheck_recursive(root);
        checked_item->set_checked(1, true);
    }

    emit_signal("tag_selected", path_arr);
}

void TagEditor::uncheck_recursive(TreeItem *item) {
    if (item->get_child_count() == 0) {
        return;
    }

    TreeItem *child = item->get_first_child();

    while (child != nullptr) {
        child->set_checked(1, false);
        uncheck_recursive(child);
        child = child->get_next();
    }
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
    
	// UtilityFunctions::print("\nDeleting tag...");
    TagTreeItem *tag = database->get_tag(get_selected_tag_path_arr());
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

    TreeItem *selected_item = tag_tree->get_edited();
    if (mode == TagEditorMode::SELECT && selected_item->is_selected(1)) {
	    toggle_select_tag();
        toggle_database_signal_connections(true);
        return;
    }

    StringName new_name = selected_item->get_text(0);
	// UtilityFunctions::print("\nUpdating tags...");

    TypedArray<StringName> tag_path = get_edited_tag_path_arr();
    TagTreeItem *current = database->get_tag(tag_path);
    
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
        database->add_tag(tag_name);
        database->save();
        return;
    }

    selected_tag_path_arr.pop_back();
    
    // UtilityFunctions::print("Getting parent...");
    TagTreeItem *parent = database->get_tag(selected_tag_path_arr);
    
    // UtilityFunctions::print("Adding tag with parent...");
    database->add_tag(tag_name, parent);
    database->save();
}

void TagEditor::rename_selected_tag(TypedArray<StringName> selected_tag_path_arr, StringName new_name) {
	// UtilityFunctions::print("Trying to rename old tag...");

    selected_tag_path_arr.pop_back();
    selected_tag_path_arr.append(old_tag_name);
    TagTreeItem *old = database->get_tag(selected_tag_path_arr);

    if (old != nullptr) {
        database->rename_tag(old, new_name);
        old_tag_name = "";
    }

    old_tag_name = SNAME("");
}

void TagEditor::empty_clicked(Vector2 position, MouseButton button) {
    tag_tree->deselect_all();
}

TypedArray<StringName> TagEditor::get_selected_tag_path_arr() {    
    TreeItem *item = tag_tree->get_selected();
    return get_tag_path_arr(item);
}

TypedArray<StringName> TagEditor::get_edited_tag_path_arr() {
    TreeItem *item = tag_tree->get_edited();
    return get_tag_path_arr(item);
}

TypedArray<StringName> TagEditor::get_tag_path_arr(TreeItem *item) {
    TypedArray<StringName> path = TypedArray<StringName>();
    
    if (item == nullptr || item == root) {
        return path;
    }
    
    while (item != root) {
        path.append(item->get_text(0));
        item = item->get_parent();
    }

    path.reverse();
	return path;
}