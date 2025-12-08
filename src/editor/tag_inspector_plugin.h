#pragma once

#include <godot_cpp/classes/editor_inspector_plugin.hpp>

using namespace godot;

class TagInspectorPlugin : public EditorInspectorPlugin {
    GDCLASS(TagInspectorPlugin, EditorInspectorPlugin);

public:
    bool _can_handle(Object *p_object) const override;
    bool _parse_property(Object *p_object, Variant::Type p_type, const String &p_name, 
        PropertyHint p_hint_type, const String &p_hint_string, BitField<PropertyUsageFlags> p_usage_flags, 
        bool p_wide) override;
	
    
protected:
    static void _bind_methods();
};