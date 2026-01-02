#pragma once

#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class Tag : public Resource {
    GDCLASS(Tag, Resource);
    
public:    
    class InternalTag *get_tag();

    bool match(Tag *tag);
    bool match_inheritance(Tag *tag);

    void set_tag_path(StringName path);
    StringName get_tag_path() { return tag_path; }

protected:
    static void _bind_methods();

private:
    class StringName tag_path;
};