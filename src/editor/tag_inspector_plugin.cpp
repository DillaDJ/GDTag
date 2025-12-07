#include "tag_inspector_plugin.h"
#include "tag_property_editor.h"
#include "tag/tag.h"

bool TagInspectorPlugin::_can_handle(Object *p_object) const {
    return p_object->is_class("Tag");
}

bool TagInspectorPlugin::_parse_property(Object *p_object, Variant::Type p_type, 
    const String &p_name, PropertyHint p_hint_type, const String &p_hint_string, 
    BitField<PropertyUsageFlags> p_usage_flags, bool p_wide) 
{
    if (p_name == StringName("linkedTag")) {
        add_custom_control(memnew(TagPropertyEditor));
        return true;
    }
    return false;
}

void TagInspectorPlugin::_parse_begin(Object *p_object) {
}

void TagInspectorPlugin::_bind_methods() {
}