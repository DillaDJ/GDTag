#include "tag_plugin.hpp"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/Node.hpp>
#include <godot_cpp/classes/Control.hpp>

#include <godot_cpp/variant/utility_functions.hpp>
#include "internal/tag_database.hpp"
#include "editor/tag_editor.hpp"
#include "editor/tag_inspector_plugin.h"

using namespace godot;

TagPlugin::TagPlugin() {
}

void TagPlugin::_enter_tree() {
	TagDatabase *database = TagDatabase::get_singleton();
    
    tag_inspector = memnew(TagInspectorPlugin);
    add_inspector_plugin(tag_inspector);

    add_tool_menu_item("Open Tag Editor Dock", callable_mp(this, &TagPlugin::OpenTagEditor));
}

void TagPlugin::_exit_tree() {
    remove_inspector_plugin(tag_inspector);
}

void TagPlugin::OpenTagEditor() {
    if (editor == nullptr) {
        editor = memnew(TagEditor);
    }
    
    if (editor->get_parent() != nullptr) {
        remove_control_from_docks(editor);
    }

    add_control_to_dock(EditorPlugin::DOCK_SLOT_LEFT_UR, editor);
}
