#include "tag_editor.hpp"

TagEditor* TagEditor::singleton = nullptr;

TagEditor::TagEditor() {
    singleton = this;

    main_vbox = memnew(VBoxContainer);
}

void TagEditor::initialize() {
    set_name("Tag Editor");

    add_child(main_vbox);
}
