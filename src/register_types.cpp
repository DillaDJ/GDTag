#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/editor_plugin_registration.hpp>

#include "tag_plugin.hpp"

#include "internal/tag_database.hpp"
#include "internal/tag_tree_item.hpp"

#include "editor/tag_editor.hpp"
#include "editor/tag_editor_dock.h"
#include "editor/tag_inspector_plugin.h"
#include "editor/tag_property_editor.h"

#include "tag/tag.h"
#include "tag/tag_container.h"

using namespace godot;

void initialize(ModuleInitializationLevel p_level)
{
	switch (p_level)
	{		
		case MODULE_INITIALIZATION_LEVEL_SCENE:
			GDREGISTER_INTERNAL_CLASS(TagDatabase);
			GDREGISTER_INTERNAL_CLASS(TagTreeItem);
			
			GDREGISTER_CLASS(Tag);
			GDREGISTER_CLASS(TagContainer);
			
			break;
			
		case MODULE_INITIALIZATION_LEVEL_EDITOR:
			GDREGISTER_INTERNAL_CLASS(TagInspectorPlugin);
			GDREGISTER_INTERNAL_CLASS(TagPropertyEditor);
			GDREGISTER_INTERNAL_CLASS(TagEditor);
			GDREGISTER_INTERNAL_CLASS(TagEditorDock);
			GDREGISTER_INTERNAL_CLASS(TagPlugin);
			EditorPlugins::add_by_type<TagPlugin>();

			break;
	}
}

void uninitialize(ModuleInitializationLevel p_level) {
	
}

extern "C"
{
	// Initialization
	GDExtensionBool GDE_EXPORT GDTag_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, 
		GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization)
	{
		GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
		
		init_obj.register_initializer(initialize);
		init_obj.register_terminator(uninitialize);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}