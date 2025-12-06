#include "tag_database.hpp"

#include "macros.h"

TagDatabase* TagDatabase::singleton = nullptr;

TagDatabase *TagDatabase::get_singleton() {
	if (singleton == nullptr) {
		singleton = memnew(TagDatabase);
	}

	return singleton;
}

void TagDatabase::initialize() {
	nodes = Dictionary();
}

TagTreeItem *TagDatabase::get_tag(TypedArray<StringName> path) {
	if (path.size() == 0 || !nodes.has(path[0])) {
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
			return nullptr;
		}
	}

	return current_tag;
}

void TagDatabase::add_tag(StringName name, TagTreeItem* parent) {
	if (parent == nullptr) {
		if (!nodes.has(name)) {
			return;
		}

		TagTreeItem* tag_item = memnew(TagTreeItem);
		tag_item->set_name(name);
		nodes[name] = tag_item;
		return;
	}

	parent->add_child(name);
}

void TagDatabase::remove_tag(TagTreeItem *tag) {
}

void TagDatabase::rename_tag(TagTreeItem tag, StringName new_name) {
}

void TagDatabase::_bind_methods() {
}