#include "tag_tree_item.h"

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
	tag_item->set_name(name);
    children[name] = tag_item;
}

TagTreeItem *TagTreeItem::get_child(StringName name) {
	return children.has(name) ? cast_to<TagTreeItem>(children[name]) : nullptr;
}

void TagTreeItem::_bind_methods() {
}