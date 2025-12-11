#include "tag_property_editor.h"

#include "internal/macros.h"
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
    select_button->set_text("Select Tag");

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
    Tag *tag = get_tag();

    if (tag == nullptr) {
        return;
    }

    select_button->set_text(tag->get_linked_path());
}

void TagPropertyEditor::_bind_methods() {
}

void TagPropertyEditor::toggle_tag_editor() {
    bool visible = !editor->is_visible();
    editor->set_visible(visible);

    if (!visible) { return; }

    editor->refresh_tags();
}

Tag *TagPropertyEditor::get_tag() {
    Variant var = owner->get(property_name);
    if (var.get_type() == Variant::NIL) {
        UtilityFunctions::print("No tag object!");
        return nullptr;
    }

    UtilityFunctions::print("Has tag object!");
    Tag *tag = cast_to<Tag>(var);
    
    if (tag == nullptr) {
        UtilityFunctions::push_error("Mismatched resource!");
        return nullptr;
    }

    return tag;
}

void TagPropertyEditor::select_tag(TypedArray<StringName> tag_path) {
    TagDatabase *database = TagDatabase::get_singleton();
    TagTreeItem *tag_tree_item = database->get_tag(tag_path);
    StringName combined_path = "";
    Tag *tag = get_tag();

    if (tag == nullptr) {

        for (size_t i = 0; i < tag_path.size(); i++)
        {
            StringName path_element = (StringName) tag_path[i];
            combined_path = combined_path + path_element + SNAME("/");
        }

        UtilityFunctions::print(combined_path);
        return;
    }

    tag->set_tag(tag_tree_item);
    select_button->set_text(tag->get_linked_path());
}
