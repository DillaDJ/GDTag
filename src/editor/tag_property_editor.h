#pragma once

#include <godot_cpp/classes/editor_property.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/popup_menu.hpp>

using namespace godot;

class TagPropertyEditor : public EditorProperty {
    GDCLASS(TagPropertyEditor, EditorProperty);

public:
    TagPropertyEditor();
    
    virtual void _update_property() override;
    
protected:
    static void _bind_methods();

private:
    void _on_tag_selected(int32_t index);
    
    Button *select_button;
};