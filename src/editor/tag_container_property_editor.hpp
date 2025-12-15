#pragma once

#include "editor/gdtag_property_editor.hpp"

using namespace godot;

class TagContainerPropertyEditor : public GDTagPropertyEditor {
    GDCLASS(TagContainerPropertyEditor, GDTagPropertyEditor);

public:
    TagContainerPropertyEditor();
    ~TagContainerPropertyEditor() {}
    
    void _exit_tree() override;

    void initialize(Object *p_owner, String p_property_name) override;

protected:
    static void _bind_methods();

    void toggle_tag_editor() override;

    void select_tag(TypedArray<StringName> tag_path_arr) override;
    void unselect_tag(TypedArray<StringName> tag_path_arr);

private:
    void get_tag_container();

    void refresh_tag_text();

    Ref<class TagContainer> tag_container;
};