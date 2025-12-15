#include "tag_container_property_editor.hpp"

#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_undo_redo_manager.hpp>

#include "internal/helpers.hpp"
#include "internal/tag_database.hpp"
#include "internal/tag_tree_item.hpp"

#include "tag_editor.hpp"
#include "tag/tag_container.h"

TagContainerPropertyEditor::TagContainerPropertyEditor() {
    editor->connect("tag_unselected", callable_mp(this, &TagContainerPropertyEditor::unselect_tag));
    editor->set_mode(TagEditorMode::SELECT_MULTI);
}

void TagContainerPropertyEditor::_exit_tree() {
    tag_container.unref();
}

void TagContainerPropertyEditor::initialize(Object *p_owner, String p_property_name) {
    GDTagPropertyEditor::initialize(p_owner, p_property_name);
    get_tag_container();

    if (tag_container.is_null()) {
        select_button->set_text("Select Tags");
        return;
    }

    int tag_count = tag_container->size();
    if (tag_count == 0) {
        select_button->set_text("Select Tags");
        return;
    }

    refresh_button_text();
}

void TagContainerPropertyEditor::_bind_methods() {
    ClassDB::bind_method(D_METHOD("refresh_button_text"), &TagContainerPropertyEditor::refresh_button_text);
}

void TagContainerPropertyEditor::toggle_tag_editor() {
    GDTagPropertyEditor::toggle_tag_editor();
    
    if (!editor->is_visible() || tag_container.is_null()) {
        return;
    }

    TypedArray<StringName> paths = tag_container->get_tag_paths();

    for (size_t i = 0; i < paths.size(); i++)
    {
        StringName path = (StringName) paths[i];
        TypedArray<StringName> path_arr = TagHelpers::split_path(path);
        editor->check_tag(path_arr);
    }
}

void TagContainerPropertyEditor::get_tag_container() {
    if (tag_container.is_valid()) {
        return;
    }
    
    Variant var = owner->get(property_name);
    if (var.get_type() == Variant::NIL) {
        return;
    }

    tag_container = var;
}

void TagContainerPropertyEditor::select_tag(TypedArray<StringName> tag_path_arr) {
    TagDatabase *database = TagDatabase::get_singleton();
    StringName tag_path = TagHelpers::merge_path(tag_path_arr);
    
    EditorUndoRedoManager *undo_redo = EditorInterface::get_singleton()->get_editor_undo_redo();
    undo_redo->create_action("Select tag");
    
    if (tag_container.is_null()) {       
        Ref<TagContainer> ref(memnew(TagContainer));
        tag_container = ref;
        
        undo_redo->add_do_property(owner, property_name, ref);
        undo_redo->add_undo_property(owner, property_name, nullptr);
    }
    
    undo_redo->add_do_method(tag_container.ptr(), "add_tag_path", tag_path);
    undo_redo->add_undo_method(tag_container.ptr(), "remove_tag_path", tag_path);

    undo_redo->add_do_method(this, "refresh_button_text");
    undo_redo->add_undo_method(this, "refresh_button_text");

    undo_redo->add_do_method(editor, "check_tag", tag_path_arr);
    undo_redo->add_undo_method(editor, "uncheck_tag", tag_path_arr);

    undo_redo->commit_action();
}

void TagContainerPropertyEditor::unselect_tag(TypedArray<StringName> tag_path_arr) {
    TagDatabase *database = TagDatabase::get_singleton();
    StringName tag_path = TagHelpers::merge_path(tag_path_arr);
    
    EditorUndoRedoManager *undo_redo = EditorInterface::get_singleton()->get_editor_undo_redo();
    undo_redo->create_action("Select tag");
    
    if (tag_container.is_null()) {       
        Ref<TagContainer> ref(memnew(TagContainer));
        tag_container = ref;
        
        undo_redo->add_do_property(owner, property_name, ref);
        undo_redo->add_undo_property(owner, property_name, nullptr);
    }
    
    undo_redo->add_do_method(tag_container.ptr(), "remove_tag_path", tag_path);
    undo_redo->add_undo_method(tag_container.ptr(), "add_tag_path", tag_path);

    undo_redo->add_do_method(this, "refresh_button_text");
    undo_redo->add_undo_method(this, "refresh_button_text");

    undo_redo->add_do_method(editor, "uncheck_tag", tag_path_arr);
    undo_redo->add_undo_method(editor, "check_tag", tag_path_arr);

    undo_redo->commit_action();
}

void TagContainerPropertyEditor::refresh_button_text() {
}
