#pragma once

#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/popup_menu.hpp>
#include <godot_cpp/variant/string.hpp>

using namespace godot;

class TagPropertyEditor : public VBoxContainer {
    GDCLASS(TagPropertyEditor, VBoxContainer);

public:
    TagPropertyEditor();

    void initialize(Object *owner, String p_property_name);

protected:
    static void _bind_methods();

private:
    void toggle_tag_editor();

    class Tag *get_tag();

    void select_tag(TypedArray<StringName> tag_path);

    VBoxContainer *container;
    HBoxContainer *h_layout;
    Label *property_label;
    Button *select_button;

    class TagEditor *editor;

    Object *owner;
    String property_name;
};