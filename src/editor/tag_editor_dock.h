#pragma once

#include <godot_cpp/variant/string_name.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/tree.hpp>
#include <godot_cpp/classes/confirmation_dialog.hpp>

using namespace godot;

class TagEditorDock : public Control {
    GDCLASS(TagEditorDock, Control);

public:
    TagEditorDock();
    ~TagEditorDock() { }
    
protected:
    static void _bind_methods();
	
private:    
    class TagEditor *editor;
};