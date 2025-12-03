#pragma once

#include <godot_cpp/classes/editor_plugin.hpp>

using namespace godot;

class TagPlugin : public EditorPlugin {
    GDCLASS(TagPlugin, EditorPlugin);
  
public:
    TagPlugin() { }
    ~TagPlugin() { }

    void _enter_tree() override;
    
protected:
    static void _bind_methods() { }
    void OpenTagEditor();
    
private:
	class TagEditor *editor;
};