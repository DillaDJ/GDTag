#include "tag_property_editor.h"
#include "internal/tag_database.hpp"

TagPropertyEditor::TagPropertyEditor() {
    select_button = memnew(Button);
    select_button->set_text("Select Tag");
    add_child(select_button);
}

void TagPropertyEditor::_update_property() {
    Variant current = get_edited_object()->get(get_edited_property());
}

void TagPropertyEditor::_on_tag_selected(int32_t index) {
    _update_property();
}

void TagPropertyEditor::_bind_methods() {
}