#include "tag_tree_item.hpp"
#include "internal/helpers.hpp"
#include <godot_cpp/variant/utility_functions.hpp>

TagTreeItem::TagTreeItem() {
    children = Dictionary();
}

TagTreeItem::~TagTreeItem() {
}

void TagTreeItem::add_child(StringName name) {
    if (children.has(name)) {
        return;
    }
    
	TagTreeItem* tag_item = memnew(TagTreeItem);
    tag_item->set_parent(this);
	tag_item->set_name(name);
    children[name] = tag_item;
}

void TagTreeItem::add_child(TagTreeItem *tag) {
    StringName name = tag->get_name();

    if (children.has(name)) {
        return;
    }
    
	children[name] = tag;
    tag->set_parent(this);
}

TagTreeItem *TagTreeItem::get_child(StringName name) {
	return children.has(name) ? cast_to<TagTreeItem>(children[name]) : nullptr;
}

void TagTreeItem::remove_child(StringName name) {
    if (!children.has(name)) return;
	
    TagTreeItem *tag = cast_to<TagTreeItem>(children[name]);
    tag->set_parent(nullptr);
    children.erase(name);
}

TypedArray<StringName> TagTreeItem::get_path_arr() {
    TypedArray<StringName> path_arr = TypedArray<StringName>();
	TagTreeItem *current = this;
        
    while (current != nullptr) {
        path_arr.append(current->get_name());
        current = current->get_parent();
    }

    path_arr.reverse();
	return path_arr;
}

StringName TagTreeItem::get_path() {
	StringName path = "";

	TypedArray<StringName> path_arr = get_path_arr();
	for (size_t i = 0; i < path_arr.size(); i++)
	{
		StringName path_element = (StringName) path_arr[i] + SNAME("/");
		path = path + path_element;
	}

	return path;
}

void TagTreeItem::populate_children(Array loaded_tags) {
	for (size_t i = 0; i < loaded_tags.size(); i++)
	{
		Variant tag = loaded_tags[i];
		
		if (tag.get_type() == Variant::Type::ARRAY) {
			Array child_tags = (Array) tag; 
			
			StringName name = (StringName) child_tags[0];
			add_child(name);
			
			TagTreeItem *loaded_tag = cast_to<TagTreeItem>(children[name]);
			loaded_tag->populate_children(child_tags[1]);

			return;
		}

		if (tag.get_type() == Variant::Type::STRING || tag.get_type() == Variant::Type::STRING_NAME) {
			StringName name = (StringName) tag;
			add_child(name);
		}
	}
}

Array TagTreeItem::get_children_names_recursive() {
	Array tags = children.keys();

    for (size_t i = 0; i < tags.size(); i++)
	{
		TagTreeItem *tag = cast_to<TagTreeItem>(children[tags[i]]);
		Array children = tag->get_children_names_recursive();
		
		if (children.size() > 0) {
            Array inner = Array();
            inner.append(tags[i]);
			inner.append(children);
            tags[i] = inner;
		}
	}

	return tags;
}

void TagTreeItem::_bind_methods() {
}