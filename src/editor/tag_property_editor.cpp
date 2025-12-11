#include "tag_property_editor.h"

#include "tag_editor.hpp"
#include "internal/tag_database.hpp"

TagPropertyEditor::TagPropertyEditor() {
    container = memnew(VBoxContainer);
	container->add_theme_constant_override("separation", 0);
    add_child(container);
    
    h_layout = memnew(HBoxContainer);
    container->add_child(h_layout);
    
    property_name = memnew(Label);
    property_name->set_h_size_flags(SIZE_EXPAND | SIZE_FILL);
    h_layout->add_child(property_name);
    
    select_button = memnew(Button);
	select_button->connect("pressed", callable_mp(this, &TagPropertyEditor::toggle_tag_editor));
    select_button->set_theme_type_variation("InspectorActionButton");
    select_button->set_h_size_flags(SIZE_EXPAND | SIZE_FILL);
    select_button->set_toggle_mode(true);
    select_button->set_text("Select Tag");

    h_layout->add_child(select_button);

    editor = memnew(TagEditor);
    editor->connect("tag_selected", callable_mp(this, &TagPropertyEditor::select_tag));
    editor->set_custom_minimum_size(Vector2(0, 400));
    editor->set_visible(false);
    editor->set_mode(TagEditorMode::SELECT);

    container->add_child(editor);
}

void TagPropertyEditor::set_property_name(String name) {
    property_name->set_text(name.capitalize());
}

void TagPropertyEditor::_bind_methods() {
}

void TagPropertyEditor::toggle_tag_editor() {
    bool visible = !editor->is_visible();
    editor->set_visible(visible);

    if (!visible) { return; }

    editor->refresh_tags();
}

void TagPropertyEditor::select_tag(TypedArray<StringName> tag_path) {
    UtilityFunctions::print(tag_path);
}
