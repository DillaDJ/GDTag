#include "tag_database.hpp"

#include "macros.h"
#include "tag_tree_item.h"


TagDatabase* TagDatabase::singleton = nullptr;

void TagDatabase::initialize() {
	nodes = TypedDictionary<StringName, TagTreeItem>();
}

TagTreeItem *TagDatabase::get_tag(TypedArray<StringName> path) {
	if (path.size() == 0 || !nodes.has(path[0])) {
		return nullptr;
	}
	
	TagTreeItem *current_tag = cast_to<TagTreeItem>(nodes[path[0]]);

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

void TagDatabase::add_tag(StringName name, TagTreeItem* parent = nullptr) {
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

void TagDatabase::rename_tag(TagTreeItem tag, StringName new_name) {
}

void TagDatabase::_bind_methods() {
}