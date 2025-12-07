#include "tag_tree_item.hpp"

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

void TagTreeItem::_bind_methods() {
}