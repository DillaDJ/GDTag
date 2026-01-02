#include "tag_property_editor.hpp"

#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_undo_redo_manager.hpp>

#include "internal/helpers.hpp"
#include "internal/tag_database.hpp"
#include "internal/internal_tag.hpp"

#include "tag_editor.hpp"
#include "tag/tag.h"

TagPropertyEditor::TagPropertyEditor() {
    editor->set_mode(TagEditorMode::SELECT);
}

void TagPropertyEditor::_exit_tree() {
    tag.unref();
}

void TagPropertyEditor::initialize(Object *p_owner, String p_property_name) {
    GDTagPropertyEditor::initialize(p_owner, p_property_name);
    
    TagDatabase *database = TagDatabase::get_singleton();
    database->connect("tag_renamed", callable_mp(this, &TagPropertyEditor::refresh_button));
    get_tag();

    if (tag.is_null()) {
        select_button->set_text("Select Tag");
        return;
    }

    StringName path = tag->get_tag_path();
    if (path == SNAME("")) {
        // UtilityFunctions::print("Path is null!");
        select_button->set_text("Select Tag");
        return;
    }

    select_button->set_text(path);
}

void TagPropertyEditor::toggle_tag_editor() {
    GDTagPropertyEditor::toggle_tag_editor();
    
    if (!editor->is_visible() || tag.is_null()) {
        return;
    }

    TypedArray<StringName> path_arr = TagHelpers::split_path(tag->get_tag_path());
    editor->check_tag(path_arr);
}

void TagPropertyEditor::get_tag() {
    if (tag.is_valid()) {
        return;
    }
    
    Variant var = owner->get(property_name);
    if (var.get_type() == Variant::NIL) {
        return;
    }

    tag = var;
}

void TagPropertyEditor::refresh_button() {
    String tag_text = tag.is_null() ? "Select Tag" : tag->get_tag_path();
    select_button->set_text(tag_text);
}

void TagPropertyEditor::select_tag(TypedArray<StringName> tag_path_arr) {
    TagDatabase *database = TagDatabase::get_singleton();
    InternalTag *linked_tag = database->get_tag(tag_path_arr);
    
    EditorUndoRedoManager *undo_redo = EditorInterface::get_singleton()->get_editor_undo_redo();
    undo_redo->create_action("Set tag");
    
    if (tag.is_null()) {
        Ref<Tag> ref(memnew(Tag));
        tag = ref;
        
        undo_redo->add_do_property(owner, property_name, ref);
        undo_redo->add_undo_property(owner, property_name, nullptr);
    }

    StringName path = linked_tag->get_path();

    InternalTag *old_linked_tag = database->get_tag(tag->get_linked_id());

    StringName old_path = old_linked_tag->get_path();
    TypedArray<StringName> old_path_arr = old_linked_tag->get_path_arr();
    
    // UtilityFunctions::print("Setting tag path: '" + tag_path + "'");
    undo_redo->add_do_method(tag.ptr(), "_link_internal_tag", linked_tag);
    undo_redo->add_undo_method(tag.ptr(), "_link_internal_tag", old_linked_tag);

    undo_redo->add_do_method(select_button, "set_text", path);
    undo_redo->add_undo_method(select_button, "set_text", old_path == SNAME("") ? "Select Tag" : old_path);

    undo_redo->add_do_method(editor, "uncheck_all_tags");
    undo_redo->add_undo_method(editor, "uncheck_all_tags");

    undo_redo->add_do_method(editor, "check_tag", tag_path_arr);
    undo_redo->add_undo_method(editor, "check_tag", old_path_arr);

    undo_redo->commit_action();
}
