#include "tag_plugin.hpp"

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/Node.hpp>
#include <godot_cpp/classes/Control.hpp>
#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/classes/editor_settings.hpp>

#include <godot_cpp/variant/utility_functions.hpp>
#include "internal/tag_database.hpp"
#include "editor/tag_editor.hpp"
#include "editor/tag_inspector_plugin.h"

using namespace godot;

TagPlugin::TagPlugin() {
    TagDatabase *database = TagDatabase::get_singleton();

    Ref<EditorSettings> settings = EditorInterface::get_singleton()->get_editor_settings();
    if (settings->has_setting("GD_tag/tag_database_location")) {
        return;
    }
    
    settings->set_setting("GD_tag/tag_database_location", "res://");

    Dictionary property_info;
    property_info["name"] = "GD_tag/tag_database_location";
    property_info["type"] = Variant::Type::STRING;
    property_info["hint"] = PROPERTY_HINT_DIR;

    settings->add_property_info(property_info);
}

TagPlugin::~TagPlugin() {
    TagDatabase::uninitialize();
}

void TagPlugin::_enter_tree() {
    editor = nullptr;

    Ref<TagInspectorPlugin> plugin(memnew(TagInspectorPlugin));
    tag_inspector = plugin;

    add_inspector_plugin(tag_inspector);

    add_tool_menu_item("Open Tag Editor Dock", callable_mp(this, &TagPlugin::OpenTagEditor));
}

void TagPlugin::_exit_tree() {
    remove_inspector_plugin(tag_inspector);
    tag_inspector.unref();
    
    if (editor != nullptr) {
        memdelete(editor);
    }
}

void TagPlugin::OpenTagEditor() {
    if (editor == nullptr) {
        editor = memnew(TagEditor);
    }
    
    if (editor->get_parent() != nullptr) {
        remove_control_from_docks(editor);
    }

    add_control_to_dock(EditorPlugin::DOCK_SLOT_LEFT_UR, editor);
    editor->refresh_tags();
}
