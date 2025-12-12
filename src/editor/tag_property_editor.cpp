#include "tag_property_editor.h"

#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_undo_redo_manager.hpp>

#include "internal/helpers.hpp"
#include "internal/tag_database.hpp"
#include "internal/tag_tree_item.hpp"

#include "tag_editor.hpp"
#include "tag/tag.h"

TagPropertyEditor::TagPropertyEditor() {
    container = memnew(VBoxContainer);
	container->add_theme_constant_override("separation", 0);
    add_child(container);
    
    h_layout = memnew(HBoxContainer);
    container->add_child(h_layout);
    
    property_label = memnew(Label);
    property_label->set_h_size_flags(SIZE_EXPAND | SIZE_FILL);
    property_label->set_stretch_ratio(0.5f);
    h_layout->add_child(property_label);
    
    select_button = memnew(Button);
	select_button->connect("pressed", callable_mp(this, &TagPropertyEditor::toggle_tag_editor));
    select_button->set_theme_type_variation("InspectorActionButton");
    select_button->set_h_size_flags(SIZE_EXPAND | SIZE_FILL);
    select_button->set_toggle_mode(true);

    h_layout->add_child(select_button);

    editor = memnew(TagEditor);
    editor->connect("tag_selected", callable_mp(this, &TagPropertyEditor::select_tag));
    editor->set_custom_minimum_size(Vector2(0, 400));
    editor->set_visible(false);
    editor->set_mode(TagEditorMode::SELECT);

    container->add_child(editor);
}

void TagPropertyEditor::initialize(Object *p_owner, String p_property_name) {
    owner = p_owner;
    property_name = p_property_name;

    property_label->set_text(property_name.capitalize());
    get_tag();

    if (tag == nullptr) {
        select_button->set_text("Select Tag");
        return;
    }

    StringName name = tag->get_tag_path();
    if (name == SNAME("")) {
        select_button->set_text("Select Tag");
        return;
    }

    select_button->set_text(name);
}

void TagPropertyEditor::_bind_methods() {
}

void TagPropertyEditor::toggle_tag_editor() {
    bool visible = !editor->is_visible();
    editor->set_visible(visible);

    if (!visible) { return; }
    editor->refresh_tags();
    
    if (tag == nullptr) {
        return;
    }

    TypedArray<StringName> path_arr = TagHelpers::split_path(tag->get_tag_path());
    editor->check_tag(path_arr);
}

void TagPropertyEditor::get_tag() {
    if (tag != nullptr) {
        return;
    }
    
    Variant var = owner->get(property_name);
    if (var.get_type() == Variant::NIL) {
        return;
    }

    tag = cast_to<Tag>(var);
}

void TagPropertyEditor::select_tag(TypedArray<StringName> tag_path_arr) {
    TagDatabase *database = TagDatabase::get_singleton();
    StringName tag_path = TagHelpers::merge_path(tag_path_arr);
    
    EditorUndoRedoManager *undo_redo = EditorInterface::get_singleton()->get_editor_undo_redo();
    undo_redo->create_action("Set tag");
    
    if (tag == nullptr) {       
        Ref<Tag> ref;
        ref.instantiate();
        tag = ref.ptr();
        
        undo_redo->add_do_property(owner, property_name, ref);
        undo_redo->add_undo_property(owner, property_name, nullptr);
    }
    
    StringName old_tag_path = tag->get_tag_path();
    TypedArray<StringName> old_path_arr = TagHelpers::split_path(old_tag_path);
    
    // UtilityFunctions::print("Setting tag path: '" + tag_path + "'");
    undo_redo->add_do_method(tag, "set_tag_path", tag_path);
    undo_redo->add_undo_method(tag, "set_tag_path", old_tag_path);

    undo_redo->add_do_method(select_button, "set_text", tag_path);
    undo_redo->add_undo_method(select_button, "set_text", old_tag_path == SNAME("") ? "Select Tag" : old_tag_path);

    undo_redo->add_do_method(editor, "uncheck_tags");
    undo_redo->add_undo_method(editor, "uncheck_tags");

    undo_redo->add_do_method(editor, "check_tag", tag_path_arr);
    undo_redo->add_undo_method(editor, "check_tag", old_path_arr);

    undo_redo->commit_action();
}
