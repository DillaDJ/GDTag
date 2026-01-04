#include "internal_tag.hpp"
#include "internal/helpers.hpp"
#include <godot_cpp/variant/utility_functions.hpp>

#include "internal/tag_database.hpp"

InternalTag::InternalTag() {
    children = Dictionary();
	parent = nullptr;
	id = 0;
}

void InternalTag::add_child(InternalTag *tag) {
    int id = tag->get_id();

    if (children.has(id)) {
        return;
    }
    
	children[id] = tag;
    tag->set_parent(this);
}

InternalTag *InternalTag::get_child(int id) {
	return children.has(id) ? cast_to<InternalTag>(children[id]) : nullptr;
}

InternalTag *InternalTag::get_child(StringName name) {
	TagDatabase *database = TagDatabase::get_singleton();
	Array child_nodes = children.keys();

	for (size_t i = 0; i < child_nodes.size(); i++)
	{
		int id = child_nodes[i];
		InternalTag *tag = database->get_tag(id);
		if (tag-> get_tag_name() != name) {
			continue;
		}

		return tag;
	}
	
	return nullptr;
}

void InternalTag::remove_child(InternalTag *tag) {
	int id = tag->get_id();

    if (!children.has(id)) {
		return;
	}
	
    tag->set_parent(nullptr);
    children.erase(id);
}

TypedArray<StringName> InternalTag::get_path_arr() {
    TypedArray<StringName> path_arr = TypedArray<StringName>();
	InternalTag *current = this;
        
    while (current != nullptr) {
        path_arr.append(current->get_tag_name());
        current = current->get_parent();
    }

    path_arr.reverse();
	return path_arr;
}

StringName InternalTag::get_path() {
	StringName path = "";

	TypedArray<StringName> path_arr = get_path_arr();
	for (size_t i = 0; i < path_arr.size(); i++)
	{
		StringName path_element = (StringName) path_arr[i] + SNAME("/");
		path = path + path_element;
	}

	return path;
}