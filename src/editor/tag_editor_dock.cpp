#include "tag_editor_dock.h"
#include "tag_editor.hpp"

TagEditorDock::TagEditorDock() {
	editor = memnew(TagEditor);
    add_child(editor);
}

void TagEditorDock::_bind_methods() {
}
