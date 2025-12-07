#include "tag_database.hpp"

#include <godot_cpp/variant/utility_functions.hpp>
#include "macros.h"
#include "tag_tree_item.hpp"

TagDatabase* TagDatabase::singleton = nullptr;

TagDatabase *TagDatabase::get_singleton() {
	if (singleton == nullptr) {
		singleton = memnew(TagDatabase);
    	singleton->initialize();
	}

	return singleton;
}

void TagDatabase::initialize() {
	nodes = Dictionary();
}

TagTreeItem *TagDatabase::get_tag(TypedArray<StringName> path) {
	UtilityFunctions::print("Getting tag with path: " + UtilityFunctions::str(path));

	if (path.size() == 0 || !nodes.has(path[0])) {
		UtilityFunctions::print("Did not find tag...");
		return nullptr;
	}
	
	Variant v = nodes[path[0]];
	TagTreeItem *current_tag = Object::cast_to<TagTreeItem>(v);

	for (size_t i = 0; i < path.size(); i++)
	{
		if (i == 0) {
			continue;
		}

		current_tag = current_tag->get_child(path[i]);

		if (current_tag == nullptr) {
			UtilityFunctions::print("Did not find tag...");
			return nullptr;
		}
	}

	UtilityFunctions::print("Found tag: " + (current_tag->get_name()));
	return current_tag;
}

void TagDatabase::add_tag(StringName name, TagTreeItem* parent) {
	UtilityFunctions::print("Adding tag: " + name);

	if (parent == nullptr) {
		if (nodes.has(name)) {
			return;
		}

		TagTreeItem* tag_item = memnew(TagTreeItem);
		tag_item->set_parent(nullptr);
		tag_item->set_name(name);
		nodes[name] = tag_item;
		
		UtilityFunctions::print("Tag added");
		return;
	}

	parent->add_child(name);
	UtilityFunctions::print("Tag added");
}

void TagDatabase::remove_tag(TagTreeItem *tag) {
	TagTreeItem *parent = tag->get_parent();
    StringName name = tag->get_name();
	UtilityFunctions::print("Removing tag: " + name);

	Array children = tag->get_children();
	for (size_t i = 0; i < children.size(); i++)
	{
		TagTreeItem *child_tag = cast_to<TagTreeItem>(children[i]);
		UtilityFunctions::print("Found child tag: " + child_tag->get_name());
		remove_tag(child_tag);
	}	
	
	if (parent == nullptr) {
		nodes.erase(name);
		memdelete(tag);
		UtilityFunctions::print("Tag deleted");
		return;
	}

	parent->remove_child(name);
	memdelete(tag);
	UtilityFunctions::print("Inner Tag deleted");
}

void TagDatabase::rename_tag(TagTreeItem* tag, StringName new_name) {
	TagTreeItem *parent = tag->get_parent();
    StringName name = tag->get_name();
	UtilityFunctions::print("Renaming tag: " + name);

	if (parent == nullptr) {
		nodes.erase(name);
		tag->set_name(new_name);
		nodes[new_name] = tag;
		UtilityFunctions::print("Tag renamed");
		return;
	}

	parent->remove_child(name);
	tag->set_name(new_name);
	parent->add_child(tag);
	UtilityFunctions::print("Tag renamed");
}

void TagDatabase::_bind_methods() {
}