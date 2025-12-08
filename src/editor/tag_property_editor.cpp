#include "tag_property_editor.h"
#include "internal/tag_database.hpp"

TagPropertyEditor::TagPropertyEditor() {
    container = memnew(MarginContainer);
    add_child(container);

    h_layout = memnew(HBoxContainer);
    container->add_child(h_layout);

    property_name = memnew(Label);
    property_name->set_h_size_flags(SIZE_EXPAND | SIZE_FILL);
    h_layout->add_child(property_name);

    select_button = memnew(Button);
    select_button->set_text("Select Tag");
    select_button->set_h_size_flags(SIZE_EXPAND | SIZE_FILL);
    h_layout->add_child(select_button);
}

void TagPropertyEditor::set_property_name(String name) {
    property_name->set_text(name.capitalize());
}

void TagPropertyEditor::_bind_methods() {
}