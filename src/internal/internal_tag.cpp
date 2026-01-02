#include "internal_tag.hpp"
#include "internal/helpers.hpp"
#include <godot_cpp/variant/utility_functions.hpp>

#include "internal/tag_database.hpp"

InternalTag::InternalTag() {
    children = Dictionary();
	id = 0;
}

InternalTag *InternalTag::add_child(StringName name) {
    if (children.has(name)) {
        return nullptr;
    }
    
	InternalTag* tag_item = memnew(InternalTag);
    tag_item->set_parent(this);
	tag_item->set_tag_name(name);
    children[name] = tag_item;

	return tag_item;
}

void InternalTag::add_child(InternalTag *tag) {
    StringName name = tag->get_tag_name();

    if (children.has(name)) {
        return;
    }
    
	children[name] = tag;
    tag->set_parent(this);
}

InternalTag *InternalTag::get_child(StringName name) {
	return children.has(name) ? cast_to<InternalTag>(children[name]) : nullptr;
}

void InternalTag::remove_child(StringName name) {
    if (!children.has(name)) return;
	
    InternalTag *tag = cast_to<InternalTag>(children[name]);
    tag->set_parent(nullptr);
    children.erase(name);
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

void InternalTag::populate_children(Array loaded_tags) {
	for (size_t i = 0; i < loaded_tags.size(); i++)
	{
		Variant tag = loaded_tags[i];
		
		if (tag.get_type() == Variant::Type::ARRAY) {
			Array child_tags = (Array) tag; 
			
			StringName name = (StringName) child_tags[0];
			add_child(name);
			
			InternalTag *loaded_tag = cast_to<InternalTag>(children[name]);
			loaded_tag->populate_children(child_tags[1]);

			return;
		}

		if (tag.get_type() == Variant::Type::STRING || tag.get_type() == Variant::Type::STRING_NAME) {
			StringName name = (StringName) tag;
			add_child(name);
		}
	}
}