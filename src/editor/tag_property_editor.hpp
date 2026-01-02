#pragma once

#include "editor/gdtag_property_editor.hpp"
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/popup_menu.hpp>
#include <godot_cpp/variant/string.hpp>

using namespace godot;

class TagPropertyEditor : public GDTagPropertyEditor {
    GDCLASS(TagPropertyEditor, GDTagPropertyEditor);

public:
    TagPropertyEditor();
    ~TagPropertyEditor() {}
    
    void _exit_tree() override;

    void initialize(Object *p_owner, String p_property_name) override;

protected:
    static void _bind_methods() { };

    void toggle_tag_editor() override;

    void select_tag(TypedArray<StringName> tag_path_arr) override;
    
private:
    void get_tag();

    void refresh_button();

    Ref<class Tag> tag;
};