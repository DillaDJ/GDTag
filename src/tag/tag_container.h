#pragma once

#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class TagContainer : public Resource {
    GDCLASS(TagContainer, Resource);
    
public:
    TagContainer();

    void add_tag_internal(class InternalTag *tag);
    void remove_tag_internal(InternalTag *tag);

    TypedArray<int> get_tag_ids() { return tag_ids.keys(); }

    bool has(class Tag *tag);
    bool any(TagContainer *container);
    bool exact(TagContainer *container);
    bool all(TagContainer *container);
    bool none(TagContainer *container);

    int overlap_count(TagContainer *container);
    int size() { return tag_ids.size(); }
    
    void set_tag_id_map(Dictionary id_map) { tag_ids = id_map; }
    Dictionary get_tag_id_map() { return tag_ids; }

protected:
    static void _bind_methods();

private:
    Dictionary tag_ids; // int, true
};