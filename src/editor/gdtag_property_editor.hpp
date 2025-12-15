#pragma once

#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/popup_menu.hpp>
#include <godot_cpp/variant/string.hpp>

using namespace godot;

class GDTagPropertyEditor : public VBoxContainer {
    GDCLASS(GDTagPropertyEditor, VBoxContainer);

public:
    GDTagPropertyEditor();
    ~GDTagPropertyEditor() {}
    
    void _exit_tree() override;

    virtual void initialize(Object *p_owner, String p_property_name);

protected:
    static void _bind_methods() {};

    virtual void toggle_tag_editor();

    virtual void select_tag(TypedArray<StringName> tag_path_arr) = 0;

    class TagEditor *editor;
    
    Object *owner;
    String property_name;
    
    Button *select_button;
    
private:
    VBoxContainer *container;
    HBoxContainer *h_layout;
    Label *property_label;
};