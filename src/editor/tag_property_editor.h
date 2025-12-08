#pragma once

#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/popup_menu.hpp>

using namespace godot;

class TagPropertyEditor : public VBoxContainer {
    GDCLASS(TagPropertyEditor, VBoxContainer);

public:
    TagPropertyEditor();

    void set_property_name(String name);

protected:
    static void _bind_methods();

private:
    MarginContainer *container;
    HBoxContainer *h_layout;
    Label *property_name;
    Button *select_button;
};