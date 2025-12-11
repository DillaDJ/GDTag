#include "tag_inspector_plugin.h"
#include "tag_property_editor.h"
#include "tag/tag.h"

bool TagInspectorPlugin::_can_handle(Object *p_object) const {
    return true;
}

bool TagInspectorPlugin::_parse_property(Object *p_object, Variant::Type p_type, const String &p_name, 
    PropertyHint p_hint_type, const String &p_hint_string, BitField<PropertyUsageFlags> p_usage_flags, 
    bool p_wide) {
    TypedArray<Dictionary> property_list = p_object->get_property_list();
    if (p_hint_string == Tag::get_class_static()) {        
        TagPropertyEditor* tag_editor = memnew(TagPropertyEditor);
        tag_editor->initialize(p_object, p_name);
        add_custom_control(tag_editor);
        return true;
    }

	return false;
}

void TagInspectorPlugin::_bind_methods() {
}