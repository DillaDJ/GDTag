#pragma once

#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class Tag : public Resource {
    GDCLASS(Tag, Resource);
    
public:    
    void set_tag(Tag *tag);
    
    class InternalTag *get_internal_tag();
    void link_internal_tag(InternalTag *internal_tag);

    bool match(Tag *tag);
    bool match_inheritance(Tag *tag);

    StringName get_tag_path();

    int get_linked_id();

protected:
    static void _bind_methods();

private:
    void set_linked_id(int value);

    int linked_id = -1;
};