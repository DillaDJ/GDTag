#pragma once

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/v_box_container.hpp>

using namespace godot;

class TagEditor : public Control {
    GDCLASS(TagEditor, Control);

public:
    TagEditor();
    ~TagEditor() { }

	static TagEditor *get_singleton() { return singleton; }
    
	void initialize();

protected:
    static void _bind_methods() { }

private:
    static TagEditor *singleton;

    VBoxContainer *main_vbox;
};