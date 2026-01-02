#include "gdtag_property_editor.hpp"

#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_undo_redo_manager.hpp>

#include "internal/helpers.hpp"
#include "internal/tag_database.hpp"
#include "internal/internal_tag.hpp"

#include "tag_editor.hpp"
#include "tag/tag_container.h"

GDTagPropertyEditor::GDTagPropertyEditor() {
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
	select_button->connect("pressed", callable_mp(this, &GDTagPropertyEditor::toggle_tag_editor));
    select_button->set_theme_type_variation("InspectorActionButton");
    select_button->set_h_size_flags(SIZE_EXPAND | SIZE_FILL);
    select_button->set_toggle_mode(true);

    h_layout->add_child(select_button);

    editor = memnew(TagEditor);
    editor->connect("tag_selected", callable_mp(this, &GDTagPropertyEditor::select_tag));
    editor->set_custom_minimum_size(Vector2(0, 400));
    editor->set_visible(false);

    container->add_child(editor);
}

void GDTagPropertyEditor::_exit_tree() {
    container->remove_child(editor);
    memdelete(editor);
    
    h_layout->remove_child(select_button);
    memdelete(select_button);

    h_layout->remove_child(property_label);
    memdelete(property_label);
        
    container->remove_child(h_layout);
    memdelete(h_layout);

    remove_child(container);
    memdelete(container);
}

void GDTagPropertyEditor::initialize(Object *p_owner, String p_property_name) {
    owner = p_owner;
    property_name = p_property_name;

    property_label->set_text(property_name.capitalize());
}

void GDTagPropertyEditor::toggle_tag_editor() {
    bool visible = !editor->is_visible();
    editor->set_visible(visible);

    if (!visible) { return; }
    editor->refresh_tags();
}
