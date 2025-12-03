#include "tag_plugin.hpp"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/Node.hpp>
#include <godot_cpp/classes/Control.hpp>

#include "tag_editor.hpp"

using namespace godot;

void TagPlugin::_enter_tree() {
    add_tool_menu_item("Open Tag Editor Dock", callable_mp(this, &TagPlugin::OpenTagEditor));
}

void TagPlugin::OpenTagEditor() {
    if (editor == nullptr) {
        editor = memnew(TagEditor);
        editor->initialize();
    }
    
    if (editor->get_parent() != nullptr) {
        remove_control_from_docks(editor);
    }

    add_control_to_dock(EditorPlugin::DOCK_SLOT_LEFT_UR, editor);
}
